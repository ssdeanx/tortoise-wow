# Database Topology & Operations

## MariaDB Topology & Credentials
- Server endpoint: `127.0.0.1:3306` (MariaDB 11.8 system daemon).
- Application user: `mangos`, password: `mangos` (all privileges on target databases).
- Root system user connects via unix_socket (`sam`, no password).
- Primary databases:
  - `tw_world`: World definitions, game objects, creatures, quests, spells, scripts.
  - `tw_char`: Characters, guilds, mail, inventories, playerbot runtime states.
  - `tw_logon`: Realm definitions and authentication accounts.
  - `tw_logs`: System audit and command history logs.

## Migration Protocol (Strictly Conditional)
- Automatic updater disabled: `Database.AutoUpdate.Enabled = 0` in `mangosd.conf`.
- Invariant: Only apply migrations if incoming git commits contain new `.sql` files in `sql/database_updates/`. Otherwise, MariaDB MUST NOT be touched.
- World migrations (`sql/database_updates/world/`):
  ```bash
  mariadb --force -h 127.0.0.1 -u mangos -pmangos tw_world < sql/database_updates/world/NEW_FILE.sql
  mariadb -h 127.0.0.1 -u mangos -pmangos -e "INSERT IGNORE INTO tw_world.migrations (Name, Hash, AppliedAt) VALUES ('NEW_FILE_NAME','manual',NOW());"
  ```
- Character migrations (`sql/database_updates/character/`):
  ```bash
  mariadb -h 127.0.0.1 -u mangos -pmangos tw_char < sql/database_updates/character/NEW_FILE.sql
  ```

## Weekly Honor Maintenance Invariant
- Maintenance routine: `HonorMaintenancer::DoMaintenance()` executes at each weekly reset and expects `character_inventory_copy`.
- Required table schema constraint:
  ```sql
  CREATE TABLE IF NOT EXISTS tw_char.character_inventory_copy LIKE tw_char.character_inventory;
  ```
