#include "pch.h"
#include "AttackComponent.h"

#include "ChaseComponent.h"
#include "ColliderComponent.h"
#include "ConeAreaComponent.h"
#include "DirectionComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Logger.h"
#include "MovementComponent.h"
#include "SpriteClipAnimatorComponent.h"
#include "StatsComponent.h"
#include "TransformComponent.h"

#include <SFML/Window.hpp>

#include <stdexcept>

namespace Engine
{
AttackComponent::AttackComponent(GameObject *gameObject, float attackPower, AttackTrigger trigger)
    : Component(gameObject), trigger(trigger), attackPower(attackPower)
{
    cone = gameObject->GetComponent<ConeAreaComponent>();
    movement = gameObject->GetComponent<MovementComponent>();
    direction = gameObject->GetComponent<DirectionComponent>();
    chase = gameObject->GetComponent<ChaseComponent>();
    animator = gameObject->GetComponent<SpriteClipAnimatorComponent>();
    if (cone == nullptr || movement == nullptr)
    {
        throw std::logic_error("Attack requires a cone and movement");
    }
    if (trigger == AttackTrigger::Input && direction == nullptr)
    {
        throw std::logic_error("Input attack requires a direction");
    }
    if (trigger == AttackTrigger::Automatic && chase == nullptr)
    {
        throw std::logic_error("Automatic attack requires a chase target");
    }

    runSpeed = movement->GetSpeed();
}

float AttackComponent::GetAttackPower() const
{
    return attackPower;
}

float AttackComponent::SetAttackPower(float newAttackPower)
{
    attackPower = newAttackPower;
    return attackPower;
}

void AttackComponent::SetAttackClip(const std::string &newClipName)
{
    if (newClipName.empty() || animator == nullptr)
    {
        throw std::logic_error("Attack clip requires sprite clips");
    }

    clipName = newClipName;
}

void AttackComponent::SetSwingDuration(float seconds)
{
    if (seconds <= 0.f)
    {
        throw std::invalid_argument("Attack swing duration must be positive");
    }

    swingDuration = seconds;
}

void AttackComponent::Update(float deltaTime)
{
    if (IsOwnerDead())
    {
        movement->SetSpeed(0.f);
        cone->SetActive(false);
        return;
    }

    Aim();

    if (trigger == AttackTrigger::Input)
    {
        const bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        const bool spacePressed = spaceDown && !spaceWasDown;
        spaceWasDown = spaceDown;
        if (spacePressed && swingTime <= 0.f && !IsClipSwinging())
        {
            BeginSwing();
        }
    }
    else if (!IsClipSwinging() && swingTime <= 0.f && ChaseTargetInCone())
    {
        BeginSwing();
    }

    const bool swinging = IsClipSwinging() || swingTime > 0.f;
    cone->SetActive(swinging);
    if (!swinging)
    {
        movement->SetSpeed(runSpeed);
        return;
    }

    movement->SetSpeed(0.f);
    if (swingTime > 0.f)
    {
        swingTime -= deltaTime;
        if (swingTime <= 0.f)
        {
            swingTime = 0.f;
            cone->SetActive(IsClipSwinging());
            if (!IsClipSwinging())
            {
                movement->SetSpeed(runSpeed);
            }
        }
    }

    if (strikePending && animator != nullptr && animator->ConsumeAction())
    {
        strikePending = false;
        if (trigger == AttackTrigger::Input || ChaseTargetInCone())
        {
            Resolve();
        }
    }
}

void AttackComponent::Render()
{
}

void AttackComponent::Aim()
{
    if (trigger == AttackTrigger::Automatic && animator != nullptr)
    {
        cone->SetDirection(animator->GetFacing());
        return;
    }

    if (swingTime > 0.f || IsClipSwinging())
    {
        cone->SetDirection(facing);
        return;
    }

    if (direction == nullptr)
    {
        cone->SetDirection(facing);
        return;
    }

    const Vector2Df axis = {direction->GetHorizontalAxis(), direction->GetVerticalAxis()};
    if (axis.x != 0.f || axis.y != 0.f)
    {
        facing = axis;
    }
    cone->SetDirection(facing);
}

void AttackComponent::BeginSwing()
{
    movement->SetSpeed(0.f);
    if (!clipName.empty())
    {
        animator->Play(clipName, true);
        strikePending = true;
        return;
    }

    swingTime = swingDuration;
    Resolve();
}

float AttackComponent::CalculateDamage() const
{
    return attackPower;
}

void AttackComponent::Resolve()
{
    for (GameObject *target : GameWorld::Instance()->GetGameObjects())
    {
        if (target == nullptr || target == m_gameObject)
        {
            continue;
        }

        auto *stats = target->GetComponent<StatsComponent>();
        if (stats == nullptr || stats->GetCurrentHealth() <= 0.f || !TargetInCone(target))
        {
            continue;
        }

        stats->TakeDamage(CalculateDamage());
        LOG_INFO(m_gameObject->GetName() + " hit " + target->GetName());
    }
}

bool AttackComponent::IsOwnerDead() const
{
    const auto *stats = m_gameObject->GetComponent<StatsComponent>();
    return stats != nullptr && stats->GetCurrentHealth() <= 0.f;
}

bool AttackComponent::ChaseTargetInCone() const
{
    if (chase == nullptr || chase->GetTarget() == nullptr)
    {
        return false;
    }

    GameObject *target = chase->GetTarget();
    auto *stats = target->GetComponent<StatsComponent>();
    if (stats == nullptr || stats->GetCurrentHealth() <= 0.f)
    {
        return false;
    }

    return TargetInCone(target);
}

bool AttackComponent::TargetInCone(GameObject *target) const
{
    if (target == nullptr)
    {
        return false;
    }

    auto *collider = target->GetComponent<ColliderComponent>();
    if (collider != nullptr)
    {
        const sf::FloatRect bounds = collider->GetBounds();
        if (bounds.size.x > 0.f && bounds.size.y > 0.f)
        {
            return cone->Intersects(bounds);
        }
    }

    auto *targetTransform = target->GetComponent<TransformComponent>();
    if (targetTransform == nullptr)
    {
        return false;
    }

    return cone->Contains(targetTransform->GetWorldPosition());
}

bool AttackComponent::IsClipSwinging() const
{
    return !clipName.empty() && animator != nullptr && animator->IsPlaying(clipName) && !animator->IsFinished();
}
} // namespace Engine
