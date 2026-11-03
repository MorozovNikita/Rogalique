#include "pch.h"
#include "SpriteDirectionComponent.h"

namespace Engine
{

	SpriteDirectionComponent::SpriteDirectionComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		direction = gameObject->GetComponent<DirectionComponent>();
		spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
	}

	void SpriteDirectionComponent::Update(float deltaTime)
	{
		if (direction == nullptr || spriteRenderer == nullptr)
		{
			return;
		}

		if (direction->GetHorizontalAxis() < 0)
		{
			spriteRenderer->FlipX(true);
		}
		if (direction->GetHorizontalAxis() > 0)
		{
			spriteRenderer->FlipX(false);
		}
	}

	void SpriteDirectionComponent::Render()
	{
	}

}