#include "MazeGenerator.h"

#include <random>
#include <vector>

namespace Roguelike
{
	MazeGenerator::MazeGenerator(int width, int height)
		: width(width)
		, height(height)
	{
	}

	std::set<Engine::GridCell> MazeGenerator::Generate() const
	{
		std::vector<Engine::GridCell> rooms;
		for (int y = 1; y < height; y += 2)
		{
			for (int x = 1; x < width; x += 2)
			{
				rooms.push_back({ x, y });
			}
		}
		if (rooms.empty())
		{
			return {};
		}

		std::mt19937 random{ std::random_device{}() };
		std::set<Engine::GridCell> visited;
		std::set<Engine::GridCell> walkable;
		std::vector<Engine::GridCell> stack;
		stack.push_back(rooms.front());
		visited.insert(stack.back());
		walkable.insert(stack.back());

		const Engine::GridCell directions[4] = 
		{
			{ 0, -2 },
			{ 0, 2 },
			{ -2, 0 },
			{ 2, 0 }
		};

		while (!stack.empty())
		{
			const Engine::GridCell current = stack.back();
			std::vector<Engine::GridCell> nextRooms;
			for (const Engine::GridCell& direction : directions)
			{
				const Engine::GridCell next = { current.first + direction.first, current.second + direction.second };
				if (next.first <= 0 || next.second <= 0 || next.first >= width || next.second >= height)
				{
					continue;
				}
				if (visited.find(next) != visited.end())
				{
					continue;
				}

				nextRooms.push_back(next);
			}

			if (nextRooms.empty())
			{
				stack.pop_back();
				continue;
			}

			std::shuffle(nextRooms.begin(), nextRooms.end(), random);
			const Engine::GridCell next = nextRooms.front();
			walkable.insert({ (current.first + next.first) / 2, (current.second + next.second) / 2 });
			walkable.insert(next);
			visited.insert(next);
			stack.push_back(next);
		}

		return walkable;
	}
}
