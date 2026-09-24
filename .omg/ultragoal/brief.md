# Ultragoal Brief: Playerbot Group Sizing, Raid Character Assembly & Account Limit Architecture

## 1. Executive Summary & Problem Statement
The user has inquired about three interrelated Playerbot subsystems:
1. **Behavioral Divergence in `.bot group size=5`**:
   - How does `.bot group size=5` work now compared to origin / commit `f776c7aa399820600275ea1012611a8ec3bcf1be`?
   - How can we make it work like it used to, and what role does `run/aiplayerbot.conf` play?
2. **Raids & Using Own Characters**:
   - How raid assembly works with bots.
   - How players can use their own characters in 10-man, 20-man, and 40-man raids.
   - Why the old way only made 10 characters and why the player's account is now reporting "Account has max characters".
3. **Character Limits per Account**:
   - Feasibility, risks, and client/server constraints of increasing max characters per account beyond 10 (`CharactersPerRealm`, `SMSG_CHAR_ENUM`, `CharacterSelect.lua`).
   - Definitive alternative solutions (Multi-Account Alt Bots via `AiPlayerbot.AllowMultiAccountAltBots`).

## 2. Core Technical Findings
- **Commit `f776c7aa399820600275ea1012611a8ec3bcf1be` / Origin**:
  - `PlayerbotHolder::HandleGroup` looped `HandleCreate(master, paramStr, security)`.
  - `CreateBot` delegated account selection to `PlayerbotHolder::GetOrCreateAccount`, returning `master->GetSession()->GetAccountId()`.
  - Characters were created directly on the player's account.
  - Guardrail: `if (sAccountMgr.GetCharactersCount(accountId) >= maxCharsPerAccount)` (where `maxCharsPerAccount = 9` in `MANGOSBOT_ZERO`).
  - Once 10 characters existed on the account (1 player + 9 bots), any further creation failed with `"Account has max characters"`, blocking raid creation beyond 10 and filling the account.
- **Commit `77e69376` (Current HEAD)**:
  - `HandleGroup` was rewritten to a 2-phase recruitment system:
    - Phase 1: Hijacks active online world randombots (`sRandomPlayerbotMgr.GetPlayers()`) matching role and level criteria.
    - Phase 2: Spawns shortfall bots via `sRandomPlayerbotMgr.HandleCreate(..., login=1 temporary=1)` into the `RNDBOT` pool (`rndbot0`..`rndbot500`), avoiding personal account pollution.
  - However, players who expected clean, level-scaled, dedicated party bots (or alt bots) find world bots kidnapped or behavior altered with no configuration toggle in `run/aiplayerbot.conf`.
- **Character Limit Constraints**:
  - `World.cpp:1133` enforces `setConfigMinMax(CONFIG_UINT32_CHARACTERS_PER_REALM, "CharactersPerRealm", 10, 1, 10)`.
  - The Vanilla 1.12 / 1.18 client character select screen contains exactly 10 button frames (`CharacterSelectCharacterButton1..10`). Sending >10 in `SMSG_CHAR_ENUM` breaks client glue UI without client mods.
  - Native architecture solution: `AiPlayerbot.AllowMultiAccountAltBots = 1` lets players create separate alt accounts (`sam1`, `sam2`) and summon whole accounts (`.bot add <account>`) or specific alts into raids without limit.

## 3. Scope of Work & Constraints
- Adhere strictly to `RULE[/home/sam/tortoise-wow/AGENTS.md]`:
  - Preserve core native contracts, generic behaviors, and Turtle compatibility.
  - Do not run unauthorized server start/stop or database operations outside SOP.
  - Provide a clean, configurable implementation allowing users to choose how `.bot group` behaves (recruit online vs generate fresh bots on rndbot pool vs alt bots).
