# Tech Stack

## Core Engine & Languages
- C++17 (`set(CMAKE_CXX_STANDARD 17)` in root `CMakeLists.txt`).
- C (C99/C11 for third-party libraries in `dep/`).
- Embedded Lua 5.2 engine (`ELUNA_LUA_VERSION=lua52`) for server-side scripts via `src/modules/Eluna`.
- Client baseline: Turtle-WoW 1.18.1 client (Build 7272).

## Build System & Toolchain
- CMake 3.16+ (minimum requirement 3.1...3.29).
- GCC / Clang on Linux x86_64.
- Build target: `build/src/mangosd/mangosd` (World daemon) and `build/src/realmd/realmd` (Auth daemon).
- Mandatory CMake configuration options:
  - `-DCMAKE_BUILD_TYPE=Release`
  - `-DCMAKE_INSTALL_PREFIX=/home/sam/server`
  - `-DBUILD_PLAYERBOTS=ON` (compiles AI playerbot module)
  - `-DUSE_EXTRACTORS=ON`
  - `-DALLOW_TURTLE_ADDONS=ON`
  - `-DBUILD_ELUNA=ON`
  - `-DUSE_STD_MALLOC=ON` (standard malloc over TBB)

## External Dependencies & Libraries
- Boost: Header-only core used in modern Boost (Boost `system` link component removed for Boost 1.91+ compatibility).
- OpenSSL: Crypto and SRP6 auth hashing.
- MariaDB Client (`libmariadb` / MySQL connector): Database connection pooling and query execution.
- zlib, bzip2: Map, DBC, and packet compression.

## Service & Runtime Infrastructure
- Linux systemd user units (`~/.config/systemd/user/`):
  - `turtle-mangosd.service`: World simulation daemon.
  - `turtle-realmd.service`: Auth & realm router daemon.
  - `turtle-logrotate.timer`: Rotates playerbot and server logs.
- MariaDB 11.8 on `127.0.0.1:3306` (databases: `tw_world`, `tw_char`, `tw_logon`, `tw_logs`).
- Network ports: `8090` (World / mangosd), `3724` (Auth / realmd).
