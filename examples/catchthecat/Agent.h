#ifndef AGENT_H
#define AGENT_H

#include "math/Point2D.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class World;

class Agent {
public:
  struct AStarNode
  {
    AStarNode(Point2D p) : point(p) {}
    AStarNode() = default;

    Point2D point;
    int accDistance;
    int heuristicDistance;

    bool operator<(const AStarNode& rhs) const
    {
      return (accDistance + heuristicDistance) > (rhs.accDistance + rhs.heuristicDistance);
    }

    int closestSideHeuristic(const Point2D p, int halfSideSize) const
    {
      if (p.x - p.y > 0 && p.x + p.y > 0)
      {
        return halfSideSize - p.x;
      }
      else if (p.x - p.y < 0 && p.x + p.y > 0)
      {
        return halfSideSize - p.y;
      }
      else if (p.x - p.y < 0 && p.x + p.y < 0)
      {
        return p.x - halfSideSize;
      }
      else
      {
        return p.y - halfSideSize;
      }
    }
  };

  explicit Agent() = default;

  virtual Point2D Move(World*) = 0;

  std::vector<Point2D> generatePath(World* w);
  std::vector<Point2D> getVisitableNeightbors(World* world, Point2D point, std::unordered_set<Point2D> &queue, std::unordered_map<Point2D, bool> &visited);
};

#endif  // AGENT_H
