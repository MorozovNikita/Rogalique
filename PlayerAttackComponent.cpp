#include "PlayerAttackComponent.h"

#include "AttackComponent.h"
#include "ConeAreaComponent.h"
#include "DirectionComponent.h"
#include "GameWorld.h"
#include "Logger.h"
#include "MovementComponent.h"
#include "StatsComponent.h"
#include "TransformComponent.h"

#include <SFML/Window.hpp>

#include <stdexcept>

namespace Roguelike
{
	namespace
	{
		constexpr float SwingDuration = 0.35f;
	}

	PlayerAttackComponent::PlayerAttackComponent(Engine::GameObject* gameObject)
		: Engine::Component(gameObject)
	{
		direction = gameObject->GetComponent<Engine::DirectionComponent>();
		movement = gameObject->GetComponent<Engine::MovementComponent>();
		attack = gameObject->GetComponent<Engine::AttackComponent>();
		cone = gameObject->GetComponent<Engine::ConeAreaComponent>();
		if (direction == nullptr || movement == nullptr || attack == nullptr || cone == nullptr)
		{
			throw std::logic_error("Player attack is missing direction, movement, attack, or cone");
		}

		runSpeed = movement->GetSpeed();
	}

	void PlayerAttackComponent::Update(float deltaTime)
	{
		if (swingTime <= 0.f)
		{
			const Engine::Vector2Df axis = { direction->GetHorizontalAxis(), direction->GetVerticalAxis() };
			if (axis.x != 0.f || axis.y != 0.f)
			{
				facing = axis;
			}
		}

		const bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		const bool spacePressed = spaceDown && !spaceWasDown;
		spaceWasDown = spaceDown;

		if (spacePressed && swingTime <= 0.f)
		{
			swingTime = SwingDuration;
			movement->SetSpeed(0.f);
			cone->SetDirection(facing);
			cone->SetActive(true);
			Strike();
		}

		if (swingTime <= 0.f)
		{
			return;
		}

		swingTime -= deltaTime;
		cone->SetDirection(facing);
		cone->SetActive(true);
		movement->SetSpeed(0.f);
		if (swingTime <= 0.f)
		{
			swingTime = 0.f;
			cone->SetActive(false);
			movement->SetSpeed(runSpeed);
		}
	}

	void PlayerAttackComponent::Render()
	{
	}

	void PlayerAttackComponent::Strike()
	{
		const auto targets = Engine::GameWorld::Instance()->FindGameObjects("Vampire");
		for (Engine::GameObject* target : targets)
		{
			if (target == nullptr)
			{
				continue;
			}

			auto* stats = target->GetComponent<Engine::StatsComponent>();
			auto* targetTransform = target->GetComponent<Engine::TransformComponent>();
			if (stats == nullptr || targetTransform == nullptr || stats->GetCurrentHealth() <= 0.f)
			{
				continue;
			}

			if (!cone->Contains(targetTransform->GetWorldPosition()))
			{
				continue;
			}

			attack->Attack(target);
			LOG_INFO("Player hit " + target->GetName());
			if (stats->GetCurrentHealth() <= 0.f)
			{
				Engine::GameWorld::Instance()->DestroyGameObject(target);
			}
		}
	}
}
