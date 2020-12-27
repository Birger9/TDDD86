#include "Fast.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "Point.h"
#include <iostream>

Fast::Fast() {

}

// Comparator to check if a < b based on their secondary values.
bool cmp(pair<Point, double> &a, pair<Point, double> &b) {
    return a.second < b.second;
}

void Fast::findLines(QApplication &a, QGraphicsScene *scene, const vector<Point> &points) {
    for (const auto &point : points) {
        std::vector<pair<Point, double>> slopeVec; // Pair vector to hold the slope to every other point.
        for (const auto &otherPoint : points) {
            if (&point == &otherPoint) continue;
                double slope = point.slopeTo(otherPoint);
                slopeVec.push_back(make_pair(otherPoint, slope));
        }
        sort(slopeVec.begin(), slopeVec.end(), cmp);

        int counter = 1;
        for (uint16_t i = 0; i < slopeVec.size(); ++i) {
            if (slopeVec[i].second == slopeVec[i+1].second) {
                counter++;
                if (counter >= 3) {
                    //render_line(scene, point, slopeVec.at(i).first);
                    point.lineTo(scene, slopeVec.at(i).first);
                    a.processEvents();
                }
            }
            else {
                counter = 1;
            }
        }
    }
}
