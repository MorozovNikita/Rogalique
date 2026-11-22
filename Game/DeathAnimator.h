#pragma once

#include "Component.h"

namespace Engine
{
class MovementComponent;
class SpriteClipAnimatorComponent;
class StatsComponent;
} // namespace Engine

namespace Roguelike
{
class DeathAnimator : public Engine::Component
{
  public:
    explicit DeathAnimator(Engine::GameObject *gameObject);

    void Update(float deltaTime) override;
    void Render() override;

  private:
    Engine::StatsComponent *stats = nullptr;
    Engine::SpriteClipAnimatorComponent *animator = nullptr;
    Engine::MovementComponent *movement = nullptr;
    bool dying = false;
};
} // namespace Roguelike
