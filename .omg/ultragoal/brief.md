# Ultragoal Brief: Autonomous Group & Raid Controller for Tortoise-WoW (5, 10, 20, 40-Man)

## Objective
Build a unified, end-to-end automation engine (via an Eluna Lua script and command suite) that eliminates the manual grind of typing dozens of macros and whispering individual bots when forming groups and raids (5, 10, 20, and 40-man). The system must:
1. **Leverage the 5-Volume Master Reference Suite** (`references/01`–`05` and `legacy_22_modules`):
   - Volume 01: Character progression, instant boost, and instance unbinding.
   - Volume 02 & Legacy 21: Full playerbot control (`.bot`), chat filter targeting tokens (`/p @tank`, `/p @heal`, etc.), strategy tokens (`co`/`nc`), whisper shortcuts, and 10/20/40-man raid compositions.
   - Volume 03: Fast group teleports (`.tele group <instance>`), keys, and attunements.
   - Volume 04: Class specs, BiS gear loadouts, tier sets, enchants, and verified 1.18.1 talent builds.
   - Volume 05: Eluna Lua engine event hooks (`RegisterPlayerEvent(42, ...)` for custom commands, `RegisterGroupEvent`, `MAP_EVENT_ON_PLAYER_ENTER`, `player:RunCommand`).
2. **Support LFT (Looking For Turtle) & Custom Group/Raid Formation**:
   - Work seamlessly whether a group is formed via the native group finder (`LFTBotFill`) or assembled on-demand.
   - Support dynamic scaling: 5-man dungeon, 10-man (UBRS, Karazhan Crypts), 20-man (ZG, AQ20), and 40-man (MC, BWL, AQ40, Naxx).
3. **Automate the Tedious Configuration**:
   - **Automatic Role & Spec Detection**: Detect bot class/spec and assign proper combat roles (`tank`, `heal`, `melee`, `ranged`), combat strategies (`co`), and non-combat behaviors (`nc`).
   - **1-Click Gearing & Preparation**: Run `.bot gear * best`, `.bot enchants *`, `.bot prep *`, potions, food, and class reagents across the entire raid.
   - **Instant Full Raid Buffing**: Apply full rank raid buffs (Fortitude, Shadow Protection, Arcane Brilliance, Gift of the Wild, Greater Blessings, Paladin Auras) to all 40 members without waiting 10 minutes for mana.
   - **Autonomous Dungeon Clear (`.dc`) Integration**: Automatically designate the Main Tank bot, toggle `.dc on`, set pull mode (`.dc pull dynamic`), and coordinate followers when entering an instance.
4. **Intuitive User Interface**:
   - Provide clean custom commands (`.raid 10`, `.raid 20`, `.raid 40`, `.raid buff`, `.raid gear`, `.raid start`, `.raid pause`, `.raid reset`).
   - Optional interactive Gossip Menu / item (Raid Commander Stone) so the player can control the entire raid with single clicks.

## Sequential Execution Pipeline
1. **Goal 1 (Active)**: Audit and Synthesize Master References for Raid Automation Commands.
2. **Goal 2**: Develop the Core Eluna Raid Automation Script (`lua_scripts/raid_automation.lua`).
3. **Goal 3**: Implement Autonomous Dungeon Clear (`.dc`) & Instance Entry Triggers.
4. **Goal 4**: Implement the In-Game Command Deck (`.raid`) & Gossip UI Controller.
5. **Goal 5**: Validate Syntax, Verify Server Configuration & Document Usage.

## Boundaries & Constraints
- Platform: Turtle-WoW 1.18.1 (Build 7272) on CMaNGOS with `mod-playerbots`, `mod-dungeon-clear`, and `Eluna`.
- Script Location: `lua_scripts/` (executed live via `.reload eluna`).
- Zero manual bot micromanagement: All bot targeting and role tuning must be fully automated across all group members.
