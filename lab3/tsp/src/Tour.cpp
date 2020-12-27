/*
 * This class handles all things related to the Tour e.g insert new nodes,
 * constructor and destructor
 *
*/

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

/*
 * Initialize tour.
 */
Tour::Tour()
{

}

/*
 * Delete all nodes.
 */
Tour::~Tour()
{
    if(firstNode == nullptr) return;

    Node* current = firstNode;

    do {
        Node* next = current->next;
        delete current;
        current = next;
    }
    while (firstNode != current);
}


/*
 * Print tour.
 */
void Tour::show()
{
    if(firstNode == nullptr) return;

    cout << firstNode->point << endl;
    for(Node* current = firstNode->next; current != firstNode; current = current->next) {
         cout << current->point << endl;
    }
}

/*
 * Visualize tour.
 */
void Tour::draw(QGraphicsScene *scene)
{
    scene->clear(); // Clears the scene
    if(firstNode == nullptr) return; // No need to draw an empty circular linked list

    if (firstNode->next != firstNode) {
        Point startPoint = firstNode->point;
        Point nextPoint = firstNode->next->point;
        startPoint.drawTo(nextPoint, scene);

        for(Node* current = firstNode->next; current != firstNode; current = current->next) {
            Point pointA = current->point;
            Point pointB = current->next->point;
            pointA.drawTo(pointB, scene);
        }
    }
}

/*
 * Tour size.
 */
int Tour::size()
{
    if(firstNode == nullptr) return 0; // The circular linked list is empy, return 0.

    int count = 1;
    for(Node* current = firstNode->next; current != firstNode; current = current->next) {
         count++;
    }
    return count;
}

/*
 * Distance between each node from start to finish.
 */
double Tour::distance()
{
    if(firstNode == nullptr) return 0.0; // If true then the circular linked list is empty, return 0.0

    double totDist = 0;
    if (firstNode->next != firstNode) {

        Point startPoint = firstNode->point;
        Point nextPoint = firstNode->next->point;
        totDist += startPoint.distanceTo(nextPoint);

        for(Node* current = firstNode->next; current != firstNode; current = current->next) {
            Point pointA = current->point;
            Point pointB = current->next->point;
            totDist += pointA.distanceTo(pointB);
        }
    }
    return totDist;
}

/*
 * Insert nearest (always selects the nearest node).
 */
void Tour::insertNearest(Point p)
{
    if(firstNode == nullptr) { // If true then the circular linked list is empty
        firstNode = new Node(p, nullptr); // Creates an new node
        firstNode->next = firstNode; // The nearest node points to itself
        return;
    }

    Node* nearestNode = firstNode;
    int shortestDistance = nearestNode->point.distanceTo(p);

    if (firstNode->next != firstNode) { // If there is more than one node in the circular linked list
        for(Node* current = firstNode->next; current != firstNode; current = current->next) {

            int newDistance = current->point.distanceTo(p); // Checks the distance between a new point and p.

            if(newDistance < shortestDistance) {
                nearestNode = current; // Update the shortest node
                shortestDistance = newDistance; // Update shortest distance
            }
        }
    }

    Node* newNode = new Node(p, nearestNode->next); // Creates an new node that is inserted after the the nearest node
    nearestNode->next = newNode; // The nearest node points to the newly created Node
}

/*
 * Insert smallest (always select the node that yields the smallest increase of the total tour distance).
 */
void Tour::insertSmallest(Point p)
{
    if(firstNode == nullptr) { // If true then the circular linked list is empty
        firstNode = new Node(p, nullptr); // Create a new node
        firstNode->next = firstNode; // The nearest node points to itself
        return;
    }

  double startDistance =  distance();
  Node* nearestNode = firstNode;
  Point a = nearestNode->point;
  Point b =  nearestNode->next->point;
  double shortestDistance = startDistance + a.distanceTo(p) + p.distanceTo(b) - a.distanceTo(b);

  if (firstNode->next != firstNode) { // If there is more than one node in the circular linked list
      for(Node* current = firstNode->next; current != firstNode; current = current->next) {
          Point newA = current->point;
          Point newB = current->next->point;
          int newDistance = startDistance + newA.distanceTo(p) + p.distanceTo(newB) - newA.distanceTo(newB); // Checks the distance between a new point and p.

          if(newDistance < shortestDistance) {
              nearestNode = current; // Update the shortest node
              shortestDistance = newDistance; // Update shortest distance
          }
      }
  }
  Node* newNode = new Node(p, nearestNode->next); // Creates an new node that is inserted after the the nearest node
  nearestNode->next = newNode; // The nearest node points to the newly created Node

}






















