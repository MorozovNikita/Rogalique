#include "pch.h"
#include "MovementComponent.h"

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