#include "LocomotionAnimator.h"

#include "DirectionComponent.h"
#include "GameWorld.h"
#include "SpriteClipAnimatorComponent.h"

#include <stdexcept>

namespace Roguelike
{
	LocomotionAnimator::LocomotionAnimator(Engine::GameObject* gameObject)
		: Engine::Component(gameObject)
	{
		animator = gameObject->GetComponent<Engine::SpriteClipAnimatorComponent>();
		direction = gameObject->GetComponent<Engine::DirectionComponent>();
		if (animator == nullptr || direction == nullptr)
		{
			throw std::logic_error("Locomotion animator is missing direction or sprite clips");
		}
	}

	void LocomotionAnimator::Update(float)
	{
		if (animator->IsOneShotPlaying())
		{
			return;
		}

		const Engine::Vector2Df axis = { direction->GetHorizontalAxis(), direction->GetVerticalAxis() };
		const bool moving = axis.x != 0.f || axis.y != 0.f;
		animator->SetFacing(axis);
		animator->Play(moving ? "run" : "idle");
	}

	void LocomotionAnimator::Render()
	{
	}
}
