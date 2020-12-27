#ifndef CUBE_H
#define CUBE_H


class Cube {
public:
    Cube(char letter, int x, int y);
    char getLetter();
    bool getVisited();
    void setVisited(bool visited);
    int getX();
    int getY();

private:
    char letter;
    bool visited;
    int x;
    int y;
};

#endif // CUBE_H

