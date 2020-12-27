/*
 * TDDD86 Pattern Recognition
 * This program computes and plots all line segments involving 4 points
 * in a file using Qt.
 */

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Point.h"
#include "Fast.h"

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

void render_points(QGraphicsScene* scene, const vector<Point>& points) {
    for(const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2) {
    p1.lineTo(scene, p2);
}

vector<Point> getPointList(int nPoints) {
    string filename = "input" + to_string(nPoints) + ".txt";

    ifstream input;
    input.open(filename);

    int N;
    int x;
    int y;

    input >> N;
    if (nPoints != N) {
        std::cout << "Something is wrooong man!" << std::endl;
    }

    vector<Point> points;
    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();
    return points;
}

void bruteForce(QApplication &a, QGraphicsScene *scene, const vector<Point> &points, int N) {
    // iterate through all combinations of 4 points
    for (int i = 0 ; i < N-3 ; ++i) {
        for (int j = i+1 ; j < N-2 ; ++j) {
            for (int k = j+1 ; k < N-1 ; ++k) {
                //only consider fourth point if first three are collinear
                if (points.at(i).slopeTo(points.at(j)) == points.at(i).slopeTo(points.at(k))) {
                    for (int m{k+1} ; m < N ; ++m) {
                        if (points.at(i).slopeTo(points.at(j)) == points.at(i).slopeTo(points.at(m))) {
                            render_line(scene, points.at(i), points.at(m));
                            a.processEvents(); // show rendered line
                        }
                    }
                }
            }
        }
    }
}

map<int, float> testAlgorithm(int iterations, QApplication &a, QGraphicsScene *scene, vector<int> pointCountVec) {
    map<int, float> timeToRunMap;
    scene->clear();

    for (int N : pointCountVec) {
        std::cout << std::endl << "RUNNING ALGORITHM WITH " << N << " POINTS." << std::endl;
        std::vector<Point> points = getPointList(N);

        sort(points.begin(), points.end());

        int totalTimeMs = 0;
        for (int i = 0; i < iterations; i++) {
            scene->clear();
            render_points(scene, points);

            Fast fast;
            auto begin = chrono::high_resolution_clock::now();
            fast.findLines(a, scene, points);       // TEST FAST
            //bruteForce(a, scene, points, N);        // TEST BRUTE
            auto end = chrono::high_resolution_clock::now();
            int time = std::chrono::duration_cast<chrono::milliseconds>(end - begin).count();
            std::cout << "Time: " << time << " ms." << std::endl;
            totalTimeMs += time;
        }
        std::cout << "Total time to run algorithm " << iterations << " times: " << totalTimeMs << " ms." << std::endl;
        float avgTime = (float)totalTimeMs/iterations;
        std::cout << "Average time: " << avgTime << " ms per iteration." << std::endl;
        timeToRunMap[N] = avgTime;
    }
    return timeToRunMap;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);

    view->setScene(scene);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Brute Force Pattern Recognition");
    view->show();

    std::vector<int> pointCountVec {1600}; // Add number of points to this vector to run tests

    int runsPerIteration = 3;
    auto timeToRunMap = testAlgorithm(runsPerIteration, a, scene, pointCountVec);

    std::cout << std::endl << "Ran " << pointCountVec.size() << " test(s) " << runsPerIteration << " time(s) each: " << std::endl;
    for (const auto &val : timeToRunMap) {
        std::cout << "Running " << val.first << " points took " << val.second << " ms." << std::endl;
    }

    return a.exec(); // start Qt event loop
}
