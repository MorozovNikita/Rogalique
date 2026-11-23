#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "Vector.h"

namespace Roguelike
{
class Player
{
  public:
    Player(const Engine::Vector2Df &position, float tileSize);
    void SetStairs(const Engine::Vector2Df &position, float tileSize);
    Engine::GameObject *GetGameObject();

  private:
    Engine::GameObject *gameObject;
};
} // namespace Roguelike