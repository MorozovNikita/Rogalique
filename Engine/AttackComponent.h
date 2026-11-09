#pragma once

#include "Component.h"

namespace Engine
{

    class AttackComponent : public Component
    {
    public:
        AttackComponent(GameObject* gameObject, float attackPower)
            : Component(gameObject), attackPower{ attackPower }
        {
        }

        float GetAttackPower() const;
        float SetAttackPower(float newAttackPower);

        void Attack(GameObject* target);

        void Update(float deltaTime) override {}
        void Render() override {}
    private:
        float attackPower;

        float CalculateDamage() const;
    };

}
