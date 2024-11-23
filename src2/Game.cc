#include "Game.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <compare>
#include <random>
#include "Err.h"
#include "Ability.h"

using namespace std;

const int maxAbilities = 5;
const int numCol = 8;

const int player1 = 1;
const int player2 = 2;
const int player3 = 3;
const int player4 = 4;

const int gameOverCondition = 4;

bool in(int x, int l, int u);

bool validPiLink(char link, int p);

bool validP1Link(char link);

bool validP2Link(char link);

bool validP3Link(char link);

bool validP4Link(char link);

bool validLink(char link);

bool validDirection(const string &direction);

void getNewCords(int &row, int &col, shared_ptr<Link> &moveLink, const string &direction);


Game::Game(int playerCount, const vector<string> &linkOrders, const vector<string> &abilities, bool graphicsEnabled) : playerCount{playerCount}, activePlayers{playerCount}, graphicsEnabled{graphicsEnabled} {
    int size = (playerCount == 2) ? 8 : 10;
    board = make_shared<Board>(size);
    for (int i = 0; i < playerCount; ++i) {
        players.emplace_back(make_shared<Player>(linkOrders[i], size, abilities[i], i + 1));
    }
}

string Game::getAbilityName(int i) {
    // check ability index here
    if (i < 1 || i > maxAbilities) throw runtime_error(Err::invalidAbilityIndex);
    return players[currentTurn - 1]->getAbilityName(i);
}

// update useAbility to check if already used ability before calling each function
void Game::useAbility(int abilityNumber, int row, int col) {
    const int numRow = (playerCount == 2) ? 8 : 10;
    int playerIndex = currentTurn - 1;
    shared_ptr<Ability> ability = players[playerIndex]->getAbility(abilityNumber);
    if (ability->getIsActivated()) throw runtime_error(Err::abilityAlreadyUsed(ability->getAbilityName(), ability->getAbilityID()));
    if (!(row >= 0 && row < numRow && col >= 0 && col < numCol)) throw out_of_range(Err::invalidCoordinates);
    useFirewall(row, col);
    ability->useAbility();
    checkGameOver();
    //notifyObservers();
}

void Game::useAbility(int abilityNumber, const string &abilityName, char link) {
    int playerIndex = currentTurn - 1;
    shared_ptr<Ability> ability = players[playerIndex]->getAbility(abilityNumber);
    if (ability->getIsActivated()) throw runtime_error(Err::abilityAlreadyUsed(ability->getAbilityName(), ability->getAbilityID()));
    if (!validLink(link)) throw runtime_error(Err::invalidLink);
    if (abilityName == "LinkBoost") {
        useLinkBoost(link);
    } else if (abilityName == "Download") {
        useDownload(link);
    } else if (abilityName == "Polarise") {
        usePolarise(link);
    } else if (abilityName == "Scan") {
        useScan(link);
    } else if (abilityName == "Restore") {
        useRestore(link);
    }
    ability->useAbility();
    checkGameOver();
    //notifyObservers();
}

void Game::useAbility(int abilityNumber, int r1, int c1, int r2, int c2) {
    int playerIndex = currentTurn - 1;
    const int numRow = (playerCount == 2) ? 8 : 10;
    shared_ptr<Ability> ability = players[playerIndex]->getAbility(abilityNumber);
    if (ability->getIsActivated()) throw runtime_error(Err::abilityAlreadyUsed(ability->getAbilityName(), ability->getAbilityID()));
    if (!(r1 >= 0 && r1 < numRow && c1 >= 0 && c1 < numCol)) throw out_of_range(Err::invalidCoordinates);
    if (!(r2 >= 0 && r2 < numRow && c2 >= 0 && c2 < numCol)) throw out_of_range(Err::invalidCoordinates);

    useTeleport(r1, c1, r2, c2);
    ability->useAbility();
}

void Game::move(char link, const string &direction) {
    // basic validations
    if (!validLink(link)) throw runtime_error(Err::invalidLink);
    if (!validDirection(direction)) throw runtime_error(Err::invalidDirection);
    if (!validPiLink(link, currentTurn)) throw runtime_error(Err::cannotMoveOpponentsLink);
    cout << link << endl;

    int playerIndex = currentTurn - 1;
    cout << "player index: " << playerIndex << endl;

    shared_ptr<Link> currLink = players[playerIndex]->getLink(link, currentTurn);
    
    // validating if moving a downloaded/trapped link
    if (currLink->getIsDownloaded()) throw runtime_error(Err::cannotMoveDownloadedLink);
    // TODO: check if trying to move a trapped link
    
    // get cords of cell we're moving to
    int newRow = 0, newCol = 0;
    getNewCords(newRow, newCol, currLink, direction);

    cout << newRow << ',' << newCol << endl;
    cout << currLink->getId() << endl;

    // check in bounds
    Cell &oldCell = board->getCell(currLink->getRow(), currLink->getCol());
    // valid iff player moves off opponents edge of the board
    bool validOB = validOutOfBounds(newRow, newCol); // throws error if not valid bounds
    if (validOB) {
        oldCell.setContent('.');
        currLink->download();
        players[playerIndex]->download(currLink);
        checkGameOver();
        // notifyObservers();
        return;
    }
    // if bounds are valid, get the cells and see if it's self server port or contains self link
    Cell &newCell = board->getCell(newRow, newCol);

    // locked check
    if (newCell.isLocked()) throw runtime_error(Err::notInBounds); 
    // self server port
    if (newCell.isOwnServerPort(currentTurn)) throw runtime_error(Err::cannotDownloadOwnLink(true));
    // self link
    if (validPiLink(newCell.getContent(), currentTurn)) throw runtime_error(Err::cannotMoveOntoOwnLink);
    // imprisoned link on cell
    // if (newCell.imprisonCell() && newCell.getImprisonCount() > 0) throw runtime_error(Err::cannotMoveOntoImprisonedSquare);

    // Validations done
    
    // set the cell that the link is being moved from to '.'
    oldCell.setContent('.');

    // moving into an opponent's server port
    if (newCell.isServerPort()) {
        int opponentIndex = newCell.getPlayersServerPort() - 1;
        if (!(players[opponentIndex]->getEliminated())) { // only download if player not eliminated
            currLink->download();
            currLink->setDownloadedBy(opponentIndex + 1);
            players[opponentIndex]->download(currLink);
        } else { // moving onto serverport of eliminated player
            newCell.setContent(currLink->getId());
            currLink->setRow(newRow);
            currLink->setCol(newCol);
        }
    } 
    
    // moving into a firewall
    else if (newCell.hasFirewall()) {
        // if self firewall
        if (newCell.hasOwnFirewall(currentTurn)) {
            if (newCell.isEmpty()) {
                newCell.setContent(currLink->getId());
                currLink->setRow(newRow);
                currLink->setCol(newCol);
            } else {
                char content = newCell.getContent();
                int opponentIndex;
                for (int i = 0; i < playerCount; ++i) {
                    if (validPiLink(content, i + 1)) opponentIndex = i;
                }
                if (!(players[opponentIndex]->getEliminated())) { // if moving onto link of non-eliminated player
                    shared_ptr<Link> opponentLink = players[opponentIndex]->getLink(content, opponentIndex + 1);
                    battle(currLink, opponentLink, opponentIndex, newCell);
                    // maybe additional logic
                } else { // if moving onto link of eliminated player
                    newCell.setContent(currLink->getId());
                    currLink->setRow(newRow);
                    currLink->setCol(newCol);
                }
                // maybe additional logic
            }
        }
        else {
            // opponent's firewall. If link is virus, then currentTurn player downloads it
            if (currLink->getType() == LinkType::Virus) {
                players[playerIndex]->download(currLink);
                currLink->download();
                currLink->setDownloadedBy(currentTurn);
            } else {
                currLink->setIsVisible(true);
                if (newCell.isEmpty()) {
                    newCell.setContent(currLink->getId());
                    currLink->setRow(newRow);
                    currLink->setCol(newCol);
                } else {
                    char content = newCell.getContent();
                    int opponentIndex;
                    for (int i = 0; i < playerCount; ++i) {
                        if (validPiLink(content, i + 1)) opponentIndex = i;
                    }
                    
                    if (!(players[opponentIndex]->getEliminated())) { // if moving onto link of non-eliminated player
                        shared_ptr<Link> opponentLink = players[opponentIndex]->getLink(content, opponentIndex + 1);
                        battle(currLink, opponentLink, opponentIndex, newCell);
                    
                    } else { // if moving onto link of eliminated player
                        newCell.setContent(currLink->getId());
                        currLink->setRow(newRow);
                        currLink->setCol(newCol);
                    }
                    // maybe additional logic
                }
            }
        }
    
    // moving into a cell contain an opponents link
    } else if (!newCell.isEmpty()) {
        char content = newCell.getContent();
        int opponentIndex;
        for (int i = 0; i < playerCount; ++i) {
            if (validPiLink(content, i + 1)) opponentIndex = i;
        }

        if (!(players[opponentIndex]->getEliminated())) { // if moving onto link of non-eliminated player
            shared_ptr<Link> opponentLink = players[opponentIndex]->getLink(content, opponentIndex + 1);
            battle(currLink, opponentLink, opponentIndex, newCell);
        } else { // if moving onto link of eliminated player
            newCell.setContent(currLink->getId());
            currLink->setRow(newRow);
            currLink->setCol(newCol);
        }
        // maybe additional logic
    }

    // moving into cell with a teleport on it
    else if (newCell.getTeleport()) {
        int teleportToRow = newCell.getTeleportRow();
        int teleportToCol = newCell.getTeleportCol();

        Cell &teleportToCell = board->getCell(teleportToRow, teleportToCol);
        teleportToCell.setContent(currLink->getId());
        newCell.setTeleport(false);
        teleportToCell.setTeleport(false);
        currLink->setRow(teleportToRow);
        currLink->setCol(teleportToCol);
    }

    // moving into cell with a imprison on it
    // else if (newCell.getImprisonCell()) {
    //     newCell.setImprisonCount(3);
    // }

    // else moving into an empty cell 
    else {
        newCell.setContent(currLink->getId());
        currLink->setRow(newRow);
        currLink->setCol(newCol);
    }

    // finds next player turn, who is currently active
    for (int i = 0; i < playerCount; ++i) {
        currentTurn = (currentTurn % playerCount) + 1;
        if (isActive(currentTurn)) break;
    }

    for (int i = 0; i < board->getSize(); ++i) {
        for (int j = 0; j < board->getSize(); ++j) {
            Cell &currCell = board->getCell(i,j);
            if (currCell.getImprisonCount() != 0)  currCell.decrementImprisonCount();
        }
    }
    checkGameOver();
    // notifyObservers();
}

void Game::useFirewall(int row, int col) {
    Cell &cell = board->getCell(row, col);
    // check if trying to place Firewall on a server port
    if (cell.isServerPort()) throw runtime_error(Err::cannotPlaceAbilityonSP("Firewall"));
    // check if trying to place firewall on a firewall
    if (cell.hasFirewall()) throw runtime_error(Err::cannotPlaceAbilityonFW("Firewall"));
    // TODO: check for teleport, trap
    // assuming we cannot place a firewall directly onto our opponents links
    for (int i = 0; i < playerCount; ++i) {
        if (i + 1 != currentTurn) {
            if (validPiLink(cell.getContent(), i + 1)) throw runtime_error(Err::cannotPlaceAbilityDirectlyOnOpp("Firewall"));
        }
    } 
    
    cell.setFirewall(currentTurn, true);
}

void Game::useRestore(char link) {
    // this is the person who owns the downloaded link
    int linkOnwerIndex;
    for (int i = 0; i < playerCount; ++i) {
        if (validPiLink(link, i+i)) linkOnwerIndex = i;
    }

    shared_ptr<Link> currLink = players[linkOnwerIndex]->getLink(link, linkOnwerIndex + 1);

    // this ability can only be used on a downloaded link
    if (!currLink->getIsDownloaded()) {
        // throw runtime error (can only be used with downloaded links)
    }

    // this is the person that downloaded the link
    int restoredLinkPlayerIndex = currLink->getDownloadedBy() - 1;

    // decrement the counts
    // Restored link is either a data or a virus
    if (currLink->getType() == LinkType::Virus) {
        int virusDownloaded = players[restoredLinkPlayerIndex]->getVirusDownloaded();
        players[restoredLinkPlayerIndex]->setVirusDownloaded(virusDownloaded - 1);
    }
    else {
        int dataDownloaded = players[restoredLinkPlayerIndex]->getDataDownloaded();
        players[restoredLinkPlayerIndex]->setVirusDownloaded(dataDownloaded - 1);
    }
    // reset status of link
    currLink->undownload();

    // find new cords randomly
    const int numRow = (playerCount == 2) ? 8 : 10;
    int restoredRow;
    int restoredCol;
    bool foundValidCords = false;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomCol(0, numCol);
    uniform_int_distribution<> randomRow(0, numRow);

    while (!foundValidCords) {
        // gets a random number between 0 and numCol/numRow
        restoredRow = randomRow(gen);
        restoredCol = randomCol(gen);

        // it only cannot be a cell containing the link owner's links and cannot be locked as well
        Cell &cell = board->getCell(restoredRow, restoredCol);
        if (!(cell.isLocked() || validPiLink(cell.getContent(), linkOnwerIndex + 1))) {
            foundValidCords = true;
        }
    }

    Cell &restoredCell = board->getCell(restoredRow, restoredCol);
    // now assume new cords is a valid cell that doesn't contain linkOwner's links

    // TODO: clean this up, it's so hard to read

    // new cords is a firewall
    if (restoredCell.hasFirewall()) {
        // either linkOwner's or opp's firewall
        if (restoredCell.hasOpponnentFirewall(linkOnwerIndex + 1)) {
            if (currLink->getType() == LinkType::Virus) {
                currLink->download();
                players[linkOnwerIndex]->download(currLink);
            }
            // currLink is a data
            else {
                if (restoredCell.isEmpty()) {
                    restoredCell.setContent(currLink->getId());
                    currLink->setRow(restoredRow);
                    currLink->setCol(restoredCol);
                }
                // if not empty then battle
                else {
                    char content = restoredCell.getContent();
                    int opponentIndex;
                    for (int i = 0; i < playerCount; ++i) {
                        if (validPiLink(content, i + 1)) opponentIndex = i;
                    }
                    shared_ptr<Link> opponentLink = players[opponentIndex]->getLink(content, opponentIndex + 1);
                    battle(currLink, opponentLink, opponentIndex, restoredCell);
                }
            }
        }
        else if (restoredCell.isEmpty()) {
                restoredCell.setContent(currLink->getId());
                currLink->setRow(restoredRow);
                currLink->setCol(restoredCol);
            }
        // else battle
        else {
            char content = restoredCell.getContent();
            int opponentIndex;
            for (int i = 0; i < playerCount; ++i) {
                if (validPiLink(content, i + 1)) opponentIndex = i;
            }
            shared_ptr<Link> opponentLink = players[opponentIndex]->getLink(content, opponentIndex + 1);
            battle(currLink, opponentLink, opponentIndex, restoredCell);
        }
    }
    // if new cords contain a teleport
    else if (restoredCell.getTeleport()) {
        int teleportRow = restoredCell.getTeleportRow();
        int teleportCol = restoredCell.getTeleportCol();
        board->getCell(teleportRow, teleportCol).setContent(currLink->getId());
        currLink->setCol(teleportCol);
        currLink->setRow(teleportRow);
    }
    else if (restoredCell.isServerPort()) {
        int playerPort = restoredCell.getPlayersServerPort();
        currLink->download();
        players[playerPort - 1]->download(currLink);
    }
    else if (false) {
        // TODO: if cell has an unactivated trap. If trap is activated with a link in it, then it will just battle
    }
    // if new cord contains opp link
    else if (!restoredCell.isEmpty()) {
        char content = restoredCell.getContent();
        int opponentIndex;
        for (int i = 0; i < playerCount; ++i) {
            if (validPiLink(content, i + 1)) opponentIndex = i;
        }
        shared_ptr<Link> opponentLink = players[opponentIndex]->getLink(content, opponentIndex + 1);
        battle(currLink, opponentLink, opponentIndex, restoredCell);
    }
    // else it is empty
    else {
        restoredCell.setContent(currLink->getId());
        currLink->setCol(restoredCell.getCol());
        currLink->setRow(restoredCell.getRow());
    }
}


void Game::useScan(char l) {
    // if player tries to scan their own link, return error
    if (validPiLink(l, currentTurn)) {
        throw runtime_error(Err::cannotUseAbilityOnOwnLink("Scan"));
    }

    // get player indices
    int otherPlayerIndex;
    
    // get the link to be scanned and execute the download
    for (int i = 0; i < playerCount; ++i) {
        if ((i + 1 != currentTurn) && (validPiLink(l, i + 1))) otherPlayerIndex = i;
    }

    shared_ptr<Link> currLink = players[otherPlayerIndex]->getLink(l, otherPlayerIndex + 1);

    // if downloaded already visible
    currLink->scan();
}

void Game::useDownload(char link) {
    // if player tries to download their own link, return error
    if (validPiLink(link, currentTurn)) throw runtime_error(Err::cannotUseAbilityOnOwnLink("Download"));
    
    // get player indices
    int playerIndex = currentTurn - 1;
    int otherPlayerIndex;
    // get the link to be scanned and execute the download
    for (int i = 0; i < playerCount; ++i) {
        if ((i + 1 != currentTurn) && (validPiLink(link, i + 1))) otherPlayerIndex = i;
    }

    // get the link to be downloaded and execute the download
    shared_ptr<Link> downloadedLink = players[otherPlayerIndex]->getLink(link, otherPlayerIndex + 1);

    // line directly below checks if already downloaded, if so throws error
    downloadedLink->download();
    downloadedLink->setDownloadedBy(currentTurn);
    players[playerIndex]->download(downloadedLink);
    int row = downloadedLink->getRow(), col = downloadedLink->getCol();
    Cell &cell = board->getCell(row, col);
    cell.setContent('.');
}


void Game::usePolarise(char link) {
    // get the link to be polarised
    shared_ptr<Link> currLink;
    for (int i = 0; i < playerCount; ++i) {
        if (validPiLink(link, i + 1)) currLink = players[i]->getLink(link, i + 1);
    }
    
    // check if already downloaded, then no point polarising
    if (currLink->getIsDownloaded()) throw runtime_error(Err::isAlreadyDownloaded);
    currLink->polarise();
}

void Game::useLinkBoost(char link) {
    // if player tries to linkboost other player's link, return error
    if (!(validPiLink(link, currentTurn))) throw runtime_error(Err::cannotUseAbilityOnOtherLink("LinkBoost")); 

    int playerIndex = currentTurn - 1;
    
    // get the link to be linkboosted
    shared_ptr<Link> currLink = players[playerIndex]->getLink(link, currentTurn);

    // if player tries to linkboost already downloaded link, return error
    if (currLink->getIsDownloaded()) throw runtime_error(Err::isAlreadyDownloaded);
    // this is executed if no errors thrown and updates ability state to being activated
    currLink->linkBoost();
}

void Game::useTeleport(int r1, int c1, int r2, int c2) {
    // cannot place teleport on top of a link/firewall/trap/server port/ another teleport
    // cannot place above half of the board
    Cell &cell1 = board->getCell(r1, c1);
    Cell &cell2 = board->getCell(r2, c2);
    
    // check if any of the cells already have teleport
    if (cell1.getTeleport() || cell2.getTeleport()) {
        throw runtime_error(Err::cannotPlaceAbilityonTP("Teleport"));
    }
    // check if any of the cells have a firewall
    if (cell1.hasFirewall() || cell2.hasFirewall()) {
        throw runtime_error(Err::cannotPlaceAbilityonFW("Teleport"));
    }
    // check if any of the cells are server ports
    if (cell1.isServerPort() || cell2.isServerPort()) {
        throw runtime_error(Err::cannotPlaceAbilityonSP("Teleport"));
    }
    // check if any of the cells contain a link
    if (!(cell1.isEmpty() && cell2.isEmpty())) {
        throw runtime_error(Err::cannotPlaceAbilityDirectlyOnOpp("Teleport"));
    }
    
    // set cell1 teleport to cell2 cords
    cell1.setTeleportTarget(cell2.getRow(), cell2.getCol());
    // repeat for cell2
    cell2.setTeleportTarget(cell1.getRow(), cell1.getCol());

    cell1.setTeleport(true);
    cell2.setTeleport(true);
}

void Game::useImprison(int row, int col) {
    Cell &cell = board->getCell(row, col);

    // check if any of the cells already have teleport
    if (cell.getTeleport()) {
        throw runtime_error(Err::cannotPlaceAbilityonTP("Imprison"));
    }
    // check if any of the cells have a firewall
    if (cell.hasFirewall()) {
        throw runtime_error(Err::cannotPlaceAbilityonFW("Imprison"));
    }
    // check if any of the cells are server ports
    if (cell.isServerPort()) {
        throw runtime_error(Err::cannotPlaceAbilityonSP("Imprison"));
    }
    // check if any of the cells contain a link
    if (!(cell.isEmpty())) {
        throw runtime_error(Err::cannotPlaceAbilityDirectlyOnOpp("Imprison"));
    }

    // cell.setImprisonCell(true);
}

void Game::checkGameOver() {
    for (int i = 0; i < playerCount; ++i) {
        // Check if (i + 1)'th player won by downloading enough data
        if (players[i]->getDataDownloaded() >= gameOverCondition) {
            gameOver = true;
            playerWon = i + 1;
        }
        // Check if (i + 1)'th player lost by downloading too many viruses
        if (players[i]->getVirusDownloaded() >= gameOverCondition) {
            
            if (playerCount > 2) {
                players[i]->setEliminated(true);
                --activePlayers;
                gameOver = (activePlayers > 1) ? false : true;
            } else {
                gameOver = true;
                playerWon = (i == 0) ? player2 : player1;
            }
            
        }
    }
}

int Game::getCurrentTurn() const {
    return currentTurn;
}

bool validLink(char link) {
    return validP1Link(link) || validP2Link(link) || validP3Link(link) || validP4Link(link);
}

bool validDirection(const string &direction) {
    return direction == "up" || direction == "down" || direction == "left" || direction == "right";
}

void Game::battle(shared_ptr<Link> currLink, shared_ptr<Link> oppLink, int opponentIndex, Cell &cell) {
    currLink->setIsVisible(true);
    oppLink->setIsVisible(true);
    int currStrength = currLink->getStrength();
    int oppStrength = oppLink->getStrength();
    int playerIndex;
    char link = currLink->getId();
    for (int i = 0; i < playerCount; i++) {
        if (validPiLink(link, i+i)) playerIndex = i;
    }
    bool currLinkWon = currStrength >= oppStrength;
    
    if (currLinkWon) {
        // player downloads and also link status gets updated
        players[playerIndex]->download(oppLink);
        oppLink->download();
        oppLink->setDownloadedBy(playerIndex + 1);

        // set the cell with currLink contents
        currLink->setCol(cell.getCol());
        currLink->setRow(cell.getRow());
        cell.setContent(currLink->getId());
    }
    else {
        // otherwise opponent simply downloads currLink and link status is updated
        players[opponentIndex]->download(currLink);
        currLink->download();
        currLink->setDownloadedBy(opponentIndex + 1);
    }
}

void getNewCords(int &row, int &col, shared_ptr<Link> &moveLink, const string &direction) {
    int increment = moveLink->getStepSize();
    if (direction == "up") {
        row = moveLink->getRow() - increment;
        col = moveLink->getCol();
    }
    else if (direction == "down") {
        row = moveLink->getRow() + increment;
        col = moveLink->getCol();
    }
    else if (direction == "left") {
        col = moveLink->getCol() - increment;
        row = moveLink->getRow();
    }
    else {
        col = moveLink->getCol() + increment;
        row = moveLink->getRow();
    }
}

bool Game::validOutOfBounds(int row, int col) const {
    int size = board.get()->getSize(); // 10 or 8
    int lower = 1, upper = 8;
    lower -= (size == 8);
    upper -= (size == 8);       
    
    if (in(row, 0, size - 1) && in(col, 0, size - 1)) {
        return false;
    } else if (row < 0) { // P1 side
        if (in(col, lower, upper)) {
            if (isActive(1)) {
                if (currentTurn != 1) return true; // if currentTUrn == 1 then error, else true;
            } 
        }  
    } else if (row >= size) { // P2 side
        if (in(col, lower, upper)) {
            if (isActive(2)) {
                if (currentTurn != 2) return true;
            }
        }
    } else if (col < 0) { // P3 side
        if (in(row, lower, upper)) {
            if (isActive(3)) {
                if (currentTurn != 3) return true;
            }
        }
    } else if (col >= size) { // P4 side
        if (in(row, lower, upper)) {
            if (isActive(4)) {
                if (currentTurn != 4) return true;
            }
        }
    }   
    
    throw runtime_error(Err::notInBounds); 
    
}

const shared_ptr<Player>& Game::getPlayer(int playerNumber) const {
    return players[playerNumber - 1];
}

shared_ptr<Board> Game::getBoard() const {
    return board;
}

// isActive() checks if player is playing
bool Game::isActive(int playeri) const {
    if (in(playeri, 3, 4) && (players.size() == 2)) throw runtime_error(Err::notInBounds);
    return !(players[playeri - 1].get()->getEliminated());
}
bool in(int x, int l, int u) {
    return (x >= l) && (x <= u);
}

bool validPiLink(char link, int p) {
    switch(p) {
        case 1: return validP1Link(link);
        case 2: return validP2Link(link);
        case 3: return validP3Link(link);
        default: return validP4Link(link);
    }
}

bool validP1Link(char link) {
    return (link >= 'a' && link <= 'z');
}

bool validP2Link(char link) {
    return (link >= 'A' && link <= 'Z');
}

bool validP3Link(char link) {
    return (link >= 'n' && link <= 'u');
}

bool validP4Link(char link) {
    return (link >= 'N' && link <= 'U');
}
