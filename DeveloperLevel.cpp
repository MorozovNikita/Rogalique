#include "DeveloperLevel.h"

#include "Logger.h"
#include "MonsterFactory.h"

#include <ChaseComponent.h>

#include <set>
#include <utility>

using namespace Engine;

namespace Roguelike
{
	void DeveloperLevel::Start()
	{
		const int width = 15;
		const int height = 15;
		const float tile = 128.f;
		const float detectionRadius = 3.f * tile;
		const float stopDistance = 48.f;
		std::set<std::pair<int, int>> walkable;

		for (int y = 0; y <= height; ++y)
		{
			for (int x = 0; x <= width; ++x)
			{
				const Vector2Df position(x * tile, y * tile);
				const bool borderX = x == 0 || x == width;
				const bool borderY = y == 0 || y == height;

				if (!borderX && !borderY)
				{
					floors.push_back(std::make_unique<Floor>(position, 0));
					walkable.insert({ x, y });
					continue;
				}

				if (borderX && !borderY)
				{
					floors.push_back(std::make_unique<Floor>(position, x == 0 ? 18 : 19));
					walls.push_back(std::make_unique<Wall>(position, 12));
					continue;
				}

				int wallIndex = 38;
				if (x == 0 && y == 0)
				{
					wallIndex = 25;
				}
				else if (x == width && y == 0)
				{
					wallIndex = 27;
				}
				else if (x == 0 && y == height)
				{
					wallIndex = 1;
				}
				else if (x == width && y == height)
				{
					wallIndex = 3;
				}

				walls.push_back(std::make_unique<Wall>(position, wallIndex));
			}
		}

		player = std::make_shared<Player>(Vector2Df((width / 2) * tile, (height / 2) * tile));

		ChaseSettings chaseSettings;
		chaseSettings.target = player->GetGameObject();
		chaseSettings.cellSize = tile;
		chaseSettings.walkable = walkable;
		chaseSettings.detectionRadius = detectionRadius;
		chaseSettings.stopDistance = stopDistance;

		MonsterCatalog monstersFactory;
		monsters.push_back(monstersFactory.Create(MonsterId::Vampire, Vector2Df((width / 3) * tile, (height / 3) * tile), chaseSettings));
		monsters.push_back(monstersFactory.Create(MonsterId::Vampire, Vector2Df((width * 2 / 3) * tile, (height / 3) * tile), chaseSettings));
		monsters.push_back(monstersFactory.Create(MonsterId::Vampire, Vector2Df((width / 2) * tile, (height / 5) * tile), chaseSettings));

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
