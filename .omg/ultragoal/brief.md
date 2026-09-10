# Ultragoal Brief: Level 60 Warrior DPS, Dungeons, Raids, Attunements & Custom Content

## Objective
Provide an exhaustive, end-to-end setup and reference suite for a Level 60 **DPS Warrior** (Dual-Wield Fury and Two-Handed Arms / Cleave) running both 5-man dungeons and 10/20/40-man raids (including classic instances and custom Turtle-WoW content). This includes:
1. Warrior DPS combat mechanics (Heroic Strike queue miss bug, Bloodthirst/WW priority, Overpower stance dancing, Execute spam, Death Wish + Diamond Flask snapshotting, threat safety with bot tanks, hit/weapon skill caps).
2. Complete Keyring and Attunement directory (UBRS Seal of Ascension, Scholomance Skeleton Key, Stratholme Key to the City, BRD Shadowforge Key, Dire Maul Crescent Key, Drakefire Amulet, Eternal Quintessence).
3. Raid coordination mechanics (10-man UBRS/Scarlet Citadel/Karazhan, `/ra` vs `/p` channels, bot Main Tank assignment).
4. Turtle-WoW custom dungeons (Hateforge Quarry, Alah'Thalas, Gilneas City, Crescent Grove) and custom raids (Scarlet Citadel 10m, Tower of Karazhan 10m, Timbermaw Hold 20m, Emerald Sanctum 40m).
5. Self-enchanting, consumable stocking, and world buffing (.bot enchants *, arcanums, potions, .buff, .aura).
6. Permanent reference module checked into repository as `references/20_warrior_dps_dungeons_and_raids.md` and linked in `gm_commands.md`.

## Context & Architecture Boundaries
- Server Engine: CMaNGOS + PlayerBots Framework + mod-dungeon-clear + Eluna Lua Engine.
- Client / Protocol: Turtle-WoW 1.18.1 (Build 7272).
- Zero SQL Guardrail: Strictly follow `MEMORY.md` — no direct database modifications. All operations handled via in-game GM commands, playerbot macros, and markdown documentation.
