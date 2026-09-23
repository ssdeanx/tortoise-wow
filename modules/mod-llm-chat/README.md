# mod-llm-chat: High-Performance, Non-Blocking Generative Dialogue for Playerbots

> **Architecture**: Asynchronous Worker Mailbox Pattern with Dialogue Level of Detail (LOD)  
> **Supported Backends**: OpenCode Go (`union-alpha`, `glm-5.3-flash`, `muse-spark-1.3-contributor`), OpenAI Chat Completions, Local Ollama / Llama.cpp  
> **Platform**: Turtle WoW 1.18.1 / CMaNGOS with Playerbots  

---

## 1. Overview

`mod-llm-chat` is a high-performance, non-blocking C++ module providing intelligent, roleplay-accurate generative dialogue for playerbots in World of Warcraft.

### Key Architectural Pillars

* **100% Non-Blocking World Tick**: All network, HTTP handshakes, and LLM inference calls execute on a detached background worker thread via `libcurl`. The main game loop (`World::Update`) only interacts with thread-safe request/response queues, adding under 1 microsecond of overhead.
* **Zero-Crash Pointer Safety**: Never passes raw `Player*` or `WorldSession*` pointers across thread boundaries. Requests and responses use stable `ObjectGuid` identifiers and are validated on the main thread before dispatching chat packets.
* **Protocol Auto-Routing**: Automatically adapts the wire protocol, HTTP headers, and JSON structure based on the configured model:
  * **`union-alpha`** (Free): Anthropic Messages API (`/v1/messages`) with `x-api-key`, `anthropic-version: 2023-06-01`, and `max_tokens: 400` (reasoning-capable).
  * **`glm-5.3-flash`** (Cheap): OpenAI Chat Completions (`/v1/chat/completions`) with Bearer auth (~1.8s ultra-fast response).
  * **`muse-spark-1.3-contributor`**: OpenAI Responses API (`/v1/responses`) with `instructions` and `input`.
  * **Local Ollama / OpenAI**: Standard Chat Completions.
* **Automatic Key Detection**: If `ModLlmChat.ApiKey` is left blank, the module automatically reads the active OpenCode Go key from `~/.local/share/opencode/auth.json`.
* **Dialogue Level of Detail (LOD)**: Bots will **never** generate ambient chat into the void if no real humans are around:
  * **Party / Raid**: Bots only speak if at least one real human player is in the group.
  * **Guild**: Bots only chat if at least one real human guild member is currently online.
  * **Say (Taverns & Towns)**: Bots only speak if a human player is within 30 yards.
* **Paced Bot-to-Bot Interactions**: Bots can reply to each other in ambient channels up to a configurable maximum chain depth (default = 2), followed by a 45-second channel quiet cooldown to prevent infinite chat loops.
* **Whisper Immersion**: When a player whispers a bot, the bot immediately performs a `/ponder` or `/nod` emote while reasoning in the background, followed by the whisper reply.

---

## 2. Configuration

Copy `conf/mod-llm-chat.conf.dist` to `mod-llm-chat.conf` in your server runtime directory:

```ini
[ModLlmChat]
# Enable or disable the module (1 = On, 0 = Off)
ModLlmChat.Enable = 1

# API Provider Endpoint
ModLlmChat.ApiEndpoint = "https://opencode.ai/zen/go/v1"

# API Key (Leave empty to auto-load from ~/.local/share/opencode/auth.json)
ModLlmChat.ApiKey = ""

# Model identifier (Default: union-alpha for 100% free operation)
ModLlmChat.Model = "union-alpha"
ModLlmChat.MaxTokens = 400
ModLlmChat.Temperature = 0.7

# Channel Response Rates (0 to 100%)
ModLlmChat.WhisperRate = 100       # 100% reply to direct player whispers
ModLlmChat.PartyRate = 20          # 20% chance per bot (~59% group reply chance in 5-man)
ModLlmChat.GuildRate = 15          # 15% chance for a guild bot to chime in
ModLlmChat.SayRate = 15            # 15% chance in /say (only if human within 30y)
ModLlmChat.RaidRate = 5            # 5% chance in /raid

# Dialogue LOD - Human Presence Enforcement
ModLlmChat.RequireHumanPresent = 1
ModLlmChat.HumanPresenceRadius = 30.0

# Bot-to-Bot Conversation Control
ModLlmChat.BotToBotEnabled = 1
ModLlmChat.MaxConversationDepth = 2
ModLlmChat.ConversationCooldown = 45

# Immersion & Anti-Spam
ModLlmChat.EmoteOnWhisper = 1
ModLlmChat.WhisperCooldown = 10
ModLlmChat.Debug = 0
```

---

## 3. Building

To compile with `mod-llm-chat` enabled:

```sh
cmake -S . -B build -DMODULES=static
cmake --build build -j$(nproc)
```

To selectively enable or disable the module:

```sh
cmake -S . -B build -DMODULE_MOD_LLM_CHAT=static
# or
cmake -S . -B build -DMODULE_MOD_LLM_CHAT=disabled
```
