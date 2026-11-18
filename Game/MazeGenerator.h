#pragma once

#include "GridPath.h"

#include <set>

namespace Roguelike
{
class MazeGenerator
{
  public:
    MazeGenerator(int width, int height);

    std::set<Engine::GridCell> Generate() const;

  private:
    int width;
    int height;
};
} // namespace Roguelike
