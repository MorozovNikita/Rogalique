#include "pch.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"

namespace Engine
{
SpriteRendererComponent::SpriteRendererComponent(GameObject *gameObject)
    : Component(gameObject), placeholder(sf::Vector2u{1, 1})
{
    sprite = new sf::Sprite(placeholder);
    scale = {1, -1};
    sprite->setScale({1, -1});
    transform = gameObject->GetComponent<TransformComponent>();
}
SpriteRendererComponent::~SpriteRendererComponent()
{
    if (sprite != nullptr)
    {
        delete sprite;
    }
}

void SpriteRendererComponent::ApplyTransform()
{
    if (sprite == nullptr || transform == nullptr)
    {
        return;
    }

    const Vector2Df worldPosition = transform->GetWorldPosition();
    sprite->setPosition(Convert<sf::Vector2f, Vector2Df>(worldPosition + drawOffset));
    sprite->setRotation(sf::degrees(transform->GetWorldRotation()));

    auto transformScale = Convert<sf::Vector2f, Vector2Df>(transform->GetWorldScale());
    sprite->setScale({scale.x * transformScale.x, scale.y * transformScale.y});
}

void SpriteRendererComponent::Update(float deltaTime)
{
    ApplyTransform();
}
void SpriteRendererComponent::Render()
{
    if (sprite == nullptr || visibleTopFraction <= 0.f)
    {
        return;
    }

    ApplyTransform();
    RenderSystem::Instance()->Render(*sprite);
}

const sf::Sprite *SpriteRendererComponent::GetSprite() const
{
    return sprite;
}
void SpriteRendererComponent::SetTexture(const sf::Texture &newTexture)
{
    sprite->setTexture(newTexture, true);
    ApplyVisibleTop();
}

void SpriteRendererComponent::SetOriginFraction(float originX, float originY)
{
    originFraction = {originX, originY};
    if (sprite == nullptr)
    {
        return;
    }

    const auto textureSize = sprite->getTexture().getSize();
    sprite->setOrigin({originFraction.x * textureSize.x, originFraction.y * textureSize.y});
}
void SpriteRendererComponent::SetDrawOffset(const Vector2Df &offset)
{
    drawOffset = offset;
}

Vector2Df SpriteRendererComponent::GetDrawOffset() const
{
    return drawOffset;
}

void SpriteRendererComponent::SetVisibleTopFraction(float fraction)
{
    if (fraction < 0.f)
    {
        fraction = 0.f;
    }
    if (fraction > 1.f)
    {
        fraction = 1.f;
    }

    visibleTopFraction = fraction;
    ApplyVisibleTop();
}

void SpriteRendererComponent::ApplyVisibleTop()
{
    if (sprite == nullptr || visibleTopFraction <= 0.f)
    {
        return;
    }

    const auto textureSize = sprite->getTexture().getSize();
    int height = static_cast<int>(textureSize.y);
    if (visibleTopFraction < 1.f)
    {
        height = static_cast<int>(textureSize.y * visibleTopFraction + 0.5f);
    }
    if (height < 1)
    {
        height = 1;
    }
    if (height > static_cast<int>(textureSize.y))
    {
        height = static_cast<int>(textureSize.y);
    }

    sprite->setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(textureSize.x), height}));
    sprite->setOrigin({originFraction.x * textureSize.x, originFraction.y * textureSize.y});
}

void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight)
{
    const auto originalSize = sprite->getTexture().getSize();
    scale = {(float)newWidth / (float)originalSize.x, -(float)newHeight / (float)originalSize.y};
}

void SpriteRendererComponent::FlipX(bool flip)
{
    if (flip != isFlipX)
    {
        scale = {-scale.x, scale.y};
        isFlipX = flip;
    }
}
void SpriteRendererComponent::FlipY(bool flip)
{
    if (flip != isFlipY)
    {
        scale = {scale.x, -scale.y};
        isFlipY = flip;
    }
}
} // namespace Engine