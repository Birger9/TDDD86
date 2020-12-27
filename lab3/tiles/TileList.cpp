/*
 * This class contains all the logic for TileList
 */

#include "TileList.h"



TileList::TileList()
{
    tileList = new Tile[size]; // Point to address for new array
}

TileList::~TileList()
{
    delete [] tileList;
}

void TileList::addTile(const Tile tile)
{
    if (count != size) {
        tileList[count] = tile;
    }else {
        int newSize = size * 2;

        Tile* newArr = new Tile[newSize];
        for(int i = 0; i < count; i++) {
            newArr[i] = tileList[i];
        }

        size = newSize;
        delete [] tileList;
        tileList = newArr;
        tileList[count] = tile;
    }
    count++;
}

void TileList::drawAll(QGraphicsScene* scene)
{
    for (int i = 0; i < count; i++) {
        tileList[i].draw(scene);
    }
}

int TileList::indexOfTopTile(const int x, const int y)
{
    int topIndex = -1;
    for (int i = count-1; i >= 0; i--) {
        if (tileList[i].contains(x, y)) {
            topIndex  = i;
            break;
        }
    }
    return topIndex;
}

void TileList::raise(const int x, const int y)
{
    const int topIndex = indexOfTopTile(x, y);
    if (topIndex == -1) return; // No tile found.

    const Tile tile = tileList[topIndex];
    for (int i = topIndex + 1; i < count; i++) {
        tileList[i-1] = tileList[i];
    }
    tileList[count-1] = tile;
}

void TileList::lower(const int x, const int y)
{
    const int topIndex = indexOfTopTile(x, y);
    if (topIndex == -1) return; // No tile found.

    const Tile tile = tileList[topIndex];
    for (int i = topIndex; i > 0; i--) {
        tileList[i] = tileList[i-1];
    }
    tileList[0] = tile;
}

void TileList::remove(const int x, const int y)
{
    const int topIndex = indexOfTopTile(x, y);
    if (topIndex == -1) return;

    for (int i = topIndex; i < count-1; i++) {
        tileList[i] = tileList[i+1];
    }
    count--;
}

void TileList::removeAll(const int x, const int y)
{
    while (indexOfTopTile(x, y) != -1) {
        remove(x, y);
    }
}
