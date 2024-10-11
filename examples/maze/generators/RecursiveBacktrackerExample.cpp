#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>
bool RecursiveBacktrackerExample::Step(World* w) {
  auto sideOver2 = w->GetSize() / 2;

  // Initialize stack if empty and starting point is not visited
  if (stack.empty() && !visited[-sideOver2][-sideOver2])
  {
    stack.emplace_back(-sideOver2, -sideOver2);
    return true;
  }

  if (!stack.empty())
  {
    Point2D current = stack.back();
    std::vector<Point2D> neighbors = getVisitables(w, current);

    if (!neighbors.empty())
    {
      w->SetNodeColor(current, Color::Red);
      visited[current.x][current.y] = true;
      Point2D next = neighbors[getRandomNumber() % neighbors.size()];
      Point2D direction = next - current;

      if (direction.y == -1) w->SetNorth(current, false);   // Up
      else if (direction.x == 1) w->SetEast(current, false); // Right
      else if (direction.y == 1) w->SetSouth(current, false);// Down
      else if (direction.x == -1) w->SetWest(current, false);// Left


      stack.push_back(next);
    }
    else
    {
      // backtrack: mark the point as visited (black) and pop from stack
      w->SetNodeColor(current, Color::Black);
      stack.pop_back();
    }
  }

  // Return whether the stack is empty or not
  return !stack.empty();
}

void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

int RecursiveBacktrackerExample::getRandomNumber()
{
   int randomNumbers[] = {72, 99, 56, 34, 43, 62, 31, 4, 70, 22, 6, 65, 96, 71, 29, 9, 98, 41, 90, 7,
    30, 3, 97, 49, 63, 88, 47, 82, 91, 54, 74, 2, 86, 14, 58, 35, 89, 11, 10, 60,
    28, 21, 52, 50, 55, 69, 76, 94, 23, 66, 15, 57, 44, 18, 67, 5, 24, 33, 77, 53,
    51, 59, 20, 42, 80, 61, 1, 0, 38, 64, 45, 92, 46, 79, 93, 95, 37, 40, 83, 13,
    12, 78, 75, 73, 84, 81, 8, 32, 27, 19, 87, 85, 16, 25, 17, 68, 26, 39, 48, 36};

  randomNumberIndex++;
  if(randomNumberIndex > sizeof(randomNumbers) / sizeof(randomNumbers[0]))
  {
    randomNumberIndex = 0;
  }

  return randomNumbers[randomNumberIndex];
}


std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  if (w->isValidPosition(p.Up()) && !visited[p.Up().x][p.Up().y])
  {
    visitables.push_back(p.Up());
  }
  if (w->isValidPosition(p.Right()) && !visited[p.Right().x][p.Right().y])
  {
    visitables.push_back(p.Right());
  }
  if (w->isValidPosition(p.Down()) && !visited[p.Down().x][p.Down().y])
  {
    visitables.push_back(p.Down());
  }
  if (w->isValidPosition(p.Left()) && !visited[p.Left().x][p.Left().y])
  {
    visitables.push_back(p.Left());
  }
  return visitables;
}
