#pragma once

#include "Vector.h"

#include <set>
#include <utility>
#include <vector>

namespace Engine
{
	using GridCell = std::pair<int, int>;

	std::vector<GridCell> FindPathBFS(const std::set<GridCell>& walkable, const GridCell& start, const GridCell& goal);
	float GridPathLength(const std::vector<GridCell>& path, float cellSize);
	GridCell ClosestWalkableCell(const std::set<GridCell>& walkable, const GridCell& cell);
	std::vector<GridCell> RandomGridCells(const std::set<GridCell>& walkable, std::size_t count);
	Vector2Df GridCellCenter(const GridCell& cell, float cellSize);
}
