#include "pch.h"
#include "InputComponent.h"

namespace Engine
{
	InputComponent::InputComponent(GameObject* gameObject) : DirectionComponent(gameObject) {}

	void InputComponent::Update(float deltaTime)
	{
		verticalAxis = 0.f;
		horizontalAxis = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			verticalAxis += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			verticalAxis -= 1.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			horizontalAxis += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			horizontalAxis -= 1.0f;
		}
	}
	void InputComponent::Render()
	{

	}

	float InputComponent::GetHorizontalAxis() const
	{
		return horizontalAxis;
	}
	float InputComponent::GetVerticalAxis() const
	{
		return verticalAxis;
	}
}