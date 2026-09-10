-- ==============================================
-- FILE: viper_set_venom_proc.sql
-- GENERATED: 20260731190000
-- ==============================================
-- Embrace of the Viper, six piece bonus: give the serpent a bite.
--
-- Spell 44085 "Cat to Serpent Form" turns cat form into Cobrahn's serpent and
-- did nothing else. It now also applies a poison, which is what the shape
-- suggests it should do.
--
-- The poison is 744, the one the Deviate Adders use in Wailing Caverns where the
-- set drops: nature damage every three seconds for thirty, dispellable as a
-- poison. An existing spell rather than a new one on purpose - the client takes
-- name, icon and tooltip from its own Spell.dbc, and an id it does not know
-- would show up as nothing.
--
-- procFlags 20 is PROC_FLAG_DEAL_MELEE_SWING plus PROC_FLAG_DEAL_MELEE_ABILITY,
-- so both white swings and Claw or Shred can carry it. procChance was 101, the
-- "always" value; ten per cent is roughly every tenth landed attack at cat
-- speed. No spell_proc_event row is wanted: without one the core requires a hit
-- or a crit, which is exactly right here.
--
-- The form test is in the script, since it cannot be expressed in the data.

UPDATE `spell_template`
SET `procFlags`   = 20,
    `procChance`  = 10,
    `script_name` = 'spell_item_viper_venom'
WHERE `entry` = 44085;
