#include "Player.h"
#include <iostream>
#include "Link.h"
#include "Ability.h"
#include "Err.h"

using namespace std;

const int individualLinkLength = 2;
const int serverPortStart = 6;
const int serverPortEnd = 10;
const int p1BaseRow = 0;
const int p2BaseRow = 7;

Player::Player(const string &linksString, const string &abilitiesString, int id) : id{id}, numOfDataDownloaded{0}, numOfVirusDownloaded{0}, abilityCount{static_cast<int>(abilitiesString.length())} {
    // Assume links and abilities passed are valid since they are checked in main.
    char baseChar = (id == 1) ? 'a' : 'A';
    int row = (id == 1) ? p1BaseRow : p2BaseRow;
    int col = 0;
    for (size_t i = 0; i < linksString.length(); i += individualLinkLength) {
        bool isServerPort = i >= serverPortStart && i < serverPortEnd;
        LinkType linkType = linksString[i] == 'D' ? LinkType::Data : LinkType::Virus;
        int strength = linksString[i + 1] - '0';
        char linkID = static_cast<char>(baseChar + col);
        int linkRow = row;
        if (isServerPort && id == 1) {
            ++linkRow;
        } else if (isServerPort) {
            --linkRow;
        }
        links.emplace_back(make_shared<Link>(linkID, linkType, strength, linkRow, col));
        ++col;
    }
    for (size_t i = 0; i < abilities.size(); ++i) {
        abilities.emplace_back(make_shared<Ability>(abilitiesString[i], i + 1));
    }
}

string Player::getAbilityName(int i) {
    return abilities[i - 1]->getAbilityName();
}

void Player::useAbility(int i) {
    abilities[i]->useAbility();
    --abilityCount;
}

void Player::download(shared_ptr<Link> &l) {
    if (l->getType() == LinkType::Data) {
        ++numOfDataDownloaded;
    } else {
        ++numOfVirusDownloaded;
    }
}

int Player::getDataDownloaded() const {
    return numOfDataDownloaded;
}

int Player::getVirusDownloaded() const {
    return numOfVirusDownloaded;
}

bool Player::getEliminated() const {
    return eliminated;
}

int Player::getRemainingAbilities() const {
    return abilityCount;
}

void Player::setEliminated(bool b) {
    eliminated = b;
}

shared_ptr<Ability> Player::getAbility(int abilityNumber) {
    if (abilityNumber > static_cast<int>(abilities.size())) throw runtime_error(Err::invalidAbilityIndex);
    return abilities[abilityNumber - 1];
}

shared_ptr<Link> Player::getLink(char c, int playerNumber) {
    char base = playerNumber == 1 ? 'a' : 'A';
    int index = static_cast<int> (c - base);
    return links[index];
}
