#include "Floor.h"

#include <stdexcept>
#include <string>

namespace Roguelike
{

	Floor::Floor(const Engine::Vector2Df& position, int textureMapIndex)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Floor");
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(position);

		const auto* texture = Engine::ResourceSystem::Instance()->GetTextureMapElementShared("level_floors", textureMapIndex);
		if (texture == nullptr)
		{
			throw std::runtime_error("Floor texture is missing at index " + std::to_string(textureMapIndex));
		}

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		renderer->SetTexture(*texture);
		renderer->SetPixelSize(128, 128);
	}

}