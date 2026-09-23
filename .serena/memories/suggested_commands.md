# Suggested Commands

## Service Management (systemd user units - no sudo required)
- Stop daemons cleanly before syncing or modifying binaries:
  ```bash
  systemctl --user stop turtle-mangosd turtle-realmd
  ```
- Start daemons:
  ```bash
  systemctl --user start turtle-mangosd turtle-realmd
  ```
- Restart world server after configuration changes:
  ```bash
  systemctl --user restart turtle-mangosd
  ```
- Clear rate limits after crashes:
  ```bash
  systemctl --user reset-failed turtle-mangosd turtle-realmd
  systemctl --user start turtle-mangosd turtle-realmd
  ```
- Inspect active unit status:
  ```bash
  systemctl --user status turtle-mangosd turtle-realmd
  ```

## Diagnostics & Log Inspection
- Follow live world daemon logs:
  ```bash
  journalctl --user -u turtle-mangosd -f
  ```
- View recent world logs (last 5 minutes):
  ```bash
  journalctl --user -u turtle-mangosd --since "5 min ago"
  ```
- View recent auth daemon logs:
  ```bash
  journalctl --user -u turtle-realmd --since "5 min ago"
  ```
- Inspect active listening ports (`mangosd` shows as `MainThread` in `ps`, so check socket directly):
  ```bash
  ss -ltn | grep -E '8090|3724'
  ```

## C++ Engine Build Commands
- Full Release build configuration:
  ```bash
  cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/home/sam/server \
    -DBUILD_PLAYERBOTS=ON \
    -DUSE_EXTRACTORS=ON \
    -DALLOW_TURTLE_ADDONS=ON
  ```
- Parallel compilation:
  ```bash
  cmake --build build -j$(nproc)
  ```
- Targeted rebuild of world server binary only:
  ```bash
  cmake --build build --target mangosd -j$(nproc)
  ```

## Architecture & Unit Tests
- Configure and execute architecture contract tests:
  ```bash
  cmake -B build/tests -S tests/architecture
  cmake --build build/tests -j$(nproc)
  ctest --test-dir build/tests --output-on-failure
  ```

## Remote Sync & Pipeline Execution
- Pull and merge latest commits from origin tracking branch:
  ```bash
  git stash save "Local modifications before sync"
  git pull origin playerbots-integration-gh
  git stash pop
  ```
- Audit incoming changes for SQL/DBC modifications:
  ```bash
  git diff HEAD@{1}..HEAD --stat
  ```
