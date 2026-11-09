#include "Player.h"

#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <MovementComponent.h>
#include <SpriteDirectionComponent.h>
#include <SpriteMovementAnimationComponent.h>

namespace Roguelike
{
	Player::Player(const Engine::Vector2Df& position)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		if (const auto* playerTexture = Engine::ResourceSystem::Instance()->GetTextureShared("ball"))
		{
			renderer->SetTexture(*playerTexture);
		}
		renderer->SetPixelSize(100, 100);

		auto camera = gameObject->AddComponent<Engine::CameraComponent>();
		camera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());
		camera->SetBaseResolution(1280, 720);

		auto input = gameObject->AddComponent<Engine::InputComponent>();

		auto movement = gameObject->AddComponent<Engine::MovementComponent>();
		movement->SetSpeed(400.f);

		auto spriteDirection = gameObject->AddComponent<Engine::SpriteDirectionComponent>();

		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
		collider->SetCollisionLayer(Engine::CollisionLayer::Actor);
		collider->SetCollisionMask(Engine::CollisionLayer::World);

		auto animator = gameObject->AddComponent<Engine::SpriteMovementAnimationComponent>();
		animator->Initialize("player", 6.f);
	}

	Engine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}