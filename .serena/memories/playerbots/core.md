# PlayerBots Subsystem Core

## Architecture & Integration
- Engine source location: `modules/mod-playerbots/`.
- Integrated directly into the MaNGOS world loop via `-DBUILD_PLAYERBOTS=ON`.
- Spawns ~1000 persistent autonomous bots using accounts prefixed with `RNDBOT%` in `tw_logon`.
- Runtime configuration driven by `run/aiplayerbot.conf` and `run/mangosd.conf`:
  - `PlayerBot.Enable = 1`
  - `AiPlayerbot.Enabled = 1`
  - `RandomBotAutologin = 1`
  - `RandomBotLoginAtStartup = 1`

## Equipment Cache Lifecycle (`ai_playerbot_equip_cache`)
- Database table: `tw_char.ai_playerbot_equip_cache`.
- Stores ~1.3M precomputed item-weight permutations.
- Initial cold build takes 12-15 minutes on first server startup.
- Invariant: Never truncate or delete `ai_playerbot_equip_cache` unless explicit gear scoring re-indexing is required. Subsequent launches reuse the cache for immediate world startup.

## Database Tables & Schema Dependencies
- World tables (`tw_world`): Source SQL in `src/modules/PlayerBots/sql/world/` (weightscales, rpg_races, texts, indexes, travel_nodes, named_location, zone_level, weightscale_data). Missing tables cause immediate crash in `RandomItemMgr`.
- Character tables (`tw_char`): Source SQL in `src/modules/PlayerBots/sql/characters/` (ahbot, cache, custom_strategy, db_store, names, random_bots).

## Navigation, Travel & Diagnostics
- Bot movement and travel planning are managed by `TravelNode` graphs combining DBC taxi paths with Recast/Detour navmesh paths.
- Event logs output to `logs/` relative to working directory (`/home/sam/tortoise-wow/logs/` or `run/logs/`).
- Automated log rotation is handled by `turtle-logrotate.timer`.
