#include "pch.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"

namespace Engine
{
	SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject)
		: Component(gameObject)
		, placeholder(sf::Vector2u{ 1, 1 })
	{
		sprite = new sf::Sprite(placeholder);
		scale = { 1, -1 };
		sprite->setScale({ 1, -1 });
		transform = gameObject->GetComponent<TransformComponent>();
	}
	SpriteRendererComponent::~SpriteRendererComponent()
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}

	void SpriteRendererComponent::ApplyTransform()
	{
		if (sprite == nullptr || transform == nullptr)
		{
			return;
		}

		sprite->setPosition(Convert<sf::Vector2f, Vector2Df>(transform->GetWorldPosition()));
		sprite->setRotation(sf::degrees(transform->GetWorldRotation()));

		auto transformScale = Convert<sf::Vector2f, Vector2Df>(transform->GetWorldScale());
		sprite->setScale({ scale.x * transformScale.x, scale.y * transformScale.y });
	}

	void SpriteRendererComponent::Update(float deltaTime)
	{
		ApplyTransform();
	}
	void SpriteRendererComponent::Render()
	{
		if (sprite != nullptr)
		{
			ApplyTransform();
			RenderSystem::Instance()->Render(*sprite);
		}
	}

	const sf::Sprite* SpriteRendererComponent::GetSprite() const
	{
		return sprite;
	}
	void SpriteRendererComponent::SetTexture(const sf::Texture& newTexture)
	{
		sprite->setTexture(newTexture, true);
		const auto textureSize = sprite->getTexture().getSize();
		sprite->setOrigin({ 0.5f * textureSize.x, 0.5f * textureSize.y });
	}
	void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight)
	{
		const auto originalSize = sprite->getTexture().getSize();
		scale = { (float)newWidth / (float)originalSize.x, -(float)newHeight / (float)originalSize.y };
	}

	void SpriteRendererComponent::FlipX(bool flip)
	{
		if (flip != isFlipX)
		{
			scale = { -scale.x, scale.y };
			isFlipX = flip;
		}
	}
	void SpriteRendererComponent::FlipY(bool flip)
	{
		if (flip != isFlipY)
		{
			scale = { scale.x, -scale.y };
			isFlipY = flip;
		}
	}
}