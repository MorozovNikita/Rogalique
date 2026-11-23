#include "pch.h"
#include "SpriteColliderComponent.h"

#include <cmath>
#include <stdexcept>

namespace Engine
{
SpriteColliderComponent::SpriteColliderComponent(GameObject *gameObject) : ColliderComponent(gameObject)
{
    renderer = gameObject->GetComponent<SpriteRendererComponent>();
    if (renderer == nullptr)
    {
        throw std::logic_error("SpriteRenderer required to SpriteCollider");
    }

    sprite = renderer->GetSprite();
    PhysicsSystem::Instance()->Subscribe(this);
}
SpriteColliderComponent::~SpriteColliderComponent()
{
    if (PhysicsSystem::IsAlive())
    {
        PhysicsSystem::Instance()->Unsubscribe(this);
    }
}

void SpriteColliderComponent::Update(float deltaTime)
{
    if (renderer == nullptr || sprite == nullptr)
    {
        return;
    }

    renderer->ApplyTransform();
    const sf::Vector2u textureSize = sprite->getTexture().getSize();
    const sf::Vector2f spriteScale = sprite->getScale();
    const float width = std::fabs(static_cast<float>(textureSize.x) * spriteScale.x) * boundsScaleX;
    const float height = std::fabs(static_cast<float>(textureSize.y) * spriteScale.y) * boundsScaleY;
    const sf::Vector2f center = sprite->getPosition();
    bounds.position = {center.x - width * 0.5f, center.y - height * 0.5f};
    bounds.size = {width, height};
}

void SpriteColliderComponent::SetBoundsScale(float scaleX, float scaleY)
{
    if (scaleX <= 0.f || scaleY <= 0.f)
    {
        throw std::invalid_argument("Sprite collider scale must be positive");
    }

    boundsScaleX = scaleX;
    boundsScaleY = scaleY;
}
void SpriteColliderComponent::Render()
{
    sf::RectangleShape rectangle(bounds.size);
    rectangle.setPosition(bounds.position);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(4);

    RenderSystem::Instance()->Render(rectangle);
}
} // namespace Engine