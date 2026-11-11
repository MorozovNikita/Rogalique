#pragma once

#include "Component.h"
#include "Vector.h"

namespace Engine
{
	class AttackComponent;
	class ConeAreaComponent;
	class DirectionComponent;
	class MovementComponent;
}

namespace Roguelike
{
	class PlayerAttackComponent : public Engine::Component
	{
	public:
		PlayerAttackComponent(Engine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

	private:
		void Strike();

		Engine::DirectionComponent* direction = nullptr;
		Engine::MovementComponent* movement = nullptr;
		Engine::AttackComponent* attack = nullptr;
		Engine::ConeAreaComponent* cone = nullptr;
		Engine::Vector2Df facing{ 0.f, -1.f };
		float runSpeed = 0.f;
		float swingTime = 0.f;
		bool spaceWasDown = false;
	};
}
