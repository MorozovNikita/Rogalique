#include "Wall.h"

#include <SpriteColliderComponent.h>

#include <stdexcept>
#include <string>

namespace Roguelike
{

	Wall::Wall(const Engine::Vector2Df& position, int textureMapIndex)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Wall");
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(position);

		const auto* texture = Engine::ResourceSystem::Instance()->GetTextureMapElementShared("level_walls", textureMapIndex);
		if (texture == nullptr)
		{
			throw std::runtime_error("Wall texture is missing at index " + std::to_string(textureMapIndex));
		}

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		renderer->SetTexture(*texture);
		renderer->SetPixelSize(128, 128);

		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		rigidbody->SetKinematic(true);

		auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
	}

}