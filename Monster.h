#pragma once

#include "GameWorld.h"
#include "Vector.h"

#include <string>

namespace Engine
{
	struct ChaseSettings;
}

namespace Roguelike
{
	struct MonsterAppearance
	{
		std::string id;
		std::string idle;
		std::string run;
		std::string attack;
	};

	class Monster
	{
	public:
		Monster(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings, const MonsterAppearance& appearance);
		Engine::GameObject* GetGameObject();

	private:
		Engine::GameObject* gameObject;
	};
}
