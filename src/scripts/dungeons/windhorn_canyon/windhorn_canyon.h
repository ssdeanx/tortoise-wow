#ifndef DEF_WINDHORN_CANYON_H
#define DEF_WINDHORN_CANYON_H

enum WindhornCanyonEncounters
{
    DATA_NARLGOM               = 1,
    DATA_SHALK                 = 2,
    DATA_PATHUN                = 3,
    DATA_VORTALUS              = 4,
    DATA_WALGAN                = 5,
    DATA_STORMHOOF             = 6,
    DATA_MAX_ENCOUNTER         = 7
};

enum WindhornCanyonCreatureIds
{
    NPC_PATHUN_DUSKHIDE        = 62779,
    NPC_BONESPEAKER_NARLGOM    = 62780,
    NPC_PROPHET_STORMHOOF      = 62781,
    NPC_CHIEFTAIN_SHALK        = 62782,
    NPC_AMBASSADOR_VORTALUS    = 62783,
    NPC_WALGAN_BLOODCALLER     = 62784,
    NPC_CHAMPION_ROTAG         = 62785,
    NPC_BLACKWIND_BLOODGUARD   = 62771,
    NPC_STORM_GUARDIAN         = 62865,
    NPC_STORM_RESIDUE          = 62866,
};

enum WindhornCanyonGameObjects
{
    GO_GHOST_GATE              = 180322,
};

enum WindhornCanyonSpellIds
{
    SPELL_BONE_ARMOR                 = 11445,
    SPELL_FLAME_OF_SHALK             = 41121,
    SPELL_SUMMON_ROTAG               = 42004,
    SPELL_TARGETED_ARCANE_CHANNELING = 51187,
    SPELL_RAIN_OF_FIRE               = 57747,

    // Boss spells
    SPELL_MULTI_SHOT                 = 14443,
    SPELL_CONCUSSIVE_SHOT            = 17174,
    SPELL_RAPTOR_STRIKE              = 14261,
    SPELL_SHADOW_BOLT                = 15232,
    SPELL_IMMOLATE                   = 11668,
    SPELL_FEAR                       = 12542,
    SPELL_MORTAL_STRIKE              = 16856,
    SPELL_WHIRLWIND                  = 15589,
    SPELL_BLOODLUST                  = 6742,
    SPELL_LIGHTNING_BOLT             = 15234,
    SPELL_EARTH_SHOCK                = 15501,
    SPELL_CHAIN_LIGHTNING            = 16006
};

enum WindhornCanyonScriptTexts
{
    SAY_PATHUN_AGGRO              = -1999929,
    SAY_PATHUN_HALF_HEALTH        = -1999930,
    SAY_PATHUN_DEATH              = -1999931,
    SAY_VORTALUS_AGGRO            = -1999932,
    SAY_VORTALUS_HALF_HEALTH      = -1999933,
    SAY_VORTALUS_DEATH            = -1999934,
    SAY_WALGAN_AGGRO              = -1999935,
    SAY_WALGAN_HALF_HEALTH        = -1999936,
    SAY_WALGAN_DEATH              = -1999937,
    SAY_NARLGOM_AGGRO             = -1999938,
    SAY_NARLGOM_SUMMON_ROTAG      = -1999939,
    SAY_SPIRIT_OF_ROTAG_SPAWN     = -1999940,
    SAY_NARLGOM_DEATH             = -1999941,
    SAY_STORMHOOF_AGGRO           = -1999942,
    SAY_STORMHOOF_HALF_HEALTH     = -1999943,
    SAY_STORMHOOF_DEATH           = -1999944,
    SAY_SHALK_AGGRO               = -1999945,
    SAY_SHALK_HALF_HEALTH         = -1999946,
    SAY_SHALK_BLOODGUARD_SPAWN    = -1999947,
    SAY_SHALK_DEATH               = -1999948,
};

#endif
