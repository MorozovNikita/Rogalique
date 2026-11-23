#include "DeveloperLevel.h"

#include "Logger.h"
#include "MonsterFactory.h"
#include "MazeGenerator.h"

#include <ChaseComponent.h>
#include <GridPath.h>

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <utility>

using namespace Engine;

namespace Roguelike
{
namespace
{
constexpr int kFloorTile = 0;
constexpr int kStairsTile = 16;
constexpr int kHoleTile = 17;

GridCell FarthestWalkableCell(const std::set<GridCell> &walkable, const GridCell &start)
{
    GridCell farthest = start;
    int farthestDistance = 0;
    std::map<GridCell, int> distance;
    std::queue<GridCell> pending;
    distance.emplace(start, 0);
    pending.push(start);

    const GridCell steps[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    while (!pending.empty())
    {
        const GridCell current = pending.front();
        pending.pop();
        const int currentDistance = distance[current];
        for (const GridCell &step : steps)
        {
            const GridCell next{current.first + step.first, current.second + step.second};
            if (walkable.find(next) == walkable.end() || distance.find(next) != distance.end())
            {
                continue;
            }

            const int nextDistance = currentDistance + 1;
            distance.emplace(next, nextDistance);
            pending.push(next);
            if (nextDistance > farthestDistance)
            {
                farthestDistance = nextDistance;
                farthest = next;
            }
        }
    }

    return farthest;
}

int WallSpriteIndex(bool north, bool east, bool south, bool west)
{
    const int mask = (north ? 1 : 0) | (east ? 2 : 0) | (south ? 4 : 0) | (west ? 8 : 0);
    const int sprites[16] = {22, 24, 37, 25, 0, 12, 1, 13, 39, 27, 38, 41, 3, 15, 2, 4};
    return sprites[mask];
}
} // namespace

void DeveloperLevel::Start()
{
    const int width = 16;
    const int height = 16;
    const float tile = 128.f;
    const float detectionRadius = 3.f * tile;
    const float stopDistance = 48.f;

    const std::set<GridCell> walkable = MazeGenerator(width, height).Generate();
    const GridCell start = RandomGridCells(walkable, 1).front();
    const GridCell exit = FarthestWalkableCell(walkable, start);
    auto isWall = [&](int x, int y) {
        if (x < 0 || y < 0 || x > width || y > height)
        {
            return false;
        }
        if (x == 0 || y == 0 || x == width || y == height)
        {
            return true;
        }
        return walkable.find({x, y}) == walkable.end();
    };

    for (int y = 0; y <= height; ++y)
    {
        for (int x = 0; x <= width; ++x)
        {
            const Vector2Df position(x * tile, y * tile);
            const bool borderX = x == 0 || x == width;
            const bool borderY = y == 0 || y == height;
            if (!borderX && !borderY && walkable.find({x, y}) != walkable.end())
            {
                const GridCell cell{x, y};
                int floorTile = kFloorTile;
                if (cell == start)
                {
                    floorTile = kHoleTile;
                }
                else if (cell == exit)
                {
                    floorTile = kStairsTile;
                }
                floors.push_back(std::make_unique<Floor>(position, floorTile));
                continue;
            }

            if (borderX && !borderY)
            {
                floors.push_back(std::make_unique<Floor>(position, x == 0 ? 18 : 19));
            }

            const int wallIndex =
                WallSpriteIndex(isWall(x, y + 1), isWall(x + 1, y), isWall(x, y - 1), isWall(x - 1, y));
            walls.push_back(std::make_unique<Wall>(position, wallIndex));
        }
    }

    player = std::make_shared<Player>(GridCellCenter(start, tile), tile);
    if (exit != start)
    {
        player->SetStairs(GridCellCenter(exit, tile), tile);
    }

    ChaseSettings chaseSettings;
    chaseSettings.target = player->GetGameObject();
    chaseSettings.cellSize = tile;
    chaseSettings.walkable = walkable;
    chaseSettings.detectionRadius = detectionRadius;
    chaseSettings.stopDistance = stopDistance;

    std::set<GridCell> monsterCells = walkable;
    monsterCells.erase(start);
    monsterCells.erase(exit);
    const std::size_t monsterCount = std::min<std::size_t>(3, monsterCells.size());
    const std::vector<GridCell> monsterSpawns =
        monsterCount == 0 ? std::vector<GridCell>{} : RandomGridCells(monsterCells, monsterCount);

    MonsterCatalog monstersFactory;
    for (const GridCell &cell : monsterSpawns)
    {
        monsters.push_back(monstersFactory.Create(MonsterId::Vampire, GridCellCenter(cell, tile), chaseSettings));
    }

    music = std::make_unique<Music>("music");
    LOG_INFO("Developer level started");
}

void DeveloperLevel::Restart()
{
    Stop();
    Start();
}

void DeveloperLevel::Stop()
{
    player.reset();
    monsters.clear();
    music.reset();
    walls.clear();
    floors.clear();
    GameWorld::Instance()->Clear();
}
} // namespace Roguelike
