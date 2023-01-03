#include "level.hpp"

const char* Levels(int& entryId, enemy::EnemyDir& firstDir, int map)
{
    if (map == 1)
    {
        entryId = 40;

        firstDir = enemy::EnemyDir::RIGHT;

        return 
        "-#--------o-#-#--o--"
        "#---#--#---RRRRRR---"
        "RRRoRRRRRRRR--#-Do--"
        "--R-R-o-#--R--##D---"
        "--RRRRRRR--D#-RRd-o-"
        "-#--D-#-R#-D-#R-R---"
        "----DDRRR-oRRRR#RDRE"
        "--o---#----o-----#--"
        "--------#-----#-----";
    }

    if (map == 2)
    {
    entryId = 162;

    firstDir = enemy::EnemyDir::UP;

    return 
    "#----#-----#----#---"
    "--#DRRRRrRRRRDDRRRRR"
    "-RDD----Ro----o#---R"
    "#R---#--RRDDDRR-#-RR"
    "#R##----##--o-R--#R-"
    "-RRRRRRRRRRRo-R--#E-"
    "-#R-#-----#RRRR--###"
    "--R----#----#--#####"
    "--R#-#---#-#--######";
    }

    if (map == 3)
    {
    entryId = 6;

    firstDir = enemy::EnemyDir::DOWN;

    return 
    "-#-o#-R--#__________"
    "--RRRRR-RDDORRRRrRRE"
    "#-R-##--R_DRR__OR__O"
    "--RRRRRRR____O__R_O_"
    "-#--#--OR_O_____R_O_"
    "#----ORRRRRRRRRRRO__"
    "--__O_R_RO___O_OR_O_"
    "_O_O__RRR____RRRR___"
    "___O___O___O_E___O__";
    }

    return "\0";
}