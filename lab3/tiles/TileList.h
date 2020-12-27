/*
 * Header file for TileList
 */

#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"

class TileList {
public:
    TileList(); // Constructor.
    ~TileList(); // Destructor.
    void addTile(Tile tile); // Add a tile to the tile list.
    void drawAll(QGraphicsScene* scene); // Render each tile in the tile list.
    int indexOfTopTile(int x, int y); // Return the index of the top tile found at (x,y). Return -1 if no tile is found.
    void lower(int x, int y); // Lower the tile at (x,y) to the bottom of the list.
    void raise(int x, int y); // Raise the tile at (x,y) to the top of the list.
    void remove(int x, int y); // Remove the top tile at (x,y) from the list
    void removeAll(int x, int y); // Remove every tile at (x,y) from the list.

private:
    int size = 10;
    Tile* tileList;
    int count = 0;


};

#endif // TILELIST_H
