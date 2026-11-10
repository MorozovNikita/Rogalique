#include "pch.h"
#include "ConeAreaComponent.h"

#include "RenderSystem.h"
#include "TransformComponent.h"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <stdexcept>

namespace Engine
{
	ConeAreaComponent::ConeAreaComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		transform = gameObject->GetComponent<TransformComponent>();
		if (transform == nullptr)
		{
			throw std::logic_error("Cone area requires a transform");
		}
	}

	void ConeAreaComponent::SetRange(float newRange)
	{
		if (newRange <= 0.f)
		{
			throw std::invalid_argument("Cone range must be positive");
		}

		range = newRange;
	}

	void ConeAreaComponent::SetAngle(float degrees)
	{
		if (degrees <= 0.f || degrees > 180.f)
		{
			throw std::invalid_argument("Cone angle must be between 0 and 180 degrees");
		}

		halfAngle = degrees * (3.14159265f / 360.f);
	}

	void ConeAreaComponent::SetDirection(const Vector2Df& newDirection)
	{
		direction = newDirection;
	}

	void ConeAreaComponent::SetActive(bool isActive)
	{
		active = isActive;
	}

	bool ConeAreaComponent::IsActive() const
	{
		return active;
	}

	float ConeAreaComponent::GetRange() const
	{
		return range;
	}

	bool ConeAreaComponent::Contains(const Vector2Df& worldPoint) const
	{
		if (transform == nullptr)
		{
			return false;
		}

		const Vector2Df origin = transform->GetWorldPosition();
		const float offsetX = worldPoint.x - origin.x;
		const float offsetY = worldPoint.y - origin.y;
		const float distanceSquared = offsetX * offsetX + offsetY * offsetY;
		if (distanceSquared > range * range)
		{
			return false;
		}

		if (distanceSquared < 1.f)
		{
			return true;
		}

		const float directionLengthSquared = direction.x * direction.x + direction.y * direction.y;
		if (directionLengthSquared < 0.0001f)
		{
			return false;
		}

		const float dot = offsetX * direction.x + offsetY * direction.y;
		const float cosHalf = std::cos(halfAngle);
		return dot >= cosHalf * std::sqrt(distanceSquared * directionLengthSquared);
	}

	void ConeAreaComponent::Update(float)
	{
	}

	void ConeAreaComponent::Render()
	{
#if !defined(NDEBUG)
		if (!active || transform == nullptr)
		{
			return;
		}

		const float directionLengthSquared = direction.x * direction.x + direction.y * direction.y;
		if (directionLengthSquared < 0.0001f)
		{
			return;
		}

		const float facing = std::atan2(direction.y, direction.x);
		constexpr int segments = 10;
		sf::ConvexShape shape;
		shape.setPointCount(static_cast<std::size_t>(segments + 2));
		shape.setPoint(0, { 0.f, 0.f });
		for (int index = 0; index <= segments; ++index)
		{
			const float t = static_cast<float>(index) / static_cast<float>(segments);
			const float angle = facing - halfAngle + t * (halfAngle * 2.f);
			shape.setPoint(static_cast<std::size_t>(index + 1), { std::cos(angle) * range, std::sin(angle) * range });
		}

		const Vector2Df position = transform->GetWorldPosition();
		shape.setPosition({ position.x, position.y });
		shape.setFillColor(sf::Color(220, 40, 40, 70));
		shape.setOutlineColor(sf::Color(220, 40, 40, 220));
		shape.setOutlineThickness(2.f);
		RenderSystem::Instance()->Render(shape);
#endif
	}
}
