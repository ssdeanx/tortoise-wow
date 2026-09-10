-- ==============================================
-- FILE: broadcast_text_sound_chattype_emote.sql
-- GENERATED: 20260906134216
-- ==============================================
-- A clean boot logs eight `broadcast_text` complaints. All eight are real data
-- faults, and two of the three kinds cost content that the client already ships.

-- ==============================================
-- FILE: sound_entries_dukedreadmoore.sql
-- ==============================================
-- Duke Dreadmoore (61972) speaks without a voice.
--
--     BroadcastText (Id: 30237) ... has SoundId 60443 but sound does not exist.
--     BroadcastText (Id: 30238) ... has SoundId 60444 but sound does not exist.
--
-- ObjectMgr drops a sound id that `sound_entries` does not know and plays the
-- line silently. Both ids exist in the client's SoundEntries.dbc - 60443 is
-- Dukedread1.mp3 and 60444 is Dukedread2.mp3, both under Sound\Interface\VA,
-- the same place every other custom voice line lives. Only the server-side
-- registry row is missing, so the recordings ship but never play. The names
-- below are the ones the DBC itself carries, matching how the neighbouring
-- entries (Garlok_aggro, Zeljeb_aggro) were registered.
--
-- These are the only two rows in `broadcast_text` pointing at a sound that
-- does not exist.
INSERT IGNORE INTO `sound_entries` (`id`, `name`) VALUES
(60443, 'Dukedread1'),
(60444, 'Dukedread2');

-- ==============================================
-- FILE: broadcast_text_chat_type.sql
-- ==============================================
-- Five rows carry a chat_type the core has no value for.
--
--     BroadcastText (Id: 6249501) ... has ChatType 12 but this chat type does not exist.
--     BroadcastText (Id: 6271501) ... has ChatType 11 but this chat type does not exist.
--
-- ChatType is 0..6 here (CHAT_TYPE_SAY..CHAT_TYPE_ZONE_YELL, Creature.h), and 11
-- and 12 come from the client's ChatMsg numbering instead. ObjectMgr forces every
-- one of them to CHAT_TYPE_SAY, so boss lines meant to carry across the zone in
-- yellow are whispered in white at 25 yards.
--
-- What each was meant to be is recorded in creature_ai_events.comment on the
-- scripts that use them:
--
--     6249501  Zel'jeb the Ancient - Yell on aggro
--     6249502  Zel'jeb the Ancient - Curse of Years at 50% health
--     6249503  Zel'jeb the Ancient - Yell on death
--     6271501  Arkod the White - Say on aggro
--     6271502  Arkod the White - Say on death
--
-- 6249502 is the one inference here: its comment names the ability rather than
-- the channel, but it belongs to the same boss as two documented yells and has
-- its own recording (Zeljeb_half, 60513), so it is treated as a yell too.
--
-- The old value is in the WHERE clause so re-running this cannot overwrite a
-- later correction.
UPDATE `broadcast_text` SET `chat_type` = 1
WHERE `entry` IN (6249501, 6249502, 6249503) AND `chat_type` = 12;

UPDATE `broadcast_text` SET `chat_type` = 0
WHERE `entry` IN (6271501, 6271502) AND `chat_type` = 11;

-- ==============================================
-- FILE: broadcast_text_emote.sql
-- ==============================================
-- One row points at an emote the client does not have.
--
--     BroadcastText (Id: 92031) ... has emoteId2 55 but emote does not exist.
--
-- (The message names the wrong field - it is emote_id1. Fixed separately in
-- ObjectMgr.cpp.)
--
-- Emotes.dbc holds 124 emotes and 55 is not among them; the nearest valid ones
-- are 53, 54, 60 and 61. The row has been carrying 55 since the very first
-- commit in this repository, so there is no author to ask what was intended,
-- and the text ("I'm not crazy, I swear. These shells are worth a lot of
-- money.") does not point at any particular animation.
--
-- Set to 0, which is what ObjectMgr already does with it in memory on every
-- start - the stored data now says the same thing. If the intended emote ever
-- turns up, this is one UPDATE away from being restored.
--
-- This is the only row in `broadcast_text` with an emote the client lacks.
UPDATE `broadcast_text` SET `emote_id1` = 0
WHERE `entry` = 92031 AND `emote_id1` = 55;
