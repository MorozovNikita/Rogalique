#pragma once

#include "Component.h"
#include "Vector.h"

#include <string>

namespace Engine
{
	class ChaseComponent;
	class ConeAreaComponent;
	class DirectionComponent;
	class MovementComponent;
	class SpriteClipAnimatorComponent;

	enum class AttackTrigger
	{
		Input,
		Automatic
	};

	class AttackComponent : public Component
	{
	public:
		AttackComponent(GameObject* gameObject, float attackPower, AttackTrigger trigger);

		float GetAttackPower() const;
		float SetAttackPower(float newAttackPower);
		void SetAttackClip(const std::string& newClipName);
		void SetSwingDuration(float seconds);

		void Update(float deltaTime) override;
		void Render() override;

	private:
		void Aim();
		void BeginSwing();
		void Resolve();
		float CalculateDamage() const;
		bool ChaseTargetInCone() const;
		bool IsClipSwinging() const;

		AttackTrigger trigger;
		ConeAreaComponent* cone = nullptr;
		DirectionComponent* direction = nullptr;
		MovementComponent* movement = nullptr;
		ChaseComponent* chase = nullptr;
		SpriteClipAnimatorComponent* animator = nullptr;
		std::string clipName;
		Vector2Df facing{ 0.f, -1.f };
		float attackPower = 0.f;
		float runSpeed = 0.f;
		float swingDuration = 0.35f;
		float swingTime = 0.f;
		bool spaceWasDown = false;
		bool strikePending = false;
	};
}
