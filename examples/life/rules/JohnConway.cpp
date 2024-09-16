#include "JohnConway.h"

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  for (int y = 0; y < world.SideSize(); y++)
  {
    for (int x = 0; x < world.SideSize(); x++)
    {
      Point2D tempPoint = Point2D(x, y);
      if (world.Get(tempPoint))
      {
        if (CountNeighbors(world, tempPoint) < 2 || CountNeighbors(world, tempPoint) > 3)
          world.SetNext(tempPoint, false);
        else
          world.SetNext(tempPoint, true);
      }
    }
  }
  world.SwapBuffers();
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  int count = 0;
  for (int y = -1; y <= 1; y++)
  {
    for (int x = -1; x <= 1; x++)
    {
      Point2D tempPoint = Point2D((point.x + x) % world.SideSize(), (point.y + y) % world.SideSize());
      if (tempPoint != point && world.Get(tempPoint))
        count++;
    }
  }
  return count;
}
