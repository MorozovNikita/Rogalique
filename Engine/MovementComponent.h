#pragma once
#include "GameObject.h"
#include "DirectionComponent.h"

namespace Engine
{
	class MovementComponent : public Component
	{
	public:
		MovementComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetSpeed(float newSpeed);
		float GetSpeed() const;
		float GetAccelerationSquared() const;
	private:
		DirectionComponent* direction = nullptr;
		TransformComponent* transform;

		float speed = 0;
		Vector2Df previousPosition = { 0, 0 };
		Vector2Df acceleration = { 0, 0 };
	};
}