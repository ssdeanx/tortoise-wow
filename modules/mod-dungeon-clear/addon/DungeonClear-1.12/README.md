# DungeonClear addon — 1.12 / Turtle WoW build

A port of [jrad7/mod-dungeon-clear-addon](https://github.com/jrad7/mod-dungeon-clear-addon)
(v3.6, AGPL-3.0-or-later, Copyright Jared Wright and contributors) to the
**1.12 client** that Turtle WoW ships. The upstream addon declares
`## Interface: 30300` and is written for WotLK's Lua 5.1; a 1.12 client runs
Lua 5.0 and refuses the file outright:

    DungeonClear.lua:878: unexpected symbol near '%'

That is the modulo operator, which Lua 5.0 does not have — and it is only the
first of several differences. This folder carries the same addon with every one
of them resolved. The server side (`DungeonClearAddonHook.cpp`, prefix `DC`)
is unchanged; both variants speak the same protocol.

## Install

Copy the folder `DungeonClear-1.12` into `Interface\AddOns\` of the client and
rename it to `DungeonClear`. Then `/dc` in game, or use the panel.

## What was changed, and why

| Upstream (Lua 5.1 / 3.3.5)              | Here (Lua 5.0 / 1.12)                          |
|-----------------------------------------|------------------------------------------------|
| `a % b`                                 | `math.mod(a, b)`                               |
| `#tbl`                                  | `table.getn(tbl)`                              |
| `string.gmatch`                         | `string.gfind`                                 |
| `str:match(pat)`                        | `string.find(str, pat)` with captures          |
| handlers `function(self, event, ...)`   | `function()` reading `this`, `event`, `arg1..` |
| `SendAddonMessage(..., "WHISPER", me)`  | removed — 1.12 knows PARTY/RAID/GUILD/BG only  |
| `frame:SetSize(w, h)` (3.0)             | shimmed onto `SetWidth`/`SetHeight`            |
| `SetNormalFontObject` (2.x)             | shimmed onto `SetTextFontObject`               |
| `InterfaceOptions_AddCategory` (2.4)    | guarded; settings stay reachable via `/dc`     |
| `strtrim`                               | fallback defined if the client lacks it        |

In 1.12 a `SetScript` handler receives **no arguments**: the frame is the global
`this`, the event name is `event`, event payload is `arg1`..`arg9`. All 28
handlers were rewritten to read those; their bodies are untouched.

The widget shims follow the pattern pfUI uses on 1.12: every object of a kind
shares one metatable, so a missing method added to `getmetatable(obj).__index`
appears on all of them.

## Verification

The file parses cleanly under a real Lua 5.0.3 (`luac -p`), which reproduces the
upstream error word for word on the original. **It has not yet been exercised
in a live 1.12 client** — runtime behaviour (camera helpers, slider callbacks,
tooltips) needs a Turtle client to confirm. Report what breaks.

`port-1.12.diff` next to this file is the complete change against upstream v3.6.
