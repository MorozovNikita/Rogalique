#pragma once

#include "DirectionComponent.h"

#include <SFML/Window.hpp> 

namespace Engine
{
	class InputComponent : public DirectionComponent
	{
	public:
		InputComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		float GetHorizontalAxis() const override;
		float GetVerticalAxis() const override;
	private:
		float horizontalAxis = 0.f;
		float verticalAxis = 0.f;
	};
}