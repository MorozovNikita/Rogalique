#pragma once

#include "Component.h"

namespace Engine
{
	class DirectionComponent : public Component
	{
	public:
		DirectionComponent(GameObject* gameObject) : Component(gameObject) {}

		virtual float GetHorizontalAxis() const = 0;
		virtual float GetVerticalAxis() const = 0;
	};
}
