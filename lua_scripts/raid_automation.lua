--[[
    ================================================================================
    Tortoise-WoW Autonomous Raid & Group Controller (Eluna Lua Engine)
    ================================================================================
    Target Platform: Turtle-WoW 1.18.1 (Build 7272)
    Architecture: CMaNGOS + mod-playerbots + mod-dungeon-clear + Eluna
    Source References:
        - references/01_game_master_handbook.md (Instance unbinds, repairs, replenish)
        - references/02_playerbot_and_raid_automation.md (Bot controls, roles, stances)
        - references/03_turtle_world_and_teleports.md (Attunements, keys, teleports)
        - references/04_class_specs_and_endgame_gear.md (BiS gear specs)
        - references/05_developer_eluna_and_engine.md (Eluna Lua API, hooks, events)
        - references/legacy_22_modules/20_warrior_dps_dungeons_and_raids.md (Keys)
    ================================================================================
]]

local SCRIPT_NAME = "RaidAutomation"
local SCRIPT_VERSION = "1.1.0"

-- Event Definitions (from Eluna Hooks.h)
local PLAYER_EVENT_ON_COMMAND = 42
local MAP_EVENT_ON_PLAYER_ENTER = 21

-- Verified Raid Buff Spell IDs (Classic / Turtle-WoW 1.18.1)
local CLASS_RAID_BUFFS = {
    -- Priest
    21564, -- Prayer of Fortitude (Rank 2: +54 Stamina)
    21850, -- Prayer of Shadow Protection (Rank 1: +60 Shadow Resist)
    27681, -- Prayer of Spirit (Rank 1: +40 Spirit)
    -- Mage
    23028, -- Arcane Brilliance (Rank 1: +31 Intellect)
    -- Druid
    9885,  -- Gift of the Wild (Rank 2: +12 Stats, +285 Armor, +20 Resists)
    -- Paladin
    25898  -- Greater Blessing of Kings (+10% All Total Stats)
}

local WORLD_BUFFS = {
    22888, -- Rallying Cry of the Dragonslayer (Onyxia / Nefarian head)
    15366, -- Songflower Serenade (Felwood +5% Crit, +15 all stats)
    16609, -- Warchief's Blessing (Rend head, 300 HP, 15% melee haste)
    24425  -- Spirit of Zandalar (Heart of Hakkar, +15% all stats, +10% speed)
}

-- Master Attunement Quest IDs (from Volume 03 & Module 20)
local ATTUNEMENT_QUESTS = {
    { id = 7848,  name = "Molten Core (Attunement to the Core)" },
    { id = 7761,  name = "Blackwing Lair (Blackhand's Command)" },
    { id = 6502,  name = "Onyxia's Lair - Alliance (Drakefire Amulet)" },
    { id = 6602,  name = "Onyxia's Lair - Horde (Blood of the Black Dragon)" },
    { id = 9123,  name = "Naxxramas Exalted (The Dread Citadel)" },
    { id = 40962, name = "Emerald Sanctum 40-Man (Into the Dream VI)" },
    { id = 40829, name = "Tower of Karazhan (The Key to Karazhan X)" },
    { id = 41931, name = "Timbermaw Hold 20-Man (Corruption of Timbermaw)" },
    { id = 40489, name = "Hateforge Quarry 5-Man (Assaulting Hateforge)" }
}

-- Master Keyring & Access Item IDs (from Volume 03 & Module 20)
local MASTER_KEYS = {
    { id = 16309, name = "Drakefire Amulet (Onyxia Key)" },
    { id = 22754, name = "Eternal Quintessence (MC Infinite Rune Dousing)" },
    { id = 12344, name = "Seal of Ascension (UBRS 10-Man Gate Key)" },
    { id = 13704, name = "Skeleton Key (Scholomance Front Gate)" },
    { id = 12382, name = "Key to the City (Stratholme Service Gate)" },
    { id = 11000, name = "Shadowforge Key (BRD Gates & Mechanism)" },
    { id = 18249, name = "Crescent Key (Dire Maul West & North Gates)" },
    { id = 7146,  name = "The Scarlet Key (Scarlet Monastery Doors)" },
    { id = 9287,  name = "Workshop Key (Gnomeregan Back Entrance)" },
    { id = 50545, name = "Gemstone of Ysera (Emerald Sanctum 40m Key)" },
    { id = 61234, name = "Upper Karazhan Tower Key (Tower of Karazhan)" },
    { id = 51356, name = "Karazhan Crypt Key (Karazhan Crypts Entrance)" },
    { id = 42379, name = "Rune of Illumination (Timbermaw Hold 20m Key)" },
    { id = 53005, name = "Scarlet Crusade Insignia (Scarlet Citadel 10m)" }
}

-- Fast Teleport Presets (Classic & Turtle Custom Instances from Volume 03)
local TELEPORT_PRESETS = {
    ["mc"]        = "molten",
    ["molten"]    = "molten",
    ["bwl"]       = "bwl",
    ["blackwing"] = "bwl",
    ["zg"]        = "zg",
    ["zulgurub"]  = "zg",
    ["aq20"]      = "aq20",
    ["aq40"]      = "aq40",
    ["naxx"]      = "naxxramas",
    ["naxxramas"] = "naxxramas",
    ["ony"]       = "ony",
    ["onyxia"]    = "ony",
    ["ubrs"]      = "ubrs",
    ["lbrs"]      = "lbrs",
    ["brd"]       = "brd",
    ["strat"]     = "strat",
    ["scholo"]    = "scholo",
    ["dm"]        = "dmnorth",
    ["sm"]        = "sm",
    ["st"]        = "st",
    ["zf"]        = "zf",
    -- Turtle-WoW Custom Instances
    ["scarlet"]   = "ScarletCitadel",
    ["karazhan"]  = "Karazhan",
    ["crypt"]     = "karazhancrypt",
    ["sanctum"]   = "sanctumraid",
    ["timbermaw"] = "TimbermawHold",
    ["hateforge"] = "hateforgeinstance",
    ["crescent"]  = "crescentgrove",
    ["vault"]     = "stormwindvault",
    ["gilneas"]   = "gilneascity"
}

-- Utility: Send styled system messages
local function SendRaidMsg(player, msg)
    if player then
        player:SendBroadcastMessage("|cFF00FF00[Raid Controller]|r " .. msg)
    end
end

local function SendRaidError(player, msg)
    if player then
        player:SendBroadcastMessage("|cFFFF3333[Raid Controller Error]|r " .. msg)
    end
end

-- ================================================================================
-- Module 1: Raid Buff Engine
-- ================================================================================
local function ApplyRaidBuffs(player, includeWorldBuffs)
    local group = player:GetGroup()
    if not group then
        for _, spellId in ipairs(CLASS_RAID_BUFFS) do
            player:CastSpell(player, spellId, true)
        end
        if includeWorldBuffs then
            for _, spellId in ipairs(WORLD_BUFFS) do
                player:CastSpell(player, spellId, true)
            end
        end
        SendRaidMsg(player, "Applied full raid buffs to yourself.")
        return
    end

    local members = group:GetMembers()
    local count = 0
    for _, member in ipairs(members) do
        if member and member:IsAlive() then
            for _, spellId in ipairs(CLASS_RAID_BUFFS) do
                member:CastSpell(member, spellId, true)
            end
            if includeWorldBuffs then
                for _, spellId in ipairs(WORLD_BUFFS) do
                    member:CastSpell(member, spellId, true)
                end
            end
            count = count + 1
        end
    end
    local wStr = includeWorldBuffs and " + World Buffs" or ""
    SendRaidMsg(player, string.format(
        "Buffed all %d raid members with complete class buffs%s!",
        count, wStr))
end

-- ================================================================================
-- Module 2: Automated Gear, Enchants & Stocking (.bot gear/prep)
-- NOTE: Does NOT modify character levels. Bots are level 60 upon LFG invite.
-- Defaults to Epic gear quality per user specification.
-- ================================================================================
local function PrepareRaidBots(player, gearQuality)
    local quality = gearQuality or "epic"
    SendRaidMsg(player, string.format(
        "Gearing bots (%s), applying enchants, spells, and stocking...",
        quality))

    -- Native playerbot server commands from Volume 02 & PlayerbotMgr.cpp
    player:RunCommand("bot gear * " .. quality)
    player:RunCommand("bot enchants *")
    player:RunCommand("bot train *")
    player:RunCommand("bot prep *")
    player:RunCommand("bot consumes *")
    player:RunCommand("bot food *")
    player:RunCommand("bot potions *")
    player:RunCommand("bot regs *")
    player:RunCommand("bot ammo *")
    player:RunCommand("repairitems")
    player:RunCommand("replenish")

    SendRaidMsg(player, string.format(
        "All bots equipped with %s gear, permanent enchants, food & pots!",
        quality:upper()))
end

-- ================================================================================
-- Module 3: Automatic Role & Strategy Assignment (Volume 02 & mod-dungeon-clear)
-- Sets up roles and AI strategies using native server 'bot' commands.
-- ================================================================================
local function ConfigureRaidRoles(player)
    local group = player:GetGroup()
    if not group then
        SendRaidError(player, "You are not in a group or raid!")
        return
    end

    local members = group:GetMembers()
    local total = #members
    local tanksAssigned = 0
    local healersAssigned = 0
    local dpsAssigned = 0

    -- Target role quotas based on raid size
    local maxTanks = 1
    local maxHealers = 1
    if total >= 40 then
        maxTanks = 5
        maxHealers = 10
    elseif total >= 20 then
        maxTanks = 3
        maxHealers = 5
    elseif total >= 10 then
        maxTanks = 2
        maxHealers = 3
    end

    SendRaidMsg(player, string.format(
        "Configuring roles for %d members (Target: %dT / %dH / %dDPS)...",
        total, maxTanks, maxHealers, total - maxTanks - maxHealers))

    for _, member in ipairs(members) do
        -- Skip the player (master)
        if member:GetGUIDLow() ~= player:GetGUIDLow() then
            local name = member:GetName()
            local classId = member:GetClass()

            -- Warrior (Class 1)
            if classId == 1 then
                if tanksAssigned < maxTanks then
                    tanksAssigned = tanksAssigned + 1
                    player:RunCommand(string.format("bot role %s tank", name))
                    player:RunCommand(string.format(
                        "bot c %s co +tank,prot,tank assist,avoid aoe,potions",
                        name))
                    player:RunCommand(string.format("bot c %s nc +food", name))
                    player:RunCommand(string.format(
                        "bot formation %s shield", name))
                    player:RunCommand(string.format("bot stance %s tank", name))
                else
                    dpsAssigned = dpsAssigned + 1
                    player:RunCommand(string.format("bot role %s dps", name))
                    player:RunCommand(string.format(
                        "bot c %s co -tank,-prot,+dps,+fury,+behind,+interrupt,+potions",
                        name))
                    player:RunCommand(string.format("bot c %s nc +food", name))
                    player:RunCommand(string.format(
                        "bot formation %s melee", name))
                    player:RunCommand(string.format(
                        "bot stance %s behind", name))
                end

                -- Paladin (Class 2)
            elseif classId == 2 then
                if healersAssigned < maxHealers then
                    healersAssigned = healersAssigned + 1
                    player:RunCommand(string.format("bot role %s heal", name))
                    player:RunCommand(string.format(
                        "bot c %s co +heal,cure,buff,potions", name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+buff", name))
                    player:RunCommand(string.format("bot formation %s back", name))
                elseif tanksAssigned < maxTanks then
                    tanksAssigned = tanksAssigned + 1
                    player:RunCommand(string.format("bot role %s tank", name))
                    player:RunCommand(string.format(
                        "bot c %s co +tank,prot,consecration,avoid aoe,potions",
                        name))
                    player:RunCommand(string.format("bot c %s nc +food", name))
                    player:RunCommand(string.format(
                        "bot formation %s shield", name))
                else
                    dpsAssigned = dpsAssigned + 1
                    player:RunCommand(string.format("bot role %s dps", name))
                    player:RunCommand(string.format(
                        "bot c %s co +dps,retrib,consecration,potions", name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+buff", name))
                    player:RunCommand(string.format(
                        "bot formation %s melee", name))
                end

                -- Hunter (Class 3)
            elseif classId == 3 then
                dpsAssigned = dpsAssigned + 1
                player:RunCommand(string.format("bot role %s dps", name))
                player:RunCommand(string.format(
                    "bot c %s co +dps,ranged,pet,ammo,potions", name))
                player:RunCommand(string.format("bot c %s nc +food", name))
                player:RunCommand(string.format("bot formation %s far", name))

                -- Rogue (Class 4)
            elseif classId == 4 then
                dpsAssigned = dpsAssigned + 1
                player:RunCommand(string.format("bot role %s dps", name))
                player:RunCommand(string.format(
                    "bot c %s co +dps,behind,interrupt,stealth,evasion,potions",
                    name))
                player:RunCommand(string.format(
                    "bot c %s nc +food,+stealth", name))
                player:RunCommand(string.format("bot formation %s melee", name))
                player:RunCommand(string.format("bot stance %s behind", name))

                -- Priest (Class 5)
            elseif classId == 5 then
                if healersAssigned < maxHealers then
                    healersAssigned = healersAssigned + 1
                    player:RunCommand(string.format("bot role %s heal", name))
                    player:RunCommand(string.format(
                        "bot c %s co +heal,cure,buff,potions", name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+buff", name))
                    player:RunCommand(string.format("bot formation %s back", name))
                else
                    dpsAssigned = dpsAssigned + 1
                    player:RunCommand(string.format("bot role %s dps", name))
                    player:RunCommand(string.format(
                        "bot c %s co +dps,shadow,shadowform,cure,potions",
                        name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+buff", name))
                    player:RunCommand(string.format("bot formation %s line", name))
                end

                -- Shaman (Class 7)
            elseif classId == 7 then
                if healersAssigned < maxHealers then
                    healersAssigned = healersAssigned + 1
                    player:RunCommand(string.format("bot role %s heal", name))
                    player:RunCommand(string.format(
                        "bot c %s co +heal,cure,totems,potions", name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+buff", name))
                    player:RunCommand(string.format("bot formation %s back", name))
                else
                    dpsAssigned = dpsAssigned + 1
                    player:RunCommand(string.format("bot role %s dps", name))
                    player:RunCommand(string.format(
                        "bot c %s co +dps,windfury totem,totems,interrupt,potions",
                        name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+buff", name))
                    player:RunCommand(string.format(
                        "bot formation %s melee", name))
                end

                -- Mage (Class 8)
            elseif classId == 8 then
                dpsAssigned = dpsAssigned + 1
                player:RunCommand(string.format("bot role %s dps", name))
                player:RunCommand(string.format(
                    "bot c %s co +dps,fire,aoe,interrupt,potions", name))
                player:RunCommand(string.format(
                    "bot c %s nc +food,+buff", name))
                player:RunCommand(string.format("bot formation %s line", name))

                -- Warlock (Class 9)
            elseif classId == 9 then
                dpsAssigned = dpsAssigned + 1
                player:RunCommand(string.format("bot role %s dps", name))
                player:RunCommand(string.format(
                    "bot c %s co +dps,shadow,pet,curse,potions", name))
                player:RunCommand(string.format(
                    "bot c %s nc +food,+buff", name))
                player:RunCommand(string.format("bot formation %s line", name))

                -- Druid (Class 11)
            elseif classId == 11 then
                if healersAssigned < maxHealers then
                    healersAssigned = healersAssigned + 1
                    player:RunCommand(string.format("bot role %s heal", name))
                    player:RunCommand(string.format(
                        "bot c %s co +heal,cure,buff,potions", name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+buff", name))
                    player:RunCommand(string.format("bot formation %s back", name))
                elseif tanksAssigned < maxTanks then
                    tanksAssigned = tanksAssigned + 1
                    player:RunCommand(string.format("bot role %s tank", name))
                    player:RunCommand(string.format(
                        "bot c %s co +tank,bear,tank assist,avoid aoe,potions",
                        name))
                    player:RunCommand(string.format("bot c %s nc +food", name))
                    player:RunCommand(string.format(
                        "bot formation %s shield", name))
                else
                    dpsAssigned = dpsAssigned + 1
                    player:RunCommand(string.format("bot role %s dps", name))
                    player:RunCommand(string.format(
                        "bot c %s co +dps,cat,behind,stealth,potions", name))
                    player:RunCommand(string.format(
                        "bot c %s nc +food,+stealth", name))
                    player:RunCommand(string.format(
                        "bot formation %s melee", name))
                    player:RunCommand(string.format(
                        "bot stance %s behind", name))
                end
            end
        end
    end

    SendRaidMsg(player, string.format(
        "Roles configured: %d Tanks, %d Healers, %d DPS assigned!",
        tanksAssigned, healersAssigned, dpsAssigned))
end

-- ================================================================================
-- Module 4: Automatic Raid Attunement & Master Keyring Suite
-- Replaces generic quest sharing with actual raid attunements and physical keys
-- from Volume 03 (World & Teleports) and Module 20 (Endgame Attunements).
-- ================================================================================
local function AttunePlayerAndRaid(player)
    SendRaidMsg(player, "Unlocking all Classic & Turtle-WoW raid attunements...")

    -- 1. Complete all Raid & Dungeon Attunement Quests
    for _, q in ipairs(ATTUNEMENT_QUESTS) do
        player:RunCommand("quest add " .. q.id)
        player:RunCommand("quest complete " .. q.id)
    end

    -- 2. Grant Exalted Argent Dawn reputation for Naxxramas access
    player:RunCommand("modify rep 529 42999")

    -- 3. Spawn Master Physical Keys into player inventory
    for _, k in ipairs(MASTER_KEYS) do
        player:RunCommand(string.format("additem %d 1", k.id))
    end

    SendRaidMsg(player, "|cFF00FF00Attunement Complete!|r MC, BWL, Onyxia, Naxx, Karazhan, Emerald Sanctum unlocked.")
    SendRaidMsg(player, "Master keyring delivered: UBRS, Scholo, Strath, BRD, Dire Maul, SM keys in bags.")
end

-- ================================================================================
-- Module 5: Master 1-Click LFG Raid Preparation Pipeline (.raid ready)
-- Automates bots assembled via the in-game group finder (LFT / LFG).
-- Does NOT modify levels. Does NOT force group conversions.
-- ================================================================================
local function SetupActiveRaid(player, includeWorldBuffs, gearQuality)
    local group = player:GetGroup()
    if not group then
        SendRaidError(player, "No active group or raid found! Assemble your group with the LFG tool first.")
        return
    end

    local count = group:GetMembersCount()
    local isRaid = group:IsRaidGroup()
    SendRaidMsg(player, string.format(
        "Initializing LFG %s (%d members)...",
        isRaid and "Raid" or "Party", count))

    -- 1. Summon all bots & lock follow
    player:RunCommand("bot summon *")
    player:RunCommand("bot p follow")
    player:RunCommand("bot r follow")
    player:RunCommand("bot c * follow")
    player:RunCommand("bot c * stay off")
    player:RunCommand("bot c * flee off")

    -- 2. Gearing, Enchants, Spells & Consumables (default: epic)
    PrepareRaidBots(player, gearQuality or "epic")

    -- 3. Automatic Roles, Combat Strategies & Formations
    ConfigureRaidRoles(player)

    -- 4. Instant Raid Attunements & Master Keyring Suite
    AttunePlayerAndRaid(player)

    -- 5. Instant Class Raid Buffs (+ Optional World Buffs)
    ApplyRaidBuffs(player, includeWorldBuffs)

    SendRaidMsg(player, string.format(
        "|cFF00FF00Raid Ready!|r All %d characters geared, role-tuned, attuned, and buffed.",
        count))
    SendRaidMsg(player, "Inside the instance, type |cFF00FFFF.raid start|r to begin autonomous clearing (.dc on).")
end

-- ================================================================================
-- Module 6: Autonomous Dungeon Clear Controller (.dc)
-- ================================================================================
local function StartDungeonClear(player)
    local map = player:GetMap()
    if not map:IsDungeon() and not map:IsRaid() then
        SendRaidError(player, "You must be inside a dungeon or raid instance to start autonomous clear (.dc on)!")
        return
    end

    SendRaidMsg(player, "Starting Autonomous Dungeon Clear engine on lead tank...")
    player:RunCommand("bot summon *")
    player:RunCommand("bot p follow")
    player:RunCommand("bot r follow")
    player:RunCommand("bot c * follow")

    -- Engage mod-dungeon-clear on the configured tank bot
    player:RunCommand("dc on")
    player:RunCommand("dc pull dynamic")
    SendRaidMsg(player, "Tank autopilot engaged! Mode: Dynamic pull. Party synced.")
end

local function PauseDungeonClear(player)
    SendRaidMsg(player, "Pausing Autonomous Dungeon Clear and holding party...")
    player:RunCommand("dc pause")
    player:RunCommand("bot p flee")
    player:RunCommand("bot r flee")
    player:RunCommand("bot c * flee")
    player:RunCommand("combatstop")
    SendRaidMsg(player, "Combat stopped. Group holding position.")
end

-- ================================================================================
-- Module 7: Main In-Game Command Handler (.raid ...)
-- ================================================================================
local function OnRaidCommand(event, player, command)
    -- Explicitly validate event trigger
    if event ~= PLAYER_EVENT_ON_COMMAND then
        return true
    end

    local cmd, arg1, arg2 = command:match("^(%S+)%s*(%S*)%s*(.*)$")
    if not cmd then return true end

    if cmd:lower() == "raid" then
        local sub = arg1:lower()

        -- .raid ready [world] [quality]
        if sub == "ready" or sub == "auto" then
            local withWorld = (arg2:lower():find("world") ~= nil or arg2:lower():find("wbuff") ~= nil)
            local quality = arg2:lower():find("best") and "best" or "epic"
            SetupActiveRaid(player, withWorld, quality)
            return false

            -- .raid attune / .raid keys
        elseif sub == "attune" or sub == "attunement" or sub == "keys" then
            AttunePlayerAndRaid(player)
            return false

            -- .raid gear [epic|best]
        elseif sub == "gear" or sub == "prep" then
            local quality = (arg2:lower() == "best") and "best" or "epic"
            PrepareRaidBots(player, quality)
            return false

            -- .raid roles / .raid setup
        elseif sub == "roles" or sub == "setup" then
            ConfigureRaidRoles(player)
            return false

            -- .raid buff / .raid worldbuff
        elseif sub == "buff" then
            ApplyRaidBuffs(player, false)
            return false
        elseif sub == "worldbuff" or sub == "wbuff" then
            ApplyRaidBuffs(player, true)
            return false

            -- .raid summon / .raid rally
        elseif sub == "summon" or sub == "rally" then
            player:RunCommand("bot summon *")
            player:RunCommand("bot p follow")
            player:RunCommand("bot r follow")
            player:RunCommand("bot c * follow")
            player:RunCommand("bot c * stay off")
            player:RunCommand("bot c * flee off")
            SendRaidMsg(player, "All raid bots summoned and follow locked.")
            return false

            -- .raid start / .raid dc (Autonomous Dungeon Clear)
        elseif sub == "start" or sub == "dc" then
            StartDungeonClear(player)
            return false
        elseif sub == "off" then
            player:RunCommand("dc off")
            SendRaidMsg(player, "Autonomous Dungeon Clear disabled.")
            return false
        elseif sub == "pause" or sub == "stop" or sub == "hold" then
            PauseDungeonClear(player)
            return false
        elseif sub == "pull" then
            local mode = (arg2 ~= "") and (" " .. arg2) or ""
            player:RunCommand("dc pull" .. mode)
            return false
        elseif sub == "go" and arg2 ~= "" then
            player:RunCommand("dc go " .. arg2)
            SendRaidMsg(player, "Tank routing directly to boss: " .. arg2)
            return false
        elseif sub == "skip" then
            player:RunCommand("dc skip")
            return false
        elseif sub == "status" then
            player:RunCommand("dc status")
            return false
        elseif sub == "bosses" then
            player:RunCommand("dc bosses")
            return false
        elseif sub == "config" then
            player:RunCommand("dc config")
            return false
        elseif sub == "spectate" then
            local specArg = (arg2 ~= "") and (" " .. arg2) or ""
            player:RunCommand("dc spectate" .. specArg)
            return false

            -- Dungeon Utility Actions (Looting, Drinking, Doors, Focus)
        elseif sub == "loot" then
            player:RunCommand("bot c * loot")
            SendRaidMsg(player, "Bots commanded to vacuum nearby corpse loot.")
            return false
        elseif sub == "drink" or sub == "food" or sub == "rest" then
            player:RunCommand("bot c * drink")
            player:RunCommand("bot c * food")
            SendRaidMsg(player, "Bots commanded to drink and eat to 100%.")
            return false
        elseif sub == "open" or sub == "breach" or sub == "use" then
            player:RunCommand("bot c * u")
            SendRaidMsg(player, "Closest bot ordered to interact with door/lever.")
            return false
        elseif sub == "aoe" then
            player:RunCommand("bot c * co +aoe")
            SendRaidMsg(player, "AoE mode ENABLED on all bots.")
            return false
        elseif sub == "single" then
            player:RunCommand("bot c * co -aoe")
            SendRaidMsg(player, "AoE mode DISABLED. Bots focusing single target.")
            return false
        elseif sub == "focus" or sub == "skull" then
            player:RunCommand("bot c * attack rti 8")
            SendRaidMsg(player, "All DPS bots ordered to burn down Skull target!")
            return false

            -- .raid unbind (Instance unbind for all members)
        elseif sub == "unbind" or sub == "reset" then
            player:RunCommand("instance groupunbind all")
            SendRaidMsg(player, "All instance lockouts cleared for entire group/raid!")
            return false

            -- .raid tele <instance> (Fast Teleport from Volume 03)
        elseif sub == "tele" and arg2 ~= "" then
            local dest = TELEPORT_PRESETS[arg2:lower()] or arg2
            SendRaidMsg(player, "Teleporting group to: " .. dest)
            player:RunCommand("tele group " .. dest)
            return false

            -- .raid help
        else
            SendRaidMsg(player, "|cFFFFD100=== Tortoise-WoW Raid Controller Commands ===|r")
            player:SendBroadcastMessage("|cFF00FFFF.raid ready [world]|r - 1-Click: Gear, roles, attune, keys & buffs")
            player:SendBroadcastMessage("|cFF00FFFF.raid attune|r - Complete raid attunements & spawn master keys")
            player:SendBroadcastMessage("|cFF00FFFF.raid gear [epic|best]|r - Equip bots in Epic or BiS gear")
            player:SendBroadcastMessage("|cFF00FFFF.raid roles|r - Auto-configure Tank/Heal/DPS strategies")
            player:SendBroadcastMessage("|cFF00FFFF.raid buff / worldbuff|r - Apply class raid buffs / world buffs")
            player:SendBroadcastMessage("|cFF00FFFF.raid start / off|r - Start/stop tank clear (.dc on/off)")
            player:SendBroadcastMessage("|cFF00FFFF.raid pause|r - Soft-stop tank clear, disengage & hold position")
            player:SendBroadcastMessage("|cFF00FFFF.raid pull [dynamic|leeroy]|r - Order tank pull or set mode")
            player:SendBroadcastMessage("|cFF00FFFF.raid go <boss>|r - Route tank autopilot directly to boss")
            player:SendBroadcastMessage("|cFF00FFFF.raid skip / status / bosses|r - Skip pack / view progress / bosses")
            player:SendBroadcastMessage("|cFF00FFFF.raid spectate [follow <name>|free]|r - Cinematic spectator camera")
            player:SendBroadcastMessage("|cFF00FFFF.raid loot / rest / open|r - Bot corpse vacuum / drink / open doors")
            player:SendBroadcastMessage("|cFF00FFFF.raid aoe / single / focus|r - Toggle AoE / focus fire Skull mark")
            player:SendBroadcastMessage("|cFF00FFFF.raid summon / unbind|r - Summon bots to you / unbind locks")
            player:SendBroadcastMessage("|cFF00FFFF.raid tele <mc|bwl|zg|aq20|aq40|naxx|ubrs|scarlet|karazhan>|r")
            return false
        end
    end

    return true
end

-- ================================================================================
-- Module 8: Auto-Sync on Instance Entry (MAP_EVENT_ON_PLAYER_ENTER)
-- ================================================================================
local function OnPlayerEnterMap(event, map, player)
    -- Explicitly validate event trigger and player pointer
    if event ~= MAP_EVENT_ON_PLAYER_ENTER or not player or not (player.ToPlayer and player:ToPlayer()) then
        return
    end

    local group = player:GetGroup()
    if group and (map:IsDungeon() or map:IsRaid()) then
        if group:GetLeaderGUID() == player:GetGUID() then
            SendRaidMsg(player, "Entered instance: " .. map:GetName() .. ". Rallying bots...")
            player:RunCommand("bot summon *")
            player:RunCommand("bot p follow")
            player:RunCommand("bot r follow")
            player:RunCommand("bot c * follow")
            ApplyRaidBuffs(player, false)
            SendRaidMsg(player, "Raid gathered and buffed. Type |cFF00FFFF.raid start|r to begin autonomous clear!")
        end
    end
end

-- Register Core Hooks
RegisterPlayerEvent(PLAYER_EVENT_ON_COMMAND, OnRaidCommand)
RegisterServerEvent(MAP_EVENT_ON_PLAYER_ENTER, OnPlayerEnterMap)

PrintInfo(string.format("[%s v%s] Initialized: Raid Automation active. Type .raid for commands.",
    SCRIPT_NAME, SCRIPT_VERSION))
