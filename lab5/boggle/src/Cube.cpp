//
//  Cube contains the information for a single character on the board.
//

#include "Cube.h"

Cube::Cube(char aLetter, int xPos, int yPos)
{
    letter = aLetter;
    visited = false;
    x = xPos;
    y = yPos;
}

char Cube::getLetter() {return letter;}

bool Cube::getVisited() {return visited;}

void Cube::setVisited(bool visited) {this->visited = visited;}

int Cube::getX() {
    return x;
}

int Cube::getY() {
    return y;
}
