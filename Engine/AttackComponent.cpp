#include "pch.h"
#include "AttackComponent.h"

#include "GameObject.h"
#include "StatsComponent.h"

namespace Engine
{
    float AttackComponent::GetAttackPower() const
    {
        return attackPower;
    }

    float AttackComponent::SetAttackPower(float newAttackPower)
    {
        attackPower = newAttackPower;
        return attackPower;
    }

    void AttackComponent::Attack(GameObject* target)
    {
        if (!target)
            return;

        auto targetStatsComponent = target->GetComponent<StatsComponent>();
        if (targetStatsComponent)
        {
            float damage = CalculateDamage();
            targetStatsComponent->TakeDamage(damage);
        }
    }

    float AttackComponent::CalculateDamage() const
    {
        // There might be many factors
        return attackPower;
    }

}
