#pragma once

#include "Scene.h"
#include "Player.h"
#include "Floor.h"
#include "Music.h"
#include "Monster.h"
#include "Wall.h"

#include <memory>
#include <vector>

using namespace Engine;

namespace Roguelike
{
	class DeveloperLevel : public Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;
	private:
		std::shared_ptr<Player> player;
		std::vector<std::unique_ptr<Monster>> monsters;
		std::unique_ptr<Music> music;

		std::vector<std::unique_ptr<Wall>> walls;
		std::vector<std::unique_ptr<Floor>> floors;
	};
}
