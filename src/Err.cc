#include "err.h"

string Err::insufficientArgs = "Insufficent arguments provided";
string Err::invalidArg = "Invalid arguments provided";
string Err::invalidCommandLineArg = 
    "Invalid command-line arguments provided\nArguments must be of one of the forms: [-ability1 <abilities>] [-ability2 <abilities>] [-link1 <placementFile>] [-link2 <placementFile] [graphics]";
string Err::invalidAbilities = "Abilities can only be of the form [L] [F] [D] [S] [P], and you may have a total of 5 and a maximum of 2 of each ability";
string Err::invalidLinks = "Links may only be of the form [D1] [D2] [D3] [D4] [V1] [V2] [V3] [V4], and there must be exactly one of each form";
