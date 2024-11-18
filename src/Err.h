#ifndef ERR_H
#define ERR_H

#include <string>

using namespace std;

class Err {
    public:
    static string insufficientArgs;
    static string invalidArg;
    static string invalidCommandLineArg;
    static string invalidAbilities;
    static string invalidLinks;
};

#endif
