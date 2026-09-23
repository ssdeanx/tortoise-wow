# Task Completion Checklist

## 1. Code Cleanliness & Diagnostics Check
- Verify that no temporary debug prints, orphaned comments, or unauthorized performance diagnostics remain in the diff.
- Confirm any added diagnostic flags conform to `doc/TURTLE_DIAGNOSTICS.md`.

## 2. Compilation Verification
- Rebuild affected target (e.g. `mangosd`):
  ```bash
  cmake --build build --target mangosd -j$(nproc)
  ```
- Or perform a full project build if shared, framework, or module headers were modified:
  ```bash
  cmake --build build -j$(nproc)
  ```
- Ensure zero compilation errors and address any newly introduced warnings.

## 3. Architecture & Contract Test Validation
- Build and run the architecture regression test suite:
  ```bash
  cmake -B build/tests -S tests/architecture
  cmake --build build/tests -j$(nproc)
  ctest --test-dir build/tests --output-on-failure
  ```
- When core hook contracts are modified, verify contracts via `tools/audit-core-contracts.ps1` (offline snapshot mode without database writes).

## 4. Runtime Service Smoke Test (If Requested by Operator)
- Stop running daemons cleanly:
  ```bash
  systemctl --user stop turtle-mangosd turtle-realmd
  ```
- Reset failed state if crashes occurred:
  ```bash
  systemctl --user reset-failed turtle-mangosd turtle-realmd
  ```
- Start daemons:
  ```bash
  systemctl --user start turtle-mangosd turtle-realmd
  ```
- Verify both ports are actively listening:
  ```bash
  ss -ltn | grep -E '8090|3724'
  ```
- Inspect recent journal logs for runtime assertion failures or startup errors:
  ```bash
  journalctl --user -u turtle-mangosd -n 50 --no-pager
  ```

## 5. Working Tree Audit
- Review modified files using `git status` to ensure no unexpected files or build artifacts are staged.
