#ifndef FAST_H
#define FAST_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "Point.h"

class Fast {

public:
    Fast();

    /*
     * Find cases where at least 4 points are collinear.
     */
    void findLines(QApplication &a, QGraphicsScene *scene, const vector<Point> &points);

};

#endif // FAST_H
