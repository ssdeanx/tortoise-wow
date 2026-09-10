-- ==============================================
-- FILE: trainer_type_zero_class_zero.sql
-- GENERATED: 20260802160000
-- ==============================================
-- Eighteen trainers could not be talked to at all.
--
-- creature_template.trainer_type 0 is TRAINER_TYPE_CLASS, and Creature::IsTrainerOf
-- then compares the player's class against trainer_class:
--
--     case TRAINER_TYPE_CLASS:
--         if (pPlayer->GetClass() != GetCreatureInfo()->trainer_class)
--             ... return false;
--
-- With trainer_class also 0 that matches no player - classes run 1 to 11 - so the
-- function returns false for everybody and the training window never opens. The
-- npc_trainer rows behind these NPCs were unreachable.
--
-- The correct value is not a guess. Every intact trainer of the same skill uses
-- trainer_type 2: 27 for blacksmithing, 27 for alchemy, 26 for tailoring, 25 for
-- fishing, 24 for engineering, 22 for herbalism, 20 for enchanting, 18 for
-- cooking, 12 for survival. Weapon masters likewise use 2 with trainer_class 0.
-- Portal trainers use 0 with class 8, and 44 of the 45 hunter trainers use 0 with
-- class 3.
--
-- Most of these are the expert and artisan ranks added later, which is why whole
-- professions dead-ended: Expert Blacksmith, Artisan Blacksmith, Expert Enchanter,
-- both Expert Survivalists.
--
-- Naela Trance (1459) is deliberately left alone. She is subtitled Bowyer but her
-- npc_trainer rows are mage spells - Fireball, Frostbolt, Polymorph. Setting her
-- to 2 would offer those to every class; setting her to a mage trainer contradicts
-- her subtitle. Her spell list wants sorting out first, and that is a separate
-- question.

-- Profession and weapon trainers.
UPDATE `creature_template`
SET `trainer_type` = 2
WHERE `entry` IN (
    8696,   -- Henry Stern (cooking)
    11557,  -- Meilosh, Timbermaw Hold Quartermaster (tailoring)
    50070,  -- Rufus Hardwick, Survival Trainer
    60510,  -- Grubgar, Fisher Orc
    61277,  -- Todd Bolder (blacksmithing)
    61438,  -- Cassie Copperlight, Aspiring Tinkerer (engineering)
    62143,  -- Smith Martin, Expert Blacksmith
    62282,  -- Linus Huxley, Chef (cooking)
    62286,  -- Pinky Tosslehouse, Herbalist
    62290,  -- Enchantress Magilou, Expert Enchanter
    62299,  -- The Witch of Northwind (alchemy)
    63056,  -- Ulf Stonetotem, Artisan Blacksmith
    63071,  -- Swampwalker Krug, Expert Survivalist
    63072,  -- Nerean Stagtree, Expert Survivalist
    80320   -- Rodfather, The Eel Hunter (fishing)
);

-- Class trainers: the type was right, the class was missing.
UPDATE `creature_template`
SET `trainer_class` = 3
WHERE `entry` = 61640;  -- Marksman Rembrandt Olar, Hunter Trainer

UPDATE `creature_template`
SET `trainer_class` = 8
WHERE `entry` = 80213;  -- Magistrix Ishalah, Portal Trainer
