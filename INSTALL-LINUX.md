# Installing on Linux

Written from a running install rather than from memory. Every version below is
one this tree is actually built and served on:

| | |
|---|---|
| Debian 13 (trixie) | gcc 14.2, CMake 3.31 |
| MariaDB 11.8 | ACE 8.0.2, Boost 1.83 |

Ubuntu 22.04 or newer works the same way; the package names are identical.

**What is in the repository:** the server source and the full world database
(131 MB under `sql/base`, 186 files).

**What is not:** the client data. Maps, DBC and vmaps come out of a game client
— see step 4. You need a **Turtle WoW 1.18.1 client, build 7272**.

---

## 1. Packages

```bash
sudo apt install build-essential cmake git libace-dev libboost-all-dev default-libmysqlclient-dev libssl-dev zlib1g-dev libbz2-dev mariadb-server
```

ACE has to be 7.x or newer. The tree is built as C++17, which removed dynamic
exception specifications, and ACE 6.x still uses them — its headers bury
`WorldSocketMgr.cpp` in errors. Debian 13 and Ubuntu 24.04 ship ACE 8.

## 2. Configure and build

```bash
git clone --recursive -b playerbots-integration-gh https://github.com/Shyalya/tortoise-wow.git
cd tortoise-wow
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/turtle -DBUILD_PLAYERBOTS=ON -DUSE_EXTRACTORS=ON
cmake --build build -j$(nproc)
```

**`BUILD_PLAYERBOTS` defaults to `OFF`.** Leave it out and you get a server with
no bots at all, with no warning anywhere — the module simply is not compiled in.

**Use `Release`.** A `Debug` build of this tree produces a `mangosd` binary well
over half a gigabyte and runs noticeably slower.

`ALLOW_TURTLE_ADDONS` is on by default and has to stay on, or the client crashes
with *"interface corrupt"* on entering the world.

### The install prefix is compiled in

Set `CMAKE_INSTALL_PREFIX` now and do not change it later. The path to
`aiplayerbot.conf` is baked into the binary at compile time:

```cpp
inline std::string _D_AIPLAYERBOT_CONFIG = SYSCONFDIR "aiplayerbot.conf";
```

`SYSCONFDIR` comes from `CMAKE_INSTALL_PREFIX/etc/`. Move the installation
afterwards and the server starts anyway, logs one line about not being able to
open the file, and runs with no bots. Changing the prefix means rebuilding.

## 3. Install

```bash
cmake --install build
```

Binaries land in `<prefix>/bin`, configs in `<prefix>/etc`.

## 4. Extract the client data

Run the tools from `build/` inside your **client** directory:

1. `mapextractor` — produces `dbc` and `maps`
2. `vmapextractor`, then `vmap_assembler` — produces `vmaps`
3. `MoveMapGen` — produces `mmaps`, and takes an hour or more

Warnings of the form `Can't find area flag for areaid ...` are normal: a few ADT
cells reference area ids that are not in `AreaTable.dbc`. Those cells end up
without an area flag, which only matters if a player stands exactly there.

Point `DataDir` in `mangosd.conf` at wherever you put the four directories.

## 5. Databases

Four of them: `tw_world`, `tw_char`, `tw_logon`, `tw_logs`.

```bash
mysql -u root -p < sql/create_databases.sql
```

That one file does more than its name suggests: besides creating the four
databases it brings 415 table definitions with it, the complete schema for
`tw_char`, `tw_logon` and `tw_logs` included. Only the world *content* is
kept separate, which is what `sql/base` holds — all 186 files there are
`tw_world_*`, so there is no `characters.sql` to look for.

Then import **every file in `sql/base`** into `tw_world` — that is the actual
world content. `sql/setup_databases.sh` exists but skips `sql/base` entirely, so
if you use it, import the base data yourself in between.

### The migrations, and why the auto-updater cannot do this for you

Turn the auto-updater **off** for the first start:

```
Database.AutoUpdate.Enabled = 0
```

Then apply the migrations yourself, tolerating errors:

```bash
for f in sql/database_updates/*.sql; do mysql --force -u root -p tw_world < "$f"; done
```

Record them as applied so the updater does not retry, then switch it back on for
future updates:

```bash
for f in sql/database_updates/*.sql; do
  n=$(basename "$f" .sql)
  mysql -u root -p -e "INSERT IGNORE INTO tw_world.migrations (Name, Hash, AppliedAt) VALUES ('$n','manual',NOW());"
done
```

**Why the detour.** `sql/base` is not the state "the first N migrations were
applied". It is a mixed snapshot: of the 101 migration files, 37 contain keys
that are already in the base data — some entirely, one to 91%, another to 5% —
while 38 are wholly new and 26 only change the schema. So there is no set of
rows you could put into the `migrations` table that would let the updater run
cleanly. Left to itself it replays everything, collides on the first duplicate
key, and the server refuses to start.

`--force` is what makes it work: duplicate-key errors are skipped, the `ALTER
TABLE` statements land, and the database ends up on the current schema. It does
also swallow genuine errors, which is the price. Verify afterwards:

```bash
mysql -u root -p -e "SELECT COUNT(*) FROM information_schema.COLUMNS WHERE TABLE_SCHEMA='tw_world' AND TABLE_NAME='spell_template' AND COLUMN_NAME='script_name';"
```

A `1` means the schema changes went in.

`sql/tools/probe_migration_overlap.py` is the script those numbers come from; run
it against a checkout to re-derive them.

### Playerbot tables

Built with `-DBUILD_PLAYERBOTS=ON`? Then the module's own tables have to go in as
well, or the server aborts on startup with `Table 'ai_playerbot_weightscales'
doesn't exist` — and it aborts through an assertion, so the message scrolls past
in a stack trace rather than telling you plainly what to do.

```bash
cd src/modules/PlayerBots/sql
cat world/*.sql world/classic/*.sql | mysql -u root -p tw_world
cat characters/*.sql | mysql -u root -p tw_char
```

Eight files into the world database, six into the characters one. `world/classic`
is the vanilla set; the `tbc` and `wotlk` siblings do not apply here. Anything
under `sql/other` is maintenance — deleting and resetting bots — not part of a
first install.


> **Caveat.** The auto-updater only works on a database built through it from the
> start. On one restored from a full dump the `migrations` table does not line up
> with the files on disk, the updater replays old migrations until one fails on a
> duplicate key, and the server refuses to start. Keep it off in that case and
> record migrations by hand:
>
> ```sql
> INSERT INTO migrations (Name, Hash, AppliedAt)
> VALUES ('20260726112016_world', 'manual', NOW());
> ```

> **MariaDB 11.8 and newer changed the default collation.** New databases get
> `utf8mb4_uca1400_ai_ci`, while dumps and SQL files written against older
> versions carry `utf8mb3_general_ci`. Mixing the two produces *"Illegal mix of
> collations"* on joins between such tables. If you hit that, align the tables
> rather than the queries:
>
> ```sql
> ALTER TABLE <name> CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_uca1400_ai_ci;
> ```

## 6. Configuration

Copy each `.dist` in `<prefix>/etc` and drop the suffix:

| Template | Becomes |
|---|---|
| `mangosd.conf.dist` | `mangosd.conf` |
| `realmd.conf.dist` | `realmd.conf` |
| `aiplayerbot.conf.dist` | `aiplayerbot.conf` |
| `ahbot.conf.dist` | `ahbot.conf` |

Those four are all there is. `rate.conf` and `mods.conf` have templates in the
source tree but nothing reads them — `_RATE_CONFIG` and `_MODS_CONFIG` are
declared in `SystemConfig.h` and used nowhere. The rate settings live in
`mangosd.conf`.

Database credentials go into `mangosd.conf` and `realmd.conf`. Everything the
fork adds is off by default:

`mangosd.conf`:

```
LFT.BotFill.Enable = 1
SoloDungeonRepopAlive.Enable = 1
Leech.Enable = 1
```

`aiplayerbot.conf` — this one is not a `mangosd.conf` key, which is easy to trip
over since every other switch is:

```
AiPlayerbot.Enabled = 1
```

While you are in that file, turn the population down for the first run. The
shipped template asks for a thousand:

```
AiPlayerbot.MinRandomBots = 10
AiPlayerbot.MaxRandomBots = 10
```

Those characters are created and geared before the world finishes coming up, so
a thousand of them turns the first start into a long wait for no benefit. Raise
it once everything works — this realm runs a thousand comfortably, but that is a
tuning question, not a setup one.

Also worth turning off before the first start:

```
LogSQL = 0
```

The template ships it as `1`, which writes every single SQL statement to disk.
With playerbots the first start computes the gear cache for every class, spec
and level — tens of thousands of inserts, each one a disk write. The cache is
built once and read back on later starts, so this only really hurts the first
time, but it hurts a lot.

`create_databases.sql` creates the `realmlist` table in `tw_logon` but leaves it
**empty**. Insert a row with your server's address, and put the same address in
the client's `realmlist.wtf`.

```sql
INSERT INTO tw_logon.realmlist (name, address, port, icon, realmflags, timezone, allowedSecurityLevel)
VALUES ('TurtleWoW', '127.0.0.1', 8090, 0, 0, 1, 0);
```

Two fields decide whether this works at all:

**`port` must match `WorldServerPort` in `mangosd.conf`.** The defaults disagree
with each other — the config ships `8090`, while the column default on
`realmlist.port` is `8085`. Take the config's value. Get this wrong and login
succeeds, the realm appears in the list, and the client then hangs before
character selection: it was handed a port nobody is listening on.

**`realmflags` has to be 0.** The default is `2`, which means offline — the world
server sets and clears that itself. Left at 2 the realm shows as permanently
offline.

## 7. Running it

`realmd` first, then `mangosd`. Two systemd units and an ordering dependency are
enough. The world server wants a console: give it a FIFO and hold it open, or it
sees EOF immediately and shuts down.

```bash
mkfifo ~/turtle/mangosd.in
```

```ini
[Service]
Type=simple
User=turtle
WorkingDirectory=/home/turtle/turtle/bin
ExecStart=/bin/bash -c 'exec 3<>/home/turtle/turtle/mangosd.in; exec /home/turtle/turtle/bin/mangosd -c /home/turtle/turtle/etc/mangosd.conf <&3'
Restart=always
RestartSec=15
TimeoutStartSec=900
```

`exec 3<>` opens the FIFO read-write and keeps it open, so the server never sees
EOF. Console commands then go in from anywhere:

```bash
echo 'server restart 45' > ~/turtle/mangosd.in
```

`TimeoutStartSec=900` matters: the first start applies migrations and builds the
bot travel graph, which takes minutes.

## 8. Checking that it worked

The log must contain

```
Loading TalentSpecs
```

with **no** `Error with premade spec link` lines after it. If you see those and
`No premade specs found!!` at the end, the `aiplayerbot.conf` in use is from an
older checkout and still carries the stock vanilla talent links — every one of
them is rejected against Turtle's reworked trees, and the bots end up with no
talents at all.

## A note on the bot event log

`AiPlayerbot.AllowedLogFiles` ships with `bot_events.csv,deaths.csv`, and the
lines written there used to be passed to `vfprintf` as a format string. Any
percent sign in a bot or mob name was read as a conversion specifier. glibc
prints nonsense and carries on, so on Linux the only symptom was garbled rows in
the CSV; the same bug killed the Windows server outright. Fixed — but if you
have older CSVs lying around, that is where the mangled lines came from.

## Troubleshooting

| Symptom | Cause |
|---|---|
| Client crashes with "interface corrupt" | built without `ALLOW_TURTLE_ADDONS` |
| No bots, no error | built without `BUILD_PLAYERBOTS` |
| `AI Playerbot is Disabled. Unable to open configuration file` | the install was moved after building — `SYSCONFDIR` is compiled in |
| Server exits seconds after starting, no error | no console on stdin; see the FIFO above |
| `No premade specs found!!` | old `aiplayerbot.conf` with the stock talent links |
| `Illegal mix of collations` | MariaDB 11.8 default vs an older dump |
| Refuses to start after migrations | auto-updater against a dump-restored database |
| World is empty, no creatures | `sql/base` was never imported |
