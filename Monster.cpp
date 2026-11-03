#include "Monster.h"

#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <ChaseComponent.h>
#include <MovementComponent.h>
#include <SpriteDirectionComponent.h>
#include <SpriteMovementAnimationComponent.h>

namespace Roguelike
{
	Monster::Monster(const Engine::Vector2Df& position)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Monster");
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		if (const auto* monsterTexture = Engine::ResourceSystem::Instance()->GetTextureShared("monster"))
		{
			renderer->SetTexture(*monsterTexture);
		}
		renderer->SetPixelSize(100, 100);

		gameObject->AddComponent<Engine::ChaseComponent>();

		auto movement = gameObject->AddComponent<Engine::MovementComponent>();
		movement->SetSpeed(450.f);

		auto spriteDirection = gameObject->AddComponent<Engine::SpriteDirectionComponent>();

		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();

		auto animator = gameObject->AddComponent<Engine::SpriteMovementAnimationComponent>();
		animator->Initialize("monster", 6.f);
	}

	Engine::GameObject* Monster::GetGameObject()
	{
		return gameObject;
	}
}