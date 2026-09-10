-- Make the guild vault keepers open the guild bank.
-- The bank UI lives client-side and triggers on the NPC greeting text being
-- exactly GUILD_BANK_TRIGGER - not on the NPC name, which is why renaming the
-- NPCs has no effect. The keepers shipped with gossip_menu_id = 0 and so only
-- ever sent the default greeting.
-- Pair with GuildBank.NpcEntriesAlliance/Horde in mangosd.conf: without those
-- the window opens but the server drops every action for lack of a keeper
-- in range.

-- Guild bank: make right-clicking the vault keepers open the bank window.
--
-- Apply to the WORLD database. Requires a server restart (see note below).
--
-- Symptom
--   Right-clicking "Teller Plushner" (Alliance, entry 80917) or "Are" (Horde,
--   entry 80918) opens a plain gossip window ("Greetings <name>") and nothing
--   else. The guild bank never opens.
--
-- Cause
--   The guild bank UI lives client-side in Turtle_GuildBankUI (patch-8/9). It
--   listens on GOSSIP_SHOW and checks:
--
--       local TRIGGER = "GUILD_BANK_TRIGGER"
--       ...
--       if GetGossipText() == TRIGGER and UnitExists("npc") then
--
--   So the trigger is the NPC's GREETING TEXT, not its name. Older client
--   versions matched on the name via GUILD_BANK_NPC_TITLE; that global is gone
--   in patch-9, which is why renaming the NPCs has no effect. Server-side the
--   two NPCs ship with gossip_menu_id = 0, so they only ever send the default
--   greeting and the trigger never matches.
--
--   Players never see the string: the addon sets the gossip frame to alpha 0
--   and hides it in the same handler.
--
-- Notes
--   * No gossip_menu_option row is needed. For creatures, SendPreparedGossip
--     always ends in SendGossipMenu(textId, guid) — the early return for empty
--     menus only applies to gameobjects. Verified in game.
--   * broadcast_text has no reload command, so a full server restart is
--     required. gossip_menu, npc_text and creature_template would otherwise
--     reload fine.
--   * IDs 6320603 and 65000 were free on this server. Change them if they
--     collide with yours.
--   * Entries 80917/80918 are Turtle-specific. The server also requires the
--     player to stand within INTERACTION_DISTANCE of them for any guild bank
--     action to be accepted.

-- 1. The trigger string itself
REPLACE INTO `broadcast_text`
    (`entry`, `male_text`, `female_text`, `chat_type`, `sound_id`, `language_id`)
VALUES
    (6320603, 'GUILD_BANK_TRIGGER', 'GUILD_BANK_TRIGGER', 0, 0, 0);

-- 2. npc_text pointing at it
REPLACE INTO `npc_text` (`ID`, `BroadcastTextID0`, `Probability0`)
VALUES (6320603, 6320603, 1);

-- 3. A gossip menu carrying that text
REPLACE INTO `gossip_menu` (`entry`, `text_id`, `script_id`, `condition_id`)
VALUES (65000, 6320603, 0, 0);

-- 4. Hand it to the vault keepers.
--    80917/80918 are the two that always worked (Stormwind, Orgrimmar).
--    62008-62012 are the decorative ones Turtle placed next to the banks in
--    Darnassus, the gnome bank, Ironforge, Thunder Bluff and Undercity. They
--    additionally need to be listed in GuildBank.NpcEntries* (patch 0008),
--    otherwise the window opens but every action is dropped by the server's
--    proximity check.
UPDATE `creature_template` SET `gossip_menu_id` = 65000
WHERE `entry` IN (80917, 80918, 62008, 62009, 62010, 62011, 62012);
