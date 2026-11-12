#include "pch.h"
#include "SpriteColliderComponent.h"

#include <stdexcept>

namespace Engine
{
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
	{
		renderer = gameObject->GetComponent<SpriteRendererComponent>();
		if (renderer == nullptr)
		{
			throw std::logic_error("SpriteRenderer required to SpriteCollider");
		}

		sprite = renderer->GetSprite();
		PhysicsSystem::Instance()->Subscribe(this);
	}
	SpriteColliderComponent::~SpriteColliderComponent()
	{
		if (PhysicsSystem::IsAlive())
		{
			PhysicsSystem::Instance()->Unsubscribe(this);
		}
	}

	void SpriteColliderComponent::Update(float deltaTime)
	{
		if (renderer == nullptr || sprite == nullptr)
		{
			return;
		}

		renderer->ApplyTransform();
		bounds = sprite->getGlobalBounds();

		const float width = bounds.size.x * boundsScaleX;
		const float height = bounds.size.y * boundsScaleY;
		bounds.position.x += (bounds.size.x - width) * 0.5f;
		bounds.position.y += (bounds.size.y - height) * 0.5f;
		bounds.size = { width, height };
	}

	void SpriteColliderComponent::SetBoundsScale(float scaleX, float scaleY)
	{
		if (scaleX <= 0.f || scaleY <= 0.f)
		{
			throw std::invalid_argument("Sprite collider scale must be positive");
		}

		boundsScaleX = scaleX;
		boundsScaleY = scaleY;
	}
	void SpriteColliderComponent::Render()
	{
		sf::RectangleShape rectangle(bounds.size);
		rectangle.setPosition(bounds.position);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(4);

		RenderSystem::Instance()->Render(rectangle);
	}
}