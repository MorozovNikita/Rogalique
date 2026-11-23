#include "LevelGateAnimator.h"

#include "LocomotionAnimator.h"

#include <AttackComponent.h>
#include <ColliderComponent.h>
#include <MovementComponent.h>
#include <SpriteClipAnimatorComponent.h>
#include <SpriteRendererComponent.h>
#include <StatsComponent.h>
#include <TransformComponent.h>

#include <stdexcept>

namespace Roguelike
{
namespace
{
constexpr float kPitBelowCenter = 0.25f;
}

LevelGateAnimator::LevelGateAnimator(Engine::GameObject *gameObject) : Engine::Component(gameObject)
{
    transform = gameObject->GetComponent<Engine::TransformComponent>();
    renderer = gameObject->GetComponent<Engine::SpriteRendererComponent>();
    collider = gameObject->GetComponent<Engine::ColliderComponent>();
    animator = gameObject->GetComponent<Engine::SpriteClipAnimatorComponent>();
    movement = gameObject->GetComponent<Engine::MovementComponent>();
    attack = gameObject->GetComponent<Engine::AttackComponent>();
    stats = gameObject->GetComponent<Engine::StatsComponent>();
    locomotion = gameObject->GetComponent<LocomotionAnimator>();
    if (transform == nullptr || renderer == nullptr || collider == nullptr || animator == nullptr ||
        movement == nullptr || attack == nullptr || stats == nullptr || locomotion == nullptr)
    {
        throw std::logic_error("Level gate requires the player movement stack");
    }

    runSpeed = movement->GetSpeed();
}

void LevelGateAnimator::BeginEmerge(float newTileSize, float newSpriteHeight, float newOriginFractionY, float seconds)
{
    if (newTileSize <= 0.f || newSpriteHeight <= 0.f || seconds <= 0.f)
    {
        throw std::invalid_argument("Level gate emerge size must be positive");
    }

    tileSize = newTileSize;
    spriteHeight = newSpriteHeight;
    originFractionY = newOriginFractionY;
    duration = seconds;
    elapsed = 0.f;
    phase = Phase::Emerging;

    const float standY = transform->GetWorldPosition().y;
    clipY = standY - tileSize * kPitBelowCenter;
    buriedOffset = clipY - standY - originFractionY * spriteHeight;
    HoldStill();
    ApplyPose(buriedOffset);
}

void LevelGateAnimator::SetStairs(const Engine::Vector2Df &position, float newTileSize)
{
    if (newTileSize <= 0.f)
    {
        throw std::invalid_argument("Stair tile size must be positive");
    }

    stairs = position;
    tileSize = newTileSize;
    hasStairs = true;
}

bool LevelGateAnimator::ConsumeLevelChange()
{
    if (!levelChangePending)
    {
        return false;
    }

    levelChangePending = false;
    return true;
}

void LevelGateAnimator::Update(float deltaTime)
{
    if (phase == Phase::Done)
    {
        return;
    }

    if (stats->GetCurrentHealth() <= 0.f)
    {
        if (phase != Phase::Playing)
        {
            ClearPose();
            phase = Phase::Playing;
        }
        return;
    }

    if (phase == Phase::Emerging || phase == Phase::Descending)
    {
        HoldStill();
        elapsed += deltaTime;
        float progress = elapsed / duration;
        if (progress > 1.f)
        {
            progress = 1.f;
        }
        const float eased = progress * progress * (3.f - 2.f * progress);
        const float offsetY = phase == Phase::Emerging ? buriedOffset * (1.f - eased) : buriedOffset * eased;
        ApplyPose(offsetY);
        if (progress < 1.f)
        {
            return;
        }

        if (phase == Phase::Emerging)
        {
            phase = Phase::Playing;
            ClearPose();
            Release();
            return;
        }

        phase = Phase::Done;
        levelChangePending = true;
        ApplyPose(buriedOffset);
        return;
    }

    if (hasStairs && StandsOnStairs())
    {
        BeginDescend();
    }
}

void LevelGateAnimator::Render()
{
}

void LevelGateAnimator::HoldStill()
{
    movement->SetSpeed(0.f);
    attack->SetEnabled(false);
    locomotion->SetEnabled(false);
    animator->Play("idle");
}

void LevelGateAnimator::Release()
{
    attack->SetEnabled(true);
    locomotion->SetEnabled(true);
    movement->SetSpeed(runSpeed);
}

void LevelGateAnimator::ApplyPose(float offsetY)
{
    const Engine::Vector2Df previous = renderer->GetDrawOffset();
    collider->ShiftBounds(-previous.x, -previous.y);
    renderer->SetDrawOffset({0.f, offsetY});

    const float spriteTop = transform->GetWorldPosition().y + offsetY + originFractionY * spriteHeight;
    float fraction = (spriteTop - clipY) / spriteHeight;
    if (fraction < 0.f)
    {
        fraction = 0.f;
    }
    if (fraction > 1.f)
    {
        fraction = 1.f;
    }
    renderer->SetVisibleTopFraction(fraction);
}

void LevelGateAnimator::ClearPose()
{
    const Engine::Vector2Df previous = renderer->GetDrawOffset();
    collider->ShiftBounds(-previous.x, -previous.y);
    renderer->SetDrawOffset({0.f, 0.f});
    renderer->SetVisibleTopFraction(1.f);
}

void LevelGateAnimator::BeginDescend()
{
    elapsed = 0.f;
    phase = Phase::Descending;
    clipY = stairs.y - tileSize * kPitBelowCenter;
    buriedOffset = clipY - transform->GetWorldPosition().y - originFractionY * spriteHeight;
    HoldStill();
}

bool LevelGateAnimator::StandsOnStairs() const
{
    const Engine::Vector2Df position = transform->GetWorldPosition();
    const float offsetX = position.x - stairs.x;
    const float offsetY = position.y - stairs.y;
    const float reach = tileSize * 0.4f;
    return offsetX * offsetX + offsetY * offsetY <= reach * reach;
}
} // namespace Roguelike
