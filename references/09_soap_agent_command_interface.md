---
name: SOAP Agent Command Interface
description: Agent-facing operating guide for the mangosd SOAP remote-command interface on loopback — how to build and configure it, the exact request envelope, authentication traps, response and fault handling, and a safety policy for running commands against a live world.
version: 1.0.0
platform: Turtle-WoW 1.18.1 (Build 7272)
stack: CMaNGOS + gSOAP + mangosd
glob:
  - src/mangosd/MaNGOSsoap.cpp
  - src/mangosd/MaNGOSsoap.h
  - src/mangosd/CMakeLists.txt
  - src/mangosd/mangosd.conf.dist.in
  - CMakeLists.txt
  - run/mangosd.conf
tags:
  - soap
  - gm-commands
  - remote-control
  - agent-tools
  - loopback
  - gsoap
  - automation
  - world-thread
---

# 09. SOAP Agent Command Interface

**Audience: AI agents.** This is the one channel an agent can use to run a
server command and read its output back. It is a *command* channel, not a
*query* API — that distinction shapes everything below.

---

## 1. What SOAP Actually Is

```lua
-- gSOAP Operation Specification (MaNGOSsoap.cpp)
-- Operation:  ns1__executeCommand
-- Namespace:  urn:MaNGOS
-- Input:      one <command> string
-- Output:     one <result> string
```

The server enqueues a `CliCommandHolder` onto the **world thread**, blocks
until that thread signals completion, and returns whatever the command printed
(`MaNGOSsoap.cpp:178-214`). There is no parallelism: the world thread executes
commands one at a time, and this request waits its turn.

**Consequences that matter:**

- It runs with **`SEC_CONSOLE`** authority — the same level as typing at the
  server console (`MaNGOSsoap.cpp:196`). Not player, not GM: console.
- **One command per request.** There is no batch form.
- Commands execute **against the live world**. There is no dry-run, no
  read-only mode, no transaction. An agent that issues a state-changing
  command has changed the world.
- If the world stops mid-command, you get a `Server is shutting down` fault
  and cannot tell whether the command ran (`MaNGOSsoap.cpp:199-206`).

For **read-only data** (spells, spawns, account state, inventory), query the
database directly — `tw_world` / `tw_char`. SOAP is the right tool for
*commands*, and the wrong tool for *inspection*.

---

## 2. Setup

### Build

SOAP is **off by default**. `ENABLE_SOAP` is declared at `CMakeLists.txt:71`
with default `OFF`, before the legacy platform-specific declaration, so the
effective default is OFF regardless of platform:

```bash
cmake -DENABLE_SOAP=ON <your other flags>
```

Confirm it reached the binary:

```bash
strings build/src/mangosd/mangosd | grep -c 'urn:MaNGOS'   # 1 = compiled in
```

### Configure

`run/mangosd.conf` (the live deployment config):

```ini
SOAP.Enabled = 1
SOAP.IP = 127.0.0.1
SOAP.Port = 7878
```

The template defaults are documented at `src/mangosd/mangosd.conf.dist.in:2434-2444`
(`SOAP.Enabled = 0` there). `run/mangosd.conf` is gitignored; the template is tracked.

### Verify it is listening

```bash
ss -ltnp | grep 7878
```

---

## 3. The Request Shape

The command text goes **in a `<command>` element**, not in the method name.
Putting the command where the method name belongs produces
`Method 'ns1:server' not implemented` — the server reads the local name as a
gSOAP operation name.

```xml
<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <ns1:executeCommand xmlns:ns1="urn:MaNGOS">
      <command>server info</command>
    </ns1:executeCommand>
  </soap:Body>
</soap:Envelope>
```

POSTed to `http://127.0.0.1:7878/` with HTTP Basic credentials.

### Copy-paste helper

```bash
soap() {
  curl -s --max-time 30 -u "${SOAP_USER:-SOAP}:${SOAP_PASS:-123}" \
    -H 'Content-Type: text/xml; charset=utf-8' \
    --data-binary @- "http://${SOAP_HOST:-127.0.0.1}:7878/" <<XML | \
    python3 -c '
import sys,re,html
t=sys.stdin.read()
m=re.search(r"<result>(.*?)</result>",t,re.S)
f=re.search(r"<faultstring>(.*?)</faultstring>",t,re.S)
if m: print(html.unescape(m.group(1)).strip())
elif f: print("FAULT:",html.unescape(f.group(1)).strip())
else: print(t.strip()[:600])
'
<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <ns1:executeCommand xmlns:ns1="urn:MaNGOS"><command>${1}</command></ns1:executeCommand>
  </soap:Body>
</soap:Envelope>
XML
}
```

```bash
soap "server info"
soap "rndbot stats"
```

Set `SOAP_USER` / `SOAP_PASS` / `SOAP_HOST` in the environment rather than
relying on the defaults above.

---

## 4. Reading the Response

```lua
-- SOAP HTTP Outcomes & Response Envelopes

-- HTTP 200: Success - <result> holds captured output (Verified)
-- HTTP 200: Command Refused - Refusal message returned verbatim inside <result> (Verified)
-- HTTP 500: Command Name Not Recognised - <Fault> string: "There is no such command" (Verified)
-- HTTP 500: Empty Command - <Fault> string: "Command must not be empty" (Verified)
-- HTTP 401 / 403: Bad / Banned / Low-Rank Credentials - Gated before reading body (Source verified)
-- HTTP 413: Content-Length over 64 KiB - Connection refused early (Source verified)
-- Disconnect (—): Chunked or undeclared body over 64 KiB - Body truncated, connection closed (Source verified)
```

> ⚠️ **A refusal is a 200, not an error.** Verified: `dc status` and `dc bosses`
> both return HTTP 200 with `dc needs a session; only dc test runs from the
> console.` inside `<result>`. A fault is raised only when the handler itself
> fails — an unknown command name or an empty command. So **HTTP 200 does not
> mean the command did anything useful.** Read the text; do not trust the
> status code.

Two more measured behaviours worth knowing:

- **An empty `<result>` can still mean it worked.** `account characters SOAP`
  returns `200` with an empty `<result>` — output went to the chat log, not the
  command's print callback. The output is whatever the command sent to
  `SoapPrint` (`MaNGOSsoap.cpp:155`), and some commands chat-log instead. An
  empty result is ambiguous: it may mean "no rows" or "output went elsewhere".
- Multi-line output is normal.

---

## 5. Authentication

`ParseAndAuthenticate` (`MaNGOSsoap.h:32`) requires a **not-banned** account at
**`SEC_ADMINISTRATOR`** (rank 4 or above) and runs before the body is read.

### The two-hash trap

This server has **two unrelated password paths**:

```lua
-- Authentication Columns & Consumers

-- Realmd game login: Uses columns `v` + `s` (SRP6 verifier/salt) for logging into WoW client
-- mangosd SOAP:      Uses column `sha_pass_hash` via AccountMgr::CheckPassword for SOAP interface
```

Realmd **never reads** `sha_pass_hash`. There is no single stored hash that
satisfies both. Writing the SOAP column does not change the game password, and
regenerating SRP fields does not change the SOAP password.

### The case-normalisation trap

`AccountMgr::CheckPassword` uppercases the **password** but preserves the
**stored username** casing. Realmd's `_safelogin` preserves the submitted
username. So for an account stored as `sam`:

```lua
-- Password Hash Case Differences (Account stored as 'sam')

-- SOAP:   SHA1("sam:" + password)  -- Preserves stored username casing
-- Realmd: SHA1("SAM:" + password)  -- Uppercases submitted username
```

**No hash satisfies both.** Consequences:

- **Prefer an all-uppercase account** (e.g. `SOAP`). Uppercase-only names
  authenticate identically on both paths, which is why the dedicated SOAP
  account exists.
- **Never use `.account set password` on a shared account.** The in-game path
  passes the session username and uppercases it; the SOAP path passes an empty
  username and keeps the stored casing. That command can silently desynchronise
  the two and lock you out of the client.

### Provisioning a dedicated account

```sql
-- rank 5 = SEC_SIGMACHAD, satisfies the SEC_ADMINISTRATOR minimum
INSERT INTO account (username, sha_pass_hash, gmlevel, locked, active)
VALUES ('SOAP', SHA1('SOAP:123'), 5, 0, 1);
```

The account needs **no** game login, so its `v` / `s` SRP fields can stay unset.

### Failed-login autoban

Repeated 401s trip an **IP** ban, not an account lock. Realmd's
`WrongPass.MaxCount` / `WrongPass.BanTime` in `run/realmd.conf` govern it, and
on loopback the banned address is `127.0.0.1` — which blocks every local client,
not just SOAP. Wait out the timer rather than deleting the row.

---

## 6. Safety Policy for Agents

SOAP has **`SEC_CONSOLE`** authority and no dry-run. An agent should:

- **Run freely** — read-only and status commands: `server info`, `uptime`,
  `rndbot stats`, `dc status`, `dc bosses`, `account characters <name>`,
  `who`, `instance stats`.
- **Confirm with the human first** — anything that writes: `.ban`, `.unban`,
  `.kick`, `.die`, `.levelup`, `.item add`, `.account set *`, `.tele`,
  `.shutdown`, `.instance unbind`, `.dc on`, and anything that spawns, despawns
  or teleports.
- **Never** run `shutdown` or `restart` to "clean up" a stuck request.

Prefer read-only: a mis-typed console command is not rejected by a confirmation
prompt, and there is no undo.

Known limits worth knowing before planning work: several useful diagnostics
**require an in-game session and refuse over SOAP** — `dc status` and
`dc bosses` both answer *"dc needs a session; only dc test runs from the
console"*. Session-bound debugging must go through the client or the GM
harness, not this channel.

---

## 7. Limits

```lua
-- Server SOAP Operating Limits and Configuration

-- Max request:          64 KiB (whole HTTP message) [MaNGOSsoap.h:37]
-- Commands per request: 1
-- Accept timeout:       3 s [MaNGOSsoap.h:35]
-- Data timeout:         5 s [MaNGOSsoap.h:36]
-- Command backlog:      100 [MaNGOSsoap.h:36]
-- Min rank:             SEC_ADMINISTRATOR (rank 4) [MaNGOSsoap.h:48]
```

The 64 KiB cap is generous — no real command approaches it. The practical
ceiling is the **5 s data timeout**: a long-running command can time out the
socket even though the world thread is still working on it. On timeout, do not
assume the command failed.

---

## 8. Troubleshooting

```lua
-- Troubleshooting SOAP Symptoms & Root Causes

-- Symptom: Method 'ns1:server' not implemented
-- Cause:   Command text was placed in the method name instead of inside <command> tags.

-- Symptom: 401 on known-good credentials
-- Cause:   sha_pass_hash out of sync, or autoban triggered on 127.0.0.1.

-- Symptom: 413 Payload Too Large
-- Cause:   Declared request body exceeds 64 KiB.

-- Symptom: Connection closes mid-response
-- Cause:   Chunked or undeclared body exceeds 64 KiB.

-- Symptom: Fault: ... Server is shutting down
-- Cause:   World thread stopped mid-command; command may or may not have run.

-- Symptom: Empty <result> on command expecting output
-- Cause:   Output was written to chat log instead of command print callback (SoapPrint).

-- Symptom: HTTP 200 but text is a refusal
-- Cause:   Command refusals are returned with HTTP 200. Check body text rather than status code.

-- Symptom: There is no such command
-- Cause:   Command name is not registered in core command table; session-bound commands refuse instead.

-- Symptom: Connection refused
-- Cause:   SOAP.Enabled is not set to 1 in mangosd.conf, or binary was built without -DENABLE_SOAP=ON.
```

---

## 9. Security Notes

- **Credentials are plaintext in this file**, and `references/` is tracked by
  git. Committing this document puts the SOAP account in repository history.
  Rotate the password, or move it to an untracked local file and read it at
  runtime, before committing.
- **HTTP Basic with no transport encryption.** Safe on loopback; exposing this
  beyond `127.0.0.1` sends the GM password in clear text and hands an attacker
  console-equivalent authority. If remote access is ever needed, tunnel it.
- `SOAP.IP = 127.0.0.1` is the safe default. Change it deliberately, never
  casually.
