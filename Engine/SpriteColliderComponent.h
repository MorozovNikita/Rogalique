#pragma once

#include "ColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Engine
{
	class SpriteColliderComponent : public ColliderComponent
	{
	public:
		SpriteColliderComponent(GameObject* gameObject);
		~SpriteColliderComponent();

		void SetBoundsScale(float scaleX, float scaleY);

		void Update(float deltaTime) override;
		void Render() override;
	private:
		SpriteRendererComponent* renderer = nullptr;
		const sf::Sprite* sprite = nullptr;
		float boundsScaleX = 1.f;
		float boundsScaleY = 1.f;
	};
}
