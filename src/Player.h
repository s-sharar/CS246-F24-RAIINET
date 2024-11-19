#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <memory>
#include "Cell.h"

using namespace std;

class Link;
class Ability;

class Player {
    int id;
    vector<shared_ptr<Link>> links;
    vector<shared_ptr<Link>> downloadedLinks;
    vector<shared_ptr<Ability>> abilities;
    int numOfDataDownloaded, numOfVirusDownloaded, abilityCount;
    public:
    Player(const string &links,const string &abilities, int id);
    string getAbilityName(int i);
    void useAbility(int i);
    void download(shared_ptr<Link> &l);
    int getDataDownloaded() const;
    int getVirusDownloaded() const;
};

#endif
