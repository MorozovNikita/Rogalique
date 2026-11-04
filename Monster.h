#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "Vector.h"

namespace Engine
{
	struct ChaseSettings;
}

namespace Roguelike
{
	class Monster
	{
	public:
		Monster(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings);
		Engine::GameObject* GetGameObject();
	private:
		Engine::GameObject* gameObject;
	};
}