#include "MeleeAttackComponent.h"

#include "AttackComponent.h"
#include "ChaseComponent.h"
#include "ConeAreaComponent.h"
#include "Logger.h"
#include "MovementComponent.h"
#include "SpriteClipAnimatorComponent.h"
#include "StatsComponent.h"
#include "TransformComponent.h"

#include <stdexcept>

namespace Roguelike
{
	MeleeAttackComponent::MeleeAttackComponent(Engine::GameObject* gameObject, std::string clipName)
		: Engine::Component(gameObject)
		, clipName(std::move(clipName))
	{
		animator = gameObject->GetComponent<Engine::SpriteClipAnimatorComponent>();
		movement = gameObject->GetComponent<Engine::MovementComponent>();
		attack = gameObject->GetComponent<Engine::AttackComponent>();
		cone = gameObject->GetComponent<Engine::ConeAreaComponent>();
		chase = gameObject->GetComponent<Engine::ChaseComponent>();
		if (animator == nullptr || movement == nullptr || attack == nullptr || cone == nullptr || chase == nullptr)
		{
			throw std::logic_error("Melee attack is missing chase, movement, attack, cone, or sprite clips");
		}

		runSpeed = movement->GetSpeed();
	}

	void MeleeAttackComponent::Update(float)
	{
		const bool swinging = animator->IsOneShotPlaying();
		cone->SetDirection(animator->GetFacing());

		Engine::GameObject* target = chase->GetTarget();
		auto* targetTransform = target != nullptr ? target->GetComponent<Engine::TransformComponent>() : nullptr;
		auto* targetStats = target != nullptr ? target->GetComponent<Engine::StatsComponent>() : nullptr;
		const bool targetAlive = targetTransform != nullptr && targetStats != nullptr && targetStats->GetCurrentHealth() > 0.f;
		const bool targetInCone = targetAlive && cone->Contains(targetTransform->GetWorldPosition());

		if (!swinging)
		{
			if (targetInCone)
			{
				movement->SetSpeed(0.f);
				animator->Play(clipName, true);
			}
			else
			{
				movement->SetSpeed(runSpeed);
			}
		}
		else
		{
			movement->SetSpeed(0.f);
		}

		const bool attackVisible = animator->IsPlaying(clipName) && !animator->IsFinished();
		cone->SetActive(attackVisible);

		if (animator->ConsumeAction() && targetInCone)
		{
			Strike(target);
		}
	}

	void MeleeAttackComponent::Render()
	{
	}

	void MeleeAttackComponent::Strike(Engine::GameObject* target)
	{
		attack->Attack(target);
		LOG_INFO(m_gameObject->GetName() + " hit the player");
	}
}
