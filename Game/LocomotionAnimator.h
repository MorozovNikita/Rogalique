#pragma once

#include "Component.h"

namespace Engine
{
class DirectionComponent;
class SpriteClipAnimatorComponent;
class SpriteRendererComponent;
} // namespace Engine

namespace Roguelike
{
class LocomotionAnimator : public Engine::Component
{
  public:
    LocomotionAnimator(Engine::GameObject *gameObject);

    void SetMirrorHorizontal(bool mirror);

    void Update(float deltaTime) override;
    void Render() override;

  private:
    Engine::SpriteClipAnimatorComponent *animator = nullptr;
    Engine::DirectionComponent *direction = nullptr;
    Engine::SpriteRendererComponent *renderer = nullptr;
    bool mirrorHorizontal = false;
};
} // namespace Roguelike
