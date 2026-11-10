#pragma once

#include "Component.h"

#include <string>

namespace Engine
{
	class AttackComponent;
	class ChaseComponent;
	class ConeAreaComponent;
	class MovementComponent;
	class SpriteClipAnimatorComponent;
}

namespace Roguelike
{
	class MeleeAttackComponent : public Engine::Component
	{
	public:
		MeleeAttackComponent(Engine::GameObject* gameObject, std::string clipName);

		void Update(float deltaTime) override;
		void Render() override;

	private:
		void Strike(Engine::GameObject* target);

		Engine::SpriteClipAnimatorComponent* animator = nullptr;
		Engine::MovementComponent* movement = nullptr;
		Engine::AttackComponent* attack = nullptr;
		Engine::ConeAreaComponent* cone = nullptr;
		Engine::ChaseComponent* chase = nullptr;
		std::string clipName;
		float runSpeed = 0.f;
	};
}
