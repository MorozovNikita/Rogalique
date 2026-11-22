#pragma once

#include "TransformComponent.h"
#include "Vector.h"

#include <SFML/Graphics.hpp>
#include <memory>

namespace Engine
{
class SpriteRendererComponent : public Component
{
  public:
    SpriteRendererComponent(GameObject *gameObject);
    ~SpriteRendererComponent();

    void Update(float deltaTime) override;
    void Render() override;

    void ApplyTransform();
    const sf::Sprite *GetSprite() const;
    void SetTexture(const sf::Texture &newTexture);
    void SetPixelSize(int newWidth, int newHeight);
    void SetOriginFraction(float originX, float originY);
    void SetDrawOffset(const Vector2Df &offset);
    Vector2Df GetDrawOffset() const;
    void SetVisibleTopFraction(float fraction);

    void FlipX(bool flip);
    void FlipY(bool flip);

  private:
    void ApplyVisibleTop();

    sf::Texture placeholder;
    Vector2Df scale;
    Vector2Df originFraction{0.5f, 0.5f};
    Vector2Df drawOffset{0.f, 0.f};
    float visibleTopFraction = 1.f;
    sf::Sprite *sprite = nullptr;
    TransformComponent *transform;

    bool isFlipX = false;
    bool isFlipY = false;
};
} // namespace Engine