#pragma once

#include "Component.h"
#include "Vector.h"

namespace Engine
{
	class TransformComponent;

	class ConeAreaComponent : public Component
	{
	public:
		ConeAreaComponent(GameObject* gameObject);

		void SetRange(float newRange);
		void SetAngle(float degrees);
		void SetDirection(const Vector2Df& newDirection);
		void SetActive(bool isActive);

		bool IsActive() const;
		float GetRange() const;
		bool Contains(const Vector2Df& worldPoint) const;

		void Update(float deltaTime) override;
		void Render() override;

	private:
		TransformComponent* transform = nullptr;
		Vector2Df direction{ 0.f, -1.f };
		float range = 110.f;
		float halfAngle = 0.61f;
		bool active = false;
	};
}
