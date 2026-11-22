#pragma once

#include "Component.h"

#include <string>

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

    void SetSound(const std::string &key);
    void SetDestroyOnFinish(bool destroy);

    void Update(float deltaTime) override;
    void Render() override;

  private:
    Engine::StatsComponent *stats = nullptr;
    Engine::SpriteClipAnimatorComponent *animator = nullptr;
    Engine::MovementComponent *movement = nullptr;
    std::string soundKey = "vampire_death";
    bool destroyOnFinish = true;
    bool dying = false;
};
} // namespace Roguelike
