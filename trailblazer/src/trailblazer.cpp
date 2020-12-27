/*
 * This file contains different pathfinding algorithms for example dfs and bfs.
 */

#include "costs.h"
#include "trailblazer.h"
#include <stack>
#include <list>
#include <queue>
#include "pqueue.h"
#include <algorithm>
using namespace std;

/*
 * A helper function to the depthFirstSearch function.
 */
void dfs(Vertex* current, vector<Vertex*> &path, Vertex* end, bool &found) {
    path.push_back(current);
    current->setColor(GREEN);
    current->visited = true;
    if(current == end) { // We found the destination node
        found = true;
        return;
    }

    else
        for (const auto &edge : current->arcs) {
            Node* neighbor = edge->finish;
            if(!neighbor->visited && !found) {
                dfs(neighbor, path, end, found);
            }
        }

    if(!found) {
        current->setColor(GRAY); // Dead end
        path.pop_back(); // Remove from path
    }
}

/*
 * The Depth First Search algorithm. Implemented using recursion.
 */
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    bool found = false;
    dfs(start, path, end, found);

    return path;
}

/*
 * The Breadth First Search algorithm.
 */
vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
   graph.resetData();
   vector<Vertex*> path;

    // Create a queue for BFS
    queue<Vertex*> q;
    start->visited = true; // Set first node visited
    q.push(start);
    Vertex* current;

    while(!q.empty()) {
        current = q.front();
        q.pop();
        current->setColor(GREEN);

        if(current == end) {
            end->previous = current->previous;
            break;
        }

        for (const auto &edge : current->arcs) {
            Node* neighbor = edge->finish;          
            if(!neighbor->visited) {
                neighbor->previous = current; // Keep track of the node on the start of the edge
                neighbor->visited = true;
                neighbor->setColor(YELLOW);
                q.push(neighbor);
            }
        }
    }

    current = end;
    path.push_back(current);
    while(current != start) {
        current = current->previous;
        path.push_back(current);
    }

    return path;
}

/*
 * The Dijkstras algorithm.
 */
vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;

    start->setColor(GREEN);
    start->visited = true;

    PriorityQueue<Node*> nodeQ;

    for (const auto &node : graph.getNodeSet()) {
        if (node == start) continue;
        nodeQ.enqueue(node, INFINITY);
        node->cost = INFINITY;
    }
    nodeQ.enqueue(start, 0.0); // Set dist to source to 0.0
    start->cost = 0.0;

    while (!nodeQ.isEmpty()) {
        Node* curr = nodeQ.dequeue();   // Pick node with smallest cost.
        curr->setColor(GREEN);

        for (const auto &edge : curr->arcs) {
            Node* neighbor = edge->finish;
            curr->setColor(GREEN);

            double cost = curr->cost + curr->heuristic(neighbor); // Get edge cost arc cost
            if (cost <= neighbor->cost) {
                neighbor->previous = curr;
                neighbor->cost = cost;
                nodeQ.changePriority(neighbor, cost);
                neighbor->setColor(YELLOW); // head of current path
            }
        }
        if (curr == end) break;
    }

    // Backtrack by iterating through curr->previous to find correct path
    Node* curr = end;
    end->setColor(GREEN);
    path.push_back(end);
    while (curr != start) {
        curr = curr->previous;
        curr->setColor(GREEN);
        path.push_back(curr);
    }
    reverse(path.begin(), path.end());
    return path;
}

/*
 * The A star algoritm.
 */
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;

    for(const auto &node : graph.getNodeSet()) {
        if(node == start) continue;
        pq.enqueue(node, INFINITY);
        node->cost = INFINITY;
    }

    start->cost = 0.0;
    pq.enqueue(start, 0.0);
    Vertex* current;
    start->visited = true;

    while(!pq.isEmpty()) {
        current = pq.dequeue(); // Remove and return the node with smallest cost
        current->setColor(GREEN);

        if(current == end) {
            end->previous = current->previous;
            break;
        }

        for (const auto &edge : current->arcs) {
            Node* neighbor = edge->finish;
            double newCost = current->cost + current->heuristic(neighbor); // The cost to from start node to the neighbor node

            if(!neighbor->visited && newCost <= neighbor->cost) {
                neighbor->visited = true;
                neighbor->cost = newCost; // Update the neighbor node cost
                double totalCost = newCost + neighbor->heuristic(end);
                pq.changePriority(neighbor, totalCost);
                neighbor->previous = current; // Keep track of the node on the start of the edge
                neighbor->setColor(YELLOW);
            }
    }
   }

    current = end;
    path.push_back(current);
    while(current != start) {
        current = current->previous;
        path.push_back(current);
    }
    reverse(path.begin(), path.end());
    return path;
}
