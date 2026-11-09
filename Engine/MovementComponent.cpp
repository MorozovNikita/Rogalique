#include "pch.h"
#include "MovementComponent.h"

#include <cmath>

namespace Engine
{

	MovementComponent::MovementComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		direction = gameObject->GetComponent<DirectionComponent>();
		transform = gameObject->GetComponent<TransformComponent>();
	}

	void MovementComponent::Update(float deltaTime)
	{
		Vector2Df axis = { 0.f, 0.f };
		if (direction != nullptr)
		{
			axis = { direction->GetHorizontalAxis(), direction->GetVerticalAxis() };
		}

		const float lengthSquared = axis.x * axis.x + axis.y * axis.y;
		if (lengthSquared > 1.f)
		{
			const float inverseLength = 1.f / std::sqrt(lengthSquared);
			axis.x *= inverseLength;
			axis.y *= inverseLength;
		}

		transform->MoveBy(speed * deltaTime * axis);

		acceleration = transform->GetWorldPosition() - previousPosition;
		previousPosition = transform->GetWorldPosition();
	}

	void MovementComponent::Render()
	{
	}

	void MovementComponent::SetSpeed(float newSpeed)
	{
		speed = newSpeed;
	}

	float MovementComponent::GetSpeed() const
	{
		return speed;
	}

	float MovementComponent::GetAccelerationSquared() const
	{
		return acceleration.x * acceleration.x + acceleration.y * acceleration.y;
	}

}