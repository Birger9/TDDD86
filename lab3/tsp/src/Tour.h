/*
 * Header file for Tour
 */

#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:
    /*
     * Constructor
     */
    Tour();
    /*
     * Deconstructor
     */
    ~Tour();
    /*
     * Prints the Tour to the terminal
     */
    void show();
    /*
     * Draws the Tour on the Scene
     */
    void draw(QGraphicsScene* scene);
    /*
     * Returns the size of the Tour
     */
    int size();
    /*
     * Returns the total distance of the Tour
     */
    double distance();
    /*
     * Inserts a point after the point it is nearest to
     */
    void insertNearest(Point p);
    /*
     * Inserts a point where the new total distance is as close to the total distance before the
     * new point where added
     */
    void insertSmallest(Point p);

private:
    Node* firstNode = nullptr;
};

#endif // TOUR_H
