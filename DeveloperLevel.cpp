#include "DeveloperLevel.h"

#include "Logger.h"
#include "MonsterFactory.h"
#include "MazeGenerator.h"

#include <ChaseComponent.h>
#include <GridPath.h>

#include <set>
#include <utility>

using namespace Engine;

namespace Roguelike
{
	namespace
	{
		int WallSpriteIndex(bool north, bool east, bool south, bool west)
		{
			const int mask = (north ? 1 : 0) | (east ? 2 : 0) | (south ? 4 : 0) | (west ? 8 : 0);
			const int sprites[16] = {
				22, 24, 37, 25,
				0, 12, 1, 13,
				39, 27, 38, 41,
				3, 15, 2, 4
			};
			return sprites[mask];
		}
	}

	void DeveloperLevel::Start()
	{
		const int width = 16;
		const int height = 16;
		const float tile = 128.f;
		const float detectionRadius = 3.f * tile;
		const float stopDistance = 48.f;

		const std::set<GridCell> walkable = MazeGenerator(width, height).Generate();
		auto isWall = [&](int x, int y)
		{
			if (x < 0 || y < 0 || x > width || y > height)
			{
				return false;
			}
			if (x == 0 || y == 0 || x == width || y == height)
			{
				return true;
			}
			return walkable.find({ x, y }) == walkable.end();
		};

		for (int y = 0; y <= height; ++y)
		{
			for (int x = 0; x <= width; ++x)
			{
				const Vector2Df position(x * tile, y * tile);
				const bool borderX = x == 0 || x == width;
				const bool borderY = y == 0 || y == height;
				if (!borderX && !borderY && walkable.find({ x, y }) != walkable.end())
				{
					floors.push_back(std::make_unique<Floor>(position, 0));
					continue;
				}

				if (borderX && !borderY)
				{
					floors.push_back(std::make_unique<Floor>(position, x == 0 ? 18 : 19));
				}

				const int wallIndex = WallSpriteIndex(isWall(x, y + 1), isWall(x + 1, y), isWall(x, y - 1), isWall(x - 1, y));
				walls.push_back(std::make_unique<Wall>(position, wallIndex));
			}
		}

		const std::vector<GridCell> spawns = RandomGridCells(walkable, 4);
		player = std::make_shared<Player>(GridCellCenter(spawns[0], tile));

		ChaseSettings chaseSettings;
		chaseSettings.target = player->GetGameObject();
		chaseSettings.cellSize = tile;
		chaseSettings.walkable = walkable;
		chaseSettings.detectionRadius = detectionRadius;
		chaseSettings.stopDistance = stopDistance;

		MonsterCatalog monstersFactory;
		for (std::size_t index = 1; index < spawns.size(); ++index)
		{
			monsters.push_back(monstersFactory.Create(MonsterId::Vampire, GridCellCenter(spawns[index], tile), chaseSettings));
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
}
