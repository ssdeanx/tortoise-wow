---
description: High-signal architecture rules, command truths, and LLM concurrency invariants for Turtle-WoW 1.18.1 and Playerbots
globs: ["**"]
alwaysApply: true
---

# Turtle-WoW Core & Playerbot Architecture Rules

## 1. Character Progression & Command Truths
- **`.maxskill` Scope**: In CMaNGOS / Turtle-WoW, `.maxskill` exclusively raises existing weapon proficiencies, defense, and class skills to the character level cap. It intentionally **skips crafting trade skills** (`SKILL_TYPE_PROFESSION`).
- **Profession Maxing**: To train full recipes for a trade skill, use `.learn all_recipes <profession>` (e.g., `.learn all_recipes enchanting`). To set crafting skill levels directly, use `.setskill <id> 300 300` (e.g., Alchemy: 171, Blacksmithing: 164, Enchanting: 333, Engineering: 202, Leatherworking: 165, Tailoring: 197).
- **Secondary Skills**: First Aid (`129`), Cooking (`185`), Fishing (`356`), and Riding (`762`) require `.setskill <id> 300 300` plus explicit trainer spell books (`.learn 18260` Artisan First Aid, `.learn 18265` Artisan Cooking).

## 2. Playerbot Guild Targeting Invariants
- **`guild` Scope Token**: In `PlayerbotHolder.cpp:1193`, `if (charname == "guild" && master)` queries `guild_member` from `CharacterDatabase`. Any `.bot` command accepting `<botname>` or `*` automatically accepts `guild` (e.g. `.bot gear guild best`, `.bot prep guild epic`, `.bot summon guild`).
- **Remote Guild Operations**: Group bots can be managed via whisper: `/w %t guild invite`, `/w %t guild promote`, `/w %t guild demote`, `/w %t guild remove`, `/w %t guild leader`, `/w %t guild leave`.
- **Charter Signatures**: Bots automatically sign guild charters when requested by players via `PetitionSignAction.cpp`.

## 3. LLM Concurrency & Core Preservation Invariants
- **Main World Tick Non-Blocking Rule**: `World::Update(uint32 diff)` is single-threaded. Never invoke synchronous HTTP, TLS handshakes (`SSL_connect`), DNS lookups (`getaddrinfo`), or socket `recv` on the world thread.
- **Zero Raw Pointers Across Threads**: Never pass raw `Player*`, `Unit*`, or `WorldSession*` pointers into asynchronous lambdas or background threads. If a bot despawns or a player logs out while an LLM request is in flight (1–5s), calling `session->QueuePacket()` causes an immediate use-after-free SIGSEGV crash.
- **Durable Identity via `ObjectGuid`**: Background workers must only accept POD (Plain Old Data) structs containing `ObjectGuid`. Response processing must occur on the main world thread, resolving `ObjectGuid` via `sObjectMgr.GetPlayer(guid)` before validating alive/in-world state and dispatching speech.
- **Out-of-Process Sidecar Daemon**: For cloud endpoints like OpenCode Go (`union-alpha`), isolate network transit, HTTP/2 connection pooling, rate limiting, and token budgeting to an external sidecar daemon communicating with the core over a local non-blocking Unix Domain Socket (`/tmp/turtle-llm.sock`).
