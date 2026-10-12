#ifndef DEF_TIMBERMAW_HOLD_H
#define DEF_TIMBERMAW_HOLD_H

enum TimbermawHoldEncounters
{
    TYPE_KARRSH            = 1,
    TYPE_BEAR_DUO          = 2,
    TYPE_ORMANOS           = 3,
    TYPE_KRONN            = 4,
    TYPE_LOKTANAG          = 5,
    TYPE_TRIOCH            = 6,
    TYPE_SELENAXX          = 7,
    TYPE_PARTATH           = 8,
    TYPE_URSOL             = 9,
    TYPE_PEROTHARN         = 10,
    MAX_ENCOUNTER          = 11
};

enum TimbermawHoldData
{
    DATA_BARRIER_OF_URSOL  = 12,
    DATA_BRAZIER_COUNT     = 13,
    DATA_DAGGER_CLAIMED    = 14
};

enum TimbermawHoldCreatures
{
    // Bosses
    NPC_KARRSH_THE_SENTINEL      = 62934,
    NPC_ROTGROWL                 = 62936,
    NPC_KODIAK                   = 62937,
    NPC_ORMANOS_THE_CRACKED      = 62935,
    NPC_ARCHDRUID_KRONN          = 62938,
    NPC_LOKTANAG_THE_VILE        = 2139,
    NPC_TRIOCH_THE_DEVOURER      = 62946,
    NPC_SELENAXX_FOULHEART       = 62940,
    NPC_CHIEFTAIN_PARTATH        = 62941,
    NPC_URSOL                    = 62947,
    NPC_PEROTHARN                = 60686,

    // Encounter Adds & Summons
    NPC_SON_OF_URSOL             = 62885,
    NPC_TREMOR_OF_ORMANOS        = 51608,
    NPC_SON_OF_ORMANOS           = 51609,
    NPC_CORRUPTION_OF_LOKTANAG   = 62879,
    NPC_TAINTED_MASS             = 62878,
    NPC_SIPHON_TOTEM             = 62950,
    NPC_EYE_OF_PEROTHARN         = 62951,
    NPC_NIGHTMARE_VOID_FISSURE   = 62952,

    // Trash Mobs
    NPC_WITHERMAW_PATHFINDER     = 62872,
    NPC_WITHERMAW_TRACKER        = 62875,
    NPC_WITHERMAW_DEN_WATCHER    = 62876,
    NPC_WITHERMAW_DEFILER        = 62870,
    NPC_WITHERMAW_SHAMAN         = 62871,
    NPC_WITHERMAW_TOTEMIC        = 62873,
    NPC_ENRAGED_WITHERMAW        = 62867,
    NPC_WITHERMAW_URSA           = 62877,
    NPC_VILE_SKITTERER           = 62874,
    NPC_FOULHEART_DECEIVER       = 62881,
    NPC_FOULHEART_SATYR          = 62882,
    NPC_FOULHEART_TRICKSTER      = 62883,
    NPC_FOULHEART_HELLCALLER     = 62884
};

enum TimbermawHoldGameObjects
{
    GO_BRAZIER_CORRUPTION        = 300602,
    GO_BRAZIER_DEFILEMENT        = 300603,
    GO_BRAZIER_DECAY             = 300604,
    GO_BRAZIER_PUTRIFICATION     = 300605,
    GO_BARRIER_OF_URSOL          = 300606,
    GO_NIGHTMARE_RIFT            = 300610,
    GO_LEGENDARY_DAGGER          = 300611
};

#endif
