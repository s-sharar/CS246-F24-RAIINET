#ifndef LINK_H
#define LINK_H
#include <string>

enum LinkType{Data = 'D', Virus = 'V'};

class Link {
    char id;
    LinkType t;
    int strength, row, col, stepSize;
    bool isVisible, isDownloaded;
    public:
    Link(char id, LinkType t, int strength, int row, int col);

    // getters
    char getId() const;
    LinkType getType() const;
    int getStrength() const;
    int getRow() const;
    int getCol() const;
    bool getIsVisible() const;
    bool getIsDownloaded() const;

    // setters
    void setRow(int r);
    void setCol(int c);
    void setStrength(int s);

    void linkBoost();
    void polarise();
    void scan();
    void download();
};


#endif
