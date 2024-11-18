#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <memory>
#include <algorithm>
#include <random>
#include <unordered_map>
#include "Game.h"
#include "TextDisplay.h"

using namespace std;

const int maxOfEachAbility = 2;

string randomLinkGenerator();
void areAbilitiesValid(const string &abilities, const string &validAbilities);
string retrieveLink(fstream &file);


int main(int argc, const char* argv[]){
    const string insufficentArguments = "Insufficient arguments provided";
    string validAblities = "LFDSP";
    string p1AbilityOrder = "LFDSP", p2AbilityOrder = "LFDSP";
    string linkOrder1 = randomLinkGenerator(), linkOrder2 = randomLinkGenerator();
    bool graphicsEnabled = false;
    try { 
        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];
            if (arg == "-ability1") {
                ++i;
                if (i >= argc) throw out_of_range(insufficentArguments);
                p1AbilityOrder = string(argv[i]);
                areAbilitiesValid(p1AbilityOrder, validAblities);
            } else if (arg == "-ability2") {
                ++i;
                if (i >= argc) throw out_of_range(insufficentArguments);
                p2AbilityOrder = string(argv[i]);
                areAbilitiesValid(p2AbilityOrder, validAblities);                
            } else if (arg == "-link1") {
                ++i;
                if (i >= argc) throw out_of_range(insufficentArguments);
                ifstream file{argv[i]};
                linkOrder1 = retrieveLink(file);
            } else if (arg == "-link2") {
                ++i;
                if (i >= argc) throw out_of_range(insufficentArguments);
                ifstream file{argv[i]};
                linkOrder2 = retrieveLink(file);
            } else if (arg == "graphics") {
                graphicsEnabled = true;
            } else {
                throw invalid_argument("Invalid command-line argument provided");
            }
        }
    } catch(exception &e) {
        cerr << e.what() << endl;
        return 1;
    }
}

string randomLinkGenerator() {
    vector<string> links = {
        "D1", "D2", "D3", "D4",
        "V1", "V2", "V3", "V4"
    };
    random_device rd;  
    mt19937 rng(rd()); 
    shuffle(links.begin(), links.end(), rng);
    string linkOrder;
    for (const auto& link : links) {
        linkOrder += link;
    }
    return linkOrder;
}

void areAbilitiesValid(const string &s, const string &validAbilities) {
    unordered_map<char, int> mp;
    for (int i = 0; i < validAbilities.size(); ++i) {
        mp[validAbilities[i]] = 0;
    }
    for (int i = 0; i < s.size(); ++i) {
        char c = toupper(s[i]);
        if (mp.find(c) != mp.end() && mp[c] < maxOfEachAbility) {
            ++mp[c];
        } else {
            throw invalid_argument("Invalid abilities entered");
        }
    }
}

string retrieveLink(ifstream &file) {
    string errorMessage = "Invalid links provided";
    const int requiredCount = 1;
    vector<string> links = {
        "D1", "D2", "D3", "D4",
        "V1", "V2", "V3", "V4"
    };
    unordered_map<string, int> mp;
    for (int i = 0; i < links.size(); ++i) {
        mp[links[i]] = 0;
    }
    string s, linkOrder;
    while (file >> s) {
        transform(s.begin(), s.end(), s.begin(), [] (unsigned char c) {
            return toupper(c);
        });
        if (mp.find(s) != mp.end()) {
            ++mp[s];
            linkOrder += s;
        } else {
            throw invalid_argument(errorMessage);
        }
    }
    for (const auto &elem : mp) {
        if (elem.second != requiredCount) {
            throw invalid_argument(errorMessage);
        }
    }
    return linkOrder;
}
