#pragma once

#include "GameObject.h"

#include <GameWorld.h>
#include <ResourceSystem.h>

namespace Roguelike
{
	class Wall
	{
	public:
		Wall(const Engine::Vector2Df& position, int textureMapIndex);
	private:
		Engine::GameObject* gameObject;
	};
}
