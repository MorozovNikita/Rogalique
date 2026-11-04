#include "pch.h"
#include "GridPath.h"

#include <algorithm>
#include <map>
#include <queue>

namespace Engine
{

	std::vector<GridCell> FindPathBFS(const std::set<GridCell>& walkable, const GridCell& start, const GridCell& goal)
	{
		std::vector<GridCell> path;

		if (walkable.find(start) == walkable.end() || walkable.find(goal) == walkable.end())
		{
			return path;
		}

		if (start == goal)
		{
			path.push_back(start);
			return path;
		}

		std::map<GridCell, GridCell> cameFrom;
		std::set<GridCell> visited;
		std::queue<GridCell> frontier;
		frontier.push(start);
		visited.insert(start);

		const int offsetX[4] = { 1, -1, 0, 0 };
		const int offsetY[4] = { 0, 0, 1, -1 };
		bool found = false;

		while (!frontier.empty())
		{
			const GridCell current = frontier.front();
			frontier.pop();
			if (current == goal)
			{
				found = true;
				break;
			}

			for (int i = 0; i < 4; i++)
			{
				const GridCell next = { current.first + offsetX[i], current.second + offsetY[i] };
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

		for (GridCell cell = goal; cell != start; cell = cameFrom[cell])
		{
			path.push_back(cell);
		}
		path.push_back(start);
		std::reverse(path.begin(), path.end());
		return path;
	}

}
