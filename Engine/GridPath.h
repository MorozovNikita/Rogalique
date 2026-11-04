#pragma once

#include <set>
#include <utility>
#include <vector>

namespace Engine
{
	using GridCell = std::pair<int, int>;

	std::vector<GridCell> FindPathBFS(const std::set<GridCell>& walkable, const GridCell& start, const GridCell& goal);
}
