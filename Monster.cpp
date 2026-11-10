#include "Monster.h"

#include "LocomotionAnimator.h"
#include "MeleeAttackComponent.h"

#include <AttackComponent.h>
#include <ChaseComponent.h>
#include <ConeAreaComponent.h>
#include <MovementComponent.h>
#include <SpriteClipAnimatorComponent.h>
#include <SpriteColliderComponent.h>
#include <StatsComponent.h>

namespace Roguelike
{
	Monster::Monster(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings, const MonsterAppearance& appearance)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Vampire");
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		auto animator = gameObject->AddComponent<Engine::SpriteClipAnimatorComponent>();
		animator->AddClip("idle", appearance.idle, 4, 4, 0.16f, true, -1);
		animator->AddClip("run", appearance.run, 8, 4, 0.08f, true, -1);
		animator->AddClip("attack", appearance.attack, 12, 4, 0.07f, false, 7);
		animator->Play("idle");
		renderer->SetPixelSize(160, 160);

		auto chase = gameObject->AddComponent<Engine::ChaseComponent>();
		chase->Initialize(chaseSettings);

		auto movement = gameObject->AddComponent<Engine::MovementComponent>();
		movement->SetSpeed(450.f);

		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
		collider->SetCollisionLayer(Engine::CollisionLayer::Actor);
		collider->SetCollisionMask(Engine::CollisionLayer::World);
		collider->SetBoundsScale(0.4f, 0.48f);

		gameObject->AddComponent<Engine::StatsComponent>(40.f, 0.f);
		gameObject->AddComponent<Engine::AttackComponent>(60.f);

		auto cone = gameObject->AddComponent<Engine::ConeAreaComponent>();
		cone->SetRange(110.f);
		cone->SetAngle(70.f);

		gameObject->AddComponent<LocomotionAnimator>();
		gameObject->AddComponent<MeleeAttackComponent>("attack");
	}

	Engine::GameObject* Monster::GetGameObject()
	{
		return gameObject;
	}
}
