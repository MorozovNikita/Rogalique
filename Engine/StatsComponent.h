#pragma once

#include "Component.h"

#include <numeric>

namespace Engine
{

	class StatsComponent : public Component
	{
	public:
		StatsComponent(GameObject* gameObject, float health, float armor)
			: Component(gameObject), maxHealth{ health }, currentHealth{ health }, armor{ armor }
		{
		}

		float GetCurrentHealth() const { return currentHealth; }
		float GetMaxHealth() const { return maxHealth; }
		float GetArmor() const { return armor; }

		void TakeDamage(float damage)
		{
			damage -= armor;
			damage = std::max(damage, 0.f);

			--armor;
			armor = std::max(armor, 0.f);

			currentHealth -= damage;
			currentHealth = std::max(currentHealth, 0.f);
		}

		void Heal(float amount)
		{
			currentHealth += amount;
			if (currentHealth > maxHealth)
				currentHealth = maxHealth;
		}

		void Update(float deltaTime) override {}
		void Render() override {}

	private:
		float maxHealth;
		float currentHealth;
		float armor;
	};

}