# Installing on Windows

Start to finish, for someone who has just unpacked this repository and has
nothing else set up yet. Written against the `playerbots-integration-gh`
branch.

**What is in the repository:** the server source and the full world database
(131 MB under `sql/base`, 186 files).

**What is not:** the client data. Maps, DBC and vmaps have to be extracted from
a game client — see step 4. You need a **Turtle WoW 1.18.1 client, build
7272**; a client that does not match will misbehave in a hundred small ways.

---

## 1. Prerequisites

| Component | Notes |
|---|---|
| Visual Studio 2022 | workload **Desktop development with C++** |
| CMake | 3.16 or newer, on `PATH` |
| MariaDB or MySQL | the server, plus its command line client on `PATH` — the installer's "add to PATH" box is easy to miss |
| **ACE 7.x or 8.x** | **not** bundled — install it and pass `-DACE_ROOT=` if CMake cannot find it |

MySQL, OpenSSL and zlib are bundled under `dep/windows`, and Recast, G3D,
libmpq and fmt under `dep/`. Those need no separate install. ACE is the one
dependency you have to supply yourself.

Installing ACE through vcpkg is fine — just point at it directly instead of
pulling in the whole toolchain, which would break OpenSSL as described below:

```
vcpkg install ace:x64-windows
cmake -B build -A x64 -DBUILD_PLAYERBOTS=ON -DUSE_EXTRACTORS=ON -DACE_ROOT=C:/vcpkg/installed/x64-windows
```

Precompiled headers: on MSVC the build defaults to the old PCH implementation
(`USE_PCH_OLD=ON`, `/Yc` and `/Yu` through `cmake/FindPCHSupport.cmake`). Add
`-DUSE_PCH_OLD=OFF` to use CMake's own `target_precompile_headers` instead - the
path the Linux build takes and the one that gets tested. If the PCH hits MSVC's
size limits, `-DUSE_PCH=OFF` builds without one; the headers the sources expect
are then force-included, in either PCH mode.

```
```

With `-DBUILD_PLAYERBOTS=ON` you need Boost as well. Install the nine libraries
the module actually includes rather than the `boost` meta-package — that one
drags in `boost-cobalt`, which needs C++20 and does not build under Visual
Studio 2019:

```
vcpkg install boost-algorithm:x64-windows boost-asio:x64-windows boost-bimap:x64-windows boost-bind:x64-windows boost-filesystem:x64-windows boost-functional:x64-windows boost-smart-ptr:x64-windows boost-stacktrace:x64-windows boost-thread:x64-windows boost-system:x64-windows
```

Then add `-DBOOST_ROOT=C:/vcpkg/installed/x64-windows` to the configure line.
Two of those, `filesystem` and `thread`, are compiled libraries rather than
header-only, so they have to be linked and not merely found.

`FindACE.cmake` looks for `ace/ACE.h` under `${ACE_ROOT}` and `${ACE_ROOT}/include`
and for the library under `${ACE_ROOT}/lib`, which is exactly vcpkg's layout.
Watch the configure output for `Found ACE headers:` — if it is missing, nothing
else will work.

Anything you take from vcpkg is built as a DLL, and those have to sit next to
`mangosd.exe` or it will not start. Copy them once the install is done:

```
copy C:\vcpkg\installed\x64-windows\bin\ACE.dll C:\turtle-server\
copy C:\vcpkg\installed\x64-windows\bin\boost_*.dll C:\turtle-server\
copy C:\vcpkg\installed\x64-windows\bin\lib*-3-x64.dll C:\turtle-server\
```

The wildcard on Boost is deliberate — `thread` pulls in `atomic` and `chrono`,
and chasing them one missing-DLL dialog at a time is a waste of an evening. The
third line is OpenSSL 3.x and only applies if you pointed `OPENSSL_LIBRARIES` at
vcpkg; with the bundled OpenSSL the install brings its own. `libmySQL.dll` comes
with the install either way.

### OpenSSL 3 needs its legacy provider

If you linked vcpkg's OpenSSL 3.x, set this as well:

```
setx OPENSSL_MODULES "C:\vcpkg\installed\x64-windows\bin" /M
```

RC4 moved into the legacy provider with OpenSSL 3.0, and the session encryption
every client connection is built on uses RC4. vcpkg compiles a module search
path into its OpenSSL that points at its own build directory, which no longer
exists once the package is installed — so `legacy.dll` sits in
`installed/x64-windows/bin` and OpenSSL never looks there.

Without it, everything appears fine: the build succeeds, the server starts, the
realm shows up, and then the login dies. Older builds crashed outright with an
access violation in `EVP_CIPHER_CTX_set_key_length`, because three OpenSSL
return values in `ARC4::ARC4` went unchecked; current ones log what is missing
instead.

Copying `legacy.dll` next to `mangosd.exe` does **not** help — provider modules
are only looked for in the module directory, never beside the executable.
Restart Visual Studio or your console after `setx`; running processes do not
pick the variable up.

> **The ACE version matters.** This tree is built as C++17, which removed
> dynamic exception specifications. ACE 6.x still uses them, so its headers
> produce a cascade of exception-specification errors in `WorldSocketMgr.cpp`
> and anything that includes it. That is ACE, not this code — the core's own
> headers contain no `throw()` at all, and patching them only moves the error.
> Verified working: **ACE 8.0.2**.

> **Do not add the vcpkg toolchain file.** The `if(WIN32)` branch of the
> top-level `CMakeLists.txt` deliberately pins MySQL, OpenSSL and zlib to the
> copies under `dep/windows` — `find_package(OpenSSL)` is only called on UNIX.
> Passing `-DCMAKE_TOOLCHAIN_FILE=...vcpkg.cmake` puts vcpkg's OpenSSL 3.x
> headers ahead of the bundled 1.1.1 ones while the hard-coded 1.1.1 import
> libraries still win at link time. The result is exactly two unresolved
> symbols, `OSSL_PROVIDER_load` and `SSL_get1_peer_certificate` — both of which
> the code guards by version and would otherwise never reference. If you have
> already configured with the toolchain, delete the build directory; the cached
> variables survive a re-run.

## 2. Configure and build

```
cmake -B build -A x64 -DBUILD_PLAYERBOTS=ON -DUSE_EXTRACTORS=ON
cmake --build build --config Release
```

Two flags matter:

- **`BUILD_PLAYERBOTS` defaults to `OFF`.** Leave it out and you get a server
  with no bots at all, with no warning anywhere — the module simply is not
  compiled in.
- **`USE_EXTRACTORS`** builds the tools you need in step 4. Skip it only if you
  already have `dbc`, `maps`, `vmaps` and `mmaps` from elsewhere.

### Pass `ACE_ROOT` as a cache entry, not an environment variable

The tree unsets `ACE_INCLUDE_DIR`, `ACE_LIBRARIES` and `ACE_LIBRARIES_DIR` from
the cache at the top of every configure, so ACE is searched for again from
scratch each time — including on the automatic reconfigure Visual Studio runs
through `ZERO_CHECK` whenever a `CMakeLists.txt` changes. That reconfigure does
not inherit the environment of the shell you first configured in, so an
`ACE_ROOT` set with `set` is gone and the build stops at

```
CMake Error at CMakeLists.txt:232 (message):
  This project requires ACE installed.
```

Give it on the command line instead — `-DACE_ROOT=C:/vcpkg/installed/x64-windows`
— and it survives in the cache. If it still fails, reconfigure from a command
prompt with `cmake -S . -B build`: the full output names what the find module is
missing, which the Error List truncates to nothing useful.

`ALLOW_TURTLE_ADDONS` is already on by default. It has to stay on: without it
the client crashes with *"interface corrupt"* the moment you enter the world.

If the link fails on `World::FinalizePlayerbotsPostPlayerInfo` or
`Player_DispatchBotChatCommand`, the checkout predates the stub fix — pull, or
see `src/game/PlayerbotStubs.cpp`. Those two only ever surface with
`BUILD_PLAYERBOTS=OFF`, the one configuration nobody builds on Linux.

## 3. Install into one folder

```
cmake --install build --config Release
```

The target directory has to be set when you **configure**, not here:

```
cmake -B build -A x64 -DCMAKE_INSTALL_PREFIX=C:/turtle-server ...
```

`--prefix` on the install line has no effect in this tree. `BIN_DIR`, `CONF_DIR`
and `LIBS_DIR` are computed from `CMAKE_INSTALL_PREFIX` while configuring and
baked into the install rules as absolute paths, so a prefix given later is
ignored and everything lands under the configure-time default —
`C:/Program Files/TurtleWoW`.

Install rather than running from the build tree. On Windows the CMake files put
binaries **and** config files into the same flat directory, which is exactly
what the server expects — see the note on `aiplayerbot.conf` in step 6. Run
`mangosd.exe` straight out of `build\src\mangosd\Release\` and the configs sit
one directory above it, where nothing looks for them.

## 4. Extract the client data

Copy the extractors from `tools/` into your **client** directory and run them
there, in this order:

1. `extractor` — produces `dbc` and `maps`
2. `vmap_extractor`, then `vmap_assembler` — produces `vmaps`
3. `mmap` — produces `mmaps` (slow, an hour or more is normal)

Move all four resulting folders next to `mangosd.exe`.

## 5. Databases

Four of them: `tw_world`, `tw_char`, `tw_logon`, `tw_logs`.

```
mariadb -u root -p < sql\create_databases.sql
```

That one file does more than its name suggests: besides creating the four
databases it brings 415 table definitions with it, the complete schema for
`tw_char`, `tw_logon` and `tw_logs` included. Only the world *content* is
kept separate, which is what `sql/base` holds — all 186 files there are
`tw_world_*`, so there is no `characters.sql` to look for.

On MariaDB the client is called `mariadb`; `mysql` is the older name and may not
be there at all. `sql\setup_databases.bat` looks for both, in that order. If
neither is found, the binaries were installed but not put on `PATH` — they live
in `C:\Program Files\MariaDB <version>\bin`, and the full path works just as
well as fixing `PATH`.

Then import **every file in `sql\base`** into `tw_world`. This is the actual
world content — creatures, quests, items, the lot.

> There is a `sql\setup_databases.bat`, but read it before you trust it: it runs
> `create_databases.sql` and then everything in `sql\database_updates`, and
> **skips `sql\base` entirely**. If you use it, import the base data yourself in
> between the two.

### The migrations, and why the auto-updater cannot do this for you

Turn the auto-updater **off** for the first start:

```
Database.AutoUpdate.Enabled = 0
```

Then apply the migrations yourself, tolerating errors, and record them as done:

```
cd sql
for %f in (database_updates\*.sql) do mariadb --force -u root -p tw_world < "%f"
```

Record them as applied so the updater does not retry — `%~nf` is the filename
without its extension, which is exactly what the table wants:

```
for %f in (database_updates\*.sql) do mariadb -u root -p tw_world -e "INSERT IGNORE INTO migrations (Name,Hash,AppliedAt) VALUES ('%~nf','manual',NOW());"
```

Afterwards switch `Database.AutoUpdate.Enabled` back on, and future updates apply
normally.

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

```
mariadb -u root -p -e "SELECT COUNT(*) FROM information_schema.COLUMNS WHERE TABLE_SCHEMA='tw_world' AND TABLE_NAME='spell_template' AND COLUMN_NAME='script_name';"
```

A `1` means the schema changes went in.

### Playerbot tables

Built with `-DBUILD_PLAYERBOTS=ON`? Then the module's own tables have to go in as
well, or the server aborts on startup with `Table 'ai_playerbot_weightscales'
doesn't exist` — and it aborts through an assertion, so the message scrolls past
in a stack trace rather than telling you plainly what to do.

```
cd src\modules\PlayerBots\sql
for %f in (world\*.sql world\classic\*.sql) do mariadb -u root -p tw_world < "%f"
for %f in (characters\*.sql) do mariadb -u root -p tw_char < "%f"
```

Eight files into the world database, six into the characters one. `world\classic`
is the vanilla set; the `tbc` and `wotlk` siblings do not apply here. Anything
under `sql\other` is maintenance — deleting and resetting bots — not part of a
first install.


> **Caveat.** The auto-updater only works on a database built through it from
> the beginning. On a database restored from a full dump the `migrations` table
> does not line up with the files on disk, the updater replays old migrations
> until one fails on a duplicate key, and the server refuses to start. If that
> is your situation, keep it disabled and apply new migrations by hand,
> recording each one afterwards:
>
> ```sql
> INSERT INTO migrations (Name, Hash, AppliedAt)
> VALUES ('20260726112016_world', 'manual', NOW());
> ```

## 6. Configuration files

The build produces six templates ending in `.dist`. Copy each one and drop the
suffix:

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

Put your database credentials into `mangosd.conf` and `realmd.conf`.

### The one Windows-specific trap

The path to the bot configuration is resolved differently per platform
(`PlayerbotAIConfig.h`):

```cpp
#if PLATFORM == PLATFORM_WINDOWS
inline std::string _D_AIPLAYERBOT_CONFIG = "aiplayerbot.conf";
#else
inline std::string _D_AIPLAYERBOT_CONFIG = SYSCONFDIR "aiplayerbot.conf";
#endif
```

On Linux the directory is compiled in. **On Windows the path is relative**, so
`aiplayerbot.conf` has to sit in the working directory the server is started
from — next to `mangosd.exe` if you launch it normally. Get this wrong and the
server starts perfectly happily, logs one line saying the file could not be
opened, and runs with no bots. Following step 3 puts it in the right place
already.

`ahbot.conf` next to it follows the *other* rule: `AhBotConfig.cpp` uses
`SYSCONFDIR"ahbot.conf"` with no platform branch, and `SYSCONFDIR` is a compile
definition set from `CMAKE_INSTALL_PREFIX`. So that one is looked up at an
absolute path baked into the binary. Two files in the same directory, found two
different ways.

The practical consequence: changing `CMAKE_INSTALL_PREFIX` changes a
preprocessor definition, so everything that sees it gets recompiled. Decide
where the server should live before the first build rather than after.

Note also that `cmake --install` on its own never re-runs the configure step.
Install rules and generated templates — `aiplayerbot.conf.dist` and
`ahbot.conf.dist` among them — only appear after `cmake -B build ...` has run
again.

### Switches that default to off

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

## 7. Realm entry and an account

`create_databases.sql` creates the `realmlist` table in `tw_logon` but leaves it
**empty**. Insert a row with your server's name and address, and put the same
address into the client's `realmlist.wtf`.

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

Start `mangosd.exe` once and create your account from its console with
`account create`, then raise it with `account set gmlevel`.

## 8. Starting

`realmd.exe` first, then `mangosd.exe`. The first start takes a long time: the
migrations run and the bot travel graph is computed from scratch.

**Check that the bots came up properly.** The log has to contain

```
Loading TalentSpecs
```

with **no** `Error with premade spec link` lines after it. If instead you see
those errors and `No premade specs found!!` at the end, you are running an
`aiplayerbot.conf` from an older checkout that still carries the stock vanilla
talent links — every one of them is rejected against Turtle's reworked talent
trees, and the bots end up with no talents at all. Regenerate the file, or copy
the `AiPlayerbot.PremadeSpec*` block out of `aiplayerbot.conf.dist`.

## Reading a crash dump

mangosd catches its own crashes and writes `crash_<timestamp>.dmp` beside the
executable. Release builds carry debug information, so those dumps are readable:
`mangosd.pdb` is produced next to `mangosd.exe` and Visual Studio finds it
without being told.

Open the `.dmp` through **File → Open → File**, then pick **Debug with Native
Only** on the summary page that appears. The debugger stops at the point of the
crash and the call stack names the function.

The top few frames are always the crash handler itself —
`Mangosd_WriteCrashDump`, `MangosdSignalHandler`,
`MangosdInvalidParameterHandler`. The first frame *below* those is where the
fault actually happened.

Two things worth knowing before you spend an afternoon on it:

- **Running mangosd under F5 tells you nothing.** The handler is installed
  before anything else runs, so Visual Studio never sees an exception; the
  process just ends with code 3. The dump is the only way in.
- **Bare addresses instead of names** mean the `.pdb` does not match the `.exe`.
  Rebuild, reproduce the crash, and read the new dump — both then come from the
  same build.

## Troubleshooting

| Symptom | Cause |
|---|---|
| Client crashes with "interface corrupt" on entering the world | built without `ALLOW_TURTLE_ADDONS` |
| No bots anywhere, no error | built without `BUILD_PLAYERBOTS`, or `aiplayerbot.conf` not in the working directory |
| `AI Playerbot is Disabled. Unable to open configuration file` | `aiplayerbot.conf` is in the wrong place — see step 6 |
| `No premade specs found!!` | old `aiplayerbot.conf` with the stock talent links |
| Server refuses to start after applying migrations | auto-updater against a dump-restored database — see step 5 |
| World is empty, no creatures | `sql\base` was never imported |
| `invalid-parameter (0xc0000420)` then SIGABRT, seconds after the bots come up | checkout predates the logger fix; as a stopgap, empty `AiPlayerbot.AllowedLogFiles` |
| `CMake Error ... requires ACE installed` on a rebuild that worked before | `ACE_ROOT` was an environment variable — see step 2 |
