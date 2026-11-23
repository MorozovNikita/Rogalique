#include "LocomotionAnimator.h"

#include "DirectionComponent.h"
#include "GameWorld.h"
#include "SpriteClipAnimatorComponent.h"
#include "SpriteRendererComponent.h"
#include "StatsComponent.h"

#include <stdexcept>

namespace Roguelike
{
LocomotionAnimator::LocomotionAnimator(Engine::GameObject *gameObject) : Engine::Component(gameObject)
{
    animator = gameObject->GetComponent<Engine::SpriteClipAnimatorComponent>();
    direction = gameObject->GetComponent<Engine::DirectionComponent>();
    if (animator == nullptr || direction == nullptr)
    {
        throw std::logic_error("Locomotion animator is missing direction or sprite clips");
    }
}

void LocomotionAnimator::SetMirrorHorizontal(bool mirror)
{
    mirrorHorizontal = mirror;
    renderer = m_gameObject->GetComponent<Engine::SpriteRendererComponent>();
    if (mirrorHorizontal && renderer == nullptr)
    {
        throw std::logic_error("Horizontal mirror requires a sprite renderer");
    }
}

void LocomotionAnimator::SetEnabled(bool isEnabled)
{
    enabled = isEnabled;
}

void LocomotionAnimator::Update(float)
{
    if (!enabled)
    {
        return;
    }

    const auto *stats = m_gameObject->GetComponent<Engine::StatsComponent>();
    if (animator->IsOneShotPlaying() || (stats != nullptr && stats->GetCurrentHealth() <= 0.f))
    {
        return;
    }

    const Engine::Vector2Df axis = {direction->GetHorizontalAxis(), direction->GetVerticalAxis()};
    const bool moving = axis.x != 0.f || axis.y != 0.f;
    animator->SetFacing(axis);
    animator->Play(moving ? "run" : "idle");
    if (!mirrorHorizontal || renderer == nullptr)
    {
        return;
    }

    if (axis.x < 0.f)
    {
        renderer->FlipX(true);
    }
    else if (axis.x > 0.f)
    {
        renderer->FlipX(false);
    }
}

void LocomotionAnimator::Render()
{
}
} // namespace Roguelike
