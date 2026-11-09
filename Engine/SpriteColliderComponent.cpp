#include "pch.h"
#include "SpriteColliderComponent.h"

namespace Engine
{
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
	{
		renderer = gameObject->GetComponent<SpriteRendererComponent>();
		if (renderer == nullptr)
		{
			std::cout << "SpriteRenderer required to SpriteCollider." << std::endl;
			return;
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