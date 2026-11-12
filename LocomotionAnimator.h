#pragma once

#include "Component.h"

namespace Engine
{
	class DirectionComponent;
	class SpriteClipAnimatorComponent;
}

namespace Roguelike
{
	class LocomotionAnimator : public Engine::Component
	{
	public:
		LocomotionAnimator(Engine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

	private:
		Engine::SpriteClipAnimatorComponent* animator = nullptr;
		Engine::DirectionComponent* direction = nullptr;
	};
}
