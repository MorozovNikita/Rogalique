#pragma once

#include "Component.h"
#include "Vector.h"

namespace Engine
{
class AttackComponent;
class ColliderComponent;
class MovementComponent;
class SpriteClipAnimatorComponent;
class SpriteRendererComponent;
class StatsComponent;
class TransformComponent;
} // namespace Engine

namespace Roguelike
{
class LocomotionAnimator;

class LevelGateAnimator : public Engine::Component
{
  public:
    explicit LevelGateAnimator(Engine::GameObject *gameObject);

    void BeginEmerge(float tileSize, float spriteHeight, float originFractionY, float seconds);
    void SetStairs(const Engine::Vector2Df &position, float tileSize);
    bool ConsumeLevelChange();

    void Update(float deltaTime) override;
    void Render() override;

  private:
    enum class Phase
    {
        Emerging,
        Playing,
        Descending,
        Done
    };

    void HoldStill();
    void Release();
    void ApplyPose(float offsetY);
    void ClearPose();
    void BeginDescend();
    bool StandsOnStairs() const;

    Engine::TransformComponent *transform = nullptr;
    Engine::SpriteRendererComponent *renderer = nullptr;
    Engine::ColliderComponent *collider = nullptr;
    Engine::SpriteClipAnimatorComponent *animator = nullptr;
    Engine::MovementComponent *movement = nullptr;
    Engine::AttackComponent *attack = nullptr;
    Engine::StatsComponent *stats = nullptr;
    LocomotionAnimator *locomotion = nullptr;

    Phase phase = Phase::Playing;
    Engine::Vector2Df stairs{0.f, 0.f};
    float tileSize = 128.f;
    float spriteHeight = 160.f;
    float originFractionY = 0.75f;
    float duration = 2.2f;
    float elapsed = 0.f;
    float clipY = 0.f;
    float buriedOffset = 0.f;
    float runSpeed = 0.f;
    bool hasStairs = false;
    bool levelChangePending = false;
};
} // namespace Roguelike
