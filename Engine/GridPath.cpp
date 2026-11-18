#include "pch.h"
#include "GridPath.h"

#include <algorithm>
#include <map>
#include <queue>
#include <random>
#include <stdexcept>

namespace Engine
{
GridCell ClosestWalkableCell(const std::set<GridCell> &walkable, const GridCell &cell)
{
    if (walkable.empty() || walkable.find(cell) != walkable.end())
    {
        return cell;
    }

    GridCell closest = *walkable.begin();
    int closestDistance = std::abs(closest.first - cell.first) + std::abs(closest.second - cell.second);
    for (const GridCell &candidate : walkable)
    {
        const int distance = std::abs(candidate.first - cell.first) + std::abs(candidate.second - cell.second);
        if (distance < closestDistance)
        {
            closest = candidate;
            closestDistance = distance;
        }
    }

    return closest;
}

std::vector<GridCell> RandomGridCells(const std::set<GridCell> &walkable, std::size_t count)
{
    if (count > walkable.size())
    {
        throw std::invalid_argument("Not enough walkable cells");
    }

    std::vector<GridCell> cells(walkable.begin(), walkable.end());
    std::mt19937 random{std::random_device{}()};
    std::shuffle(cells.begin(), cells.end(), random);
    cells.resize(count);
    return cells;
}

float GridPathLength(const std::vector<GridCell> &path, float cellSize)
{
    if (path.size() < 2 || cellSize <= 0.f)
    {
        return 0.f;
    }

    return static_cast<float>(path.size() - 1) * cellSize;
}

Vector2Df GridCellCenter(const GridCell &cell, float cellSize)
{
    return {cell.first * cellSize, cell.second * cellSize};
}

std::vector<GridCell> FindPathBFS(const std::set<GridCell> &walkable, const GridCell &start, const GridCell &goal)
{
    std::vector<GridCell> path;
    if (walkable.empty())
    {
        return path;
    }

    const GridCell pathStart = ClosestWalkableCell(walkable, start);
    const GridCell pathGoal = ClosestWalkableCell(walkable, goal);
    if (pathStart == pathGoal)
    {
        path.push_back(pathStart);
        return path;
    }

    std::map<GridCell, GridCell> cameFrom;
    std::set<GridCell> visited;
    std::queue<GridCell> frontier;
    frontier.push(pathStart);
    visited.insert(pathStart);

    const int offsetX[4] = {1, -1, 0, 0};
    const int offsetY[4] = {0, 0, 1, -1};
    bool found = false;

    while (!frontier.empty())
    {
        const GridCell current = frontier.front();
        frontier.pop();
        if (current == pathGoal)
        {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i)
        {
            const GridCell next = {current.first + offsetX[i], current.second + offsetY[i]};
            if (walkable.find(next) == walkable.end() || visited.find(next) != visited.end())
            {
                continue;
            }

            visited.insert(next);
            cameFrom[next] = current;
            frontier.push(next);
        }
    }

    if (!found)
    {
        return path;
    }

    for (GridCell cell = pathGoal; cell != pathStart; cell = cameFrom[cell])
    {
        path.push_back(cell);
    }
    path.push_back(pathStart);
    std::reverse(path.begin(), path.end());
    return path;
}
} // namespace Engine
