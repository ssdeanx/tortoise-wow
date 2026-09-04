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

| Upstream (Lua 5.1 / 3.3.5)              | Here (Lua 5.0 / 1.12)                                    |
|-----------------------------------------|----------------------------------------------------------|
| `a % b`                                 | `math.mod(a, b)`                                         |
| `#tbl`                                  | `table.getn(tbl)`                                        |
| `string.gmatch`                         | `string.gfind`                                           |
| `str:match(pat)`                        | `string.find(str, pat)` with captures                    |
| handlers `function(self, event, ...)`   | `function()` reading `this`, `event`, `arg1..`           |
| named handler `OnUpdateHandler(self, elap)` | same rule - the first parameter is always the frame  |
| `SendAddonMessage(..., "WHISPER", me)`  | removed - 1.12 knows PARTY/RAID/GUILD/BG only            |
| `frame:SetSize(w, h)` (3.0)             | `SetWidth(w); SetHeight(h)` - replaced in the text       |
| `SetNormalFontObject` (2.x)             | `SetTextFontObject`                                      |
| `SetWordWrap`, `GetStringHeight`        | guarded / fall back to `GetHeight`                       |
| `SetPoint("TOPLEFT")`                   | the five-argument form; 1.12 accepts no other            |
| `UIDropDownMenu_SetWidth(frame, w)`     | `(w, frame)` - and `SetText` is `(text, frame)`          |
| dropdown init/click callbacks `(self, level)` | no arguments; `this`, `UIDROPDOWNMENU_MENU_LEVEL`  |
| `RegisterEvent("GROUP_ROSTER_UPDATE")`  | `PARTY_MEMBERS_CHANGED` + `RAID_ROSTER_UPDATE`           |
| `local inInstance, instanceType = IsInInstance()` | one return value on 1.12; type assumed "party" |
| `InterfaceOptions_AddCategory` (2.4)    | guarded; settings stay reachable via `/dc`               |
| `strtrim`                               | fallback defined if the client lacks it                  |

In 1.12 a `SetScript` handler receives **no arguments**: the frame is the global
`this`, the event name is `event`, event payload is `arg1`..`arg9`. Every
handler - inline or named - was rewritten to read those; their bodies are
untouched.

One lesson cost a whole evening and is worth stating on its own: **do not
create throwaway frames to probe widget metatables.** An earlier build did, to
add `SetSize` and friends to the shared metatable the way pfUI does. It did not
take on this client, and one of the probes was an `EditBox` - which on 1.12
auto-focuses by default and swallowed every keystroke: no movement, no chat,
for as long as the addon was loaded. The missing methods are replaced in the
text instead.

## Verification

Live on a Turtle WoW 1.18 client, 2026-09-04, in Razorfen Kraul with an LFT
party: `/dc on`, status ON, pull mode Dynamic, live state ("Clearing Path
(Trash) - Fighting Razorfen Handler"), next boss, and the four-boss list with
Alive/Go controls, all pushed by the server and rendered. That is the complete
round trip - addon message, `DungeonClearAddonHook`, tank bot, status push,
panel. Eight client-side iterations got here; each error the client reported
was a 2.x convenience at a single site (the table above), and the parser check
under a real Lua 5.0.3 (`luac -p`) caught none of them - it only sees syntax.

Not yet exercised: the Spectate camera buttons and the Settings panel's
sliders and dropdowns beyond their creation.

`port-1.12.diff` next to this file is the complete change against upstream v3.6.
