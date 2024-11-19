#ifndef ERR_H
#define ERR_H

#include <string>

using namespace std;

class Err {
    public:
    static string insufficientArgs;
    static string invalidArg;
    static string invalidCommandLineArg;
    static string Err::invalidRuntimeCommand;
    static string invalidAbilities;
    static string invalidAbility;
    static string invalidAbilityIndex;
    static string invalidLinks;
    static string invalidFile;
    static string abilityUsedThisTurn;
    static string reenterCommand;
};

#endif
