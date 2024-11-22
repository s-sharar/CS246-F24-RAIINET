#include "TextDisplay.h"
#include <string>
#include <sstream>
#include <iostream>

const int player1 = 1;
const int player2 = 2;
const int numberOfLines = 2;
const int numberOfLinksPerLine = 4;
const char p1Firewall = 'm';
const char p2Firewall = 'w';
const char p1BaseChar = 'a';
const char p2BaseChar = 'A';
const char serverPort = 'S';

using namespace std;

TextObserver::TextObserver(weak_ptr<Game> &g, ostream &out, int playerNumber) : g{g}, out{out}, playerNumber{playerNumber} {
}

void TextObserver::notify() {
    shared_ptr<Game> game;
    if (!(game = g.lock())) return;
    if (playerNumber == player1) {
        out << "Player " << playerNumber << ":" << endl;
        auto &player = game->getPlayer(playerNumber);
        out << "Downloaded: " << player->getDataDownloaded() << "D, " << player->getVirusDownloaded() << 'V' << endl;
        out << "Abilities: " << player->getRemainingAbilities() << endl;
        for (int i = 0; i < numberOfLines; ++i) {
            for (int j = 0; j < numberOfLinksPerLine; ++j) {
                char toPrint = p1BaseChar + i * numberOfLinksPerLine + j;
                shared_ptr<Link> currLink = player->getLink(toPrint, player1);
                string type = currLink->getType() == LinkType::Data ? "D" : "V";
                type += to_string(currLink->getStrength()); 
                out << toPrint << ": " << type << " ";
            }
            out << endl;
        }
        auto board = game->getBoard();
        for (int i = 0; i < board->getSize(); ++i) {
            out << "=";
        }
        out << endl;
        for (int i = 0; i < board->getSize(); ++i) {
            for (int j = 0; j < board->getSize(); ++j) {
                Cell &cell = board->getCell(i, j);
                if (cell.isServerPort()) {
                    out << serverPort;
                } else if (!cell.isEmpty()) {
                    out << cell.getContent();
                } else if (cell.hasOwnFirewall(player1)){
                    out << p1Firewall;
                } else {
                    out << cell.getContent();
                }
            }
            out << endl;
        }
        for (int i = 0; i < board->getSize(); ++i) {
            out << "=";
        }
        out << endl;
        out << "Player " << player2 << ":" << endl;
        auto &opp = game->getPlayer(player2);
        out << "Downloaded: " << opp->getDataDownloaded() << "D, " << opp->getVirusDownloaded() << 'V' << endl;
        out << "Abilities: " << opp->getRemainingAbilities() << endl;
        for (int i = 0; i < numberOfLines; ++i) {
            for (int j = 0; j < numberOfLinksPerLine; ++j) {
                char toPrint = p2BaseChar + i * numberOfLinksPerLine + j;
                shared_ptr<Link> currLink = opp->getLink(toPrint, player2);
                string type = "?";
                if (currLink->getIsVisible()) {
                    type = currLink->getType() == LinkType :: Data ? "D" : "V";
                    type += to_string(currLink->getStrength());
                }
                out << toPrint << ": " << type << " ";
            }
            out << endl;
        }
        out << endl;
    } else {
        out << "Player " << playerNumber << ":" << endl;
        auto &player = game->getPlayer(playerNumber);
        out << "Downloaded: " << player->getDataDownloaded() << "D, " << player->getVirusDownloaded() << 'V' << endl;
        out << "Abilities: " << player->getRemainingAbilities() << endl;
        for (int i = 0; i < numberOfLines; ++i) {
            for (int j = 0; j < numberOfLinksPerLine; ++j) {
                char toPrint = p2BaseChar + i * numberOfLinksPerLine + j;
                shared_ptr<Link> currLink = player->getLink(toPrint, player2);
                string type = currLink->getType() == LinkType::Data ? "D" : "V";
                type += to_string(currLink->getStrength()); 
                out << toPrint << ": " << type << " ";
            }
            out << endl;
        }
        auto board = game->getBoard();
        for (int i = 0; i < board->getSize(); ++i) {
            out << "=";
        }
        out << endl;
        for (int i = board->getSize() - 1; i >= 0; --i) {
            for (int j = 0; j < board->getSize(); ++j) {
                Cell &cell = board->getCell(i, j);
                if (cell.isServerPort()) {
                    out << serverPort;
                } else if (!cell.isEmpty()) {
                    out << cell.getContent();
                } else if (cell.hasOwnFirewall(player2)){
                    out << p2Firewall;
                } else {
                    out << cell.getContent();
                }
            }
            out << endl;
        }
        for (int i = 0; i < board->getSize(); ++i) {
            out << "=";
        }
        out << endl;
        out << "Player " << player1 << ":" << endl;
        auto &opp = game->getPlayer(player1);
        out << "Downloaded: " << opp->getDataDownloaded() << "D, " << opp->getVirusDownloaded() << 'V' << endl;
        out << "Abilities: " << opp->getRemainingAbilities() << endl;
        for (int i = 0; i < numberOfLines; ++i) {
            for (int j = 0; j < numberOfLinksPerLine; ++j) {
                char toPrint = p1BaseChar + i * numberOfLinksPerLine + j;
                shared_ptr<Link> currLink = opp->getLink(toPrint, player1);
                string type = "?";
                if (currLink->getIsVisible()) {
                    type = currLink->getType() == LinkType :: Data ? "D" : "V";
                    type += to_string(currLink->getStrength());
                }
                out << toPrint << ": " << type << " ";
            }
            out << endl;
        }
        out << endl;
    }
}
