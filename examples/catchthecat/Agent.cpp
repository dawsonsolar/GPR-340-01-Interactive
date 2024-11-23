#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "World.h"
using namespace std;

std::vector<Point2D> Agent::generatePath(World* w) {
    unordered_map<Point2D, Point2D> cameFrom;
    priority_queue<AStarNode> frontier;
    unordered_set<Point2D> frontierSet;
    unordered_map<Point2D, bool> visited;

    AStarNode start(w->getCat());
    start.heuristicDistance = start.closestSideHeuristic(start.point, w->getWorldSideSize() / 2);

    frontier.push(start);
    frontierSet.insert(start.point);
    Point2D borderExit = Point2D::INFINITE;

    while (!frontier.empty()) {
        const AStarNode current = frontier.top();
        frontier.pop();
        frontierSet.erase(current.point);

        int gridHalfSize = w->getWorldSideSize() / 2;
        if (current.point.y == gridHalfSize || current.point.y == -gridHalfSize ||
            current.point.x == gridHalfSize || current.point.x == -gridHalfSize) {
            borderExit = current.point;
            break;
        }

        visited[current.point] = true;
        std::vector<Point2D> neighbors = getVisitableNeightbors(w, current.point, frontierSet, visited);

        for (const Point2D& neighbor : neighbors) {
            cameFrom[neighbor] = current.point;
            AStarNode newNeighborNode(neighbor);
            newNeighborNode.heuristicDistance = start.closestSideHeuristic(neighbor, w->getWorldSideSize() / 2);
            frontier.push(newNeighborNode);
            frontierSet.insert(neighbor);
        }
    }

    vector<Point2D> path;
    if (borderExit != Point2D::INFINITE) {
        Point2D current = borderExit;
        while (current != start.point) {
            path.push_back(current);
            current = cameFrom[current];
        }
    }

    return path;
}

std::vector<Point2D> Agent::getVisitableNeightbors(World* world, Point2D point, std::unordered_set<Point2D>& queue, std::unordered_map<Point2D, bool>& visited) {
    int start = point.x - static_cast<int>(point.y % 2 == 0);
    std::vector<Point2D> visitables;

    for (int i = start; i < start + 2; i++) {
        Point2D checkPoint = {i, point.y - 1};
        if (!queue.contains(checkPoint) && !visited.contains(checkPoint) &&
            !world->getContent(checkPoint) && world->getCat() != checkPoint) {
            visitables.push_back(checkPoint);
        }
    }

    for (int i = start; i < start + 2; i++) {
        Point2D checkPoint = {i, point.y + 1};
        if (!queue.contains(checkPoint) && !visited.contains(checkPoint) &&
            !world->getContent(checkPoint) && world->getCat() != checkPoint) {
            visitables.push_back(checkPoint);
        }
    }

    start = point.x - 1;
    for (int i = start; i < start + 3; i++) {
        if (i != point.x) {
            Point2D checkPoint = {i, point.y};
            if (!queue.contains(checkPoint) && !visited.contains(checkPoint) &&
                !world->getContent(checkPoint) && world->getCat() != checkPoint) {
                visitables.push_back(checkPoint);
            }
        }
    }

    return visitables;
}
