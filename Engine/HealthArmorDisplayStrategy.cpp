#include "pch.h"
#include "HealthArmorDisplayStrategy.h"

#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "StatsComponent.h"

#include <algorithm>
#include <stdexcept>

namespace Engine
{
namespace
{
const sf::Texture &RequireTexture(const std::string &key)
{
    const sf::Texture *texture = ResourceSystem::Instance()->GetTextureShared(key);
    if (texture == nullptr)
    {
        throw std::runtime_error("Stats bar texture was not loaded: " + key);
    }

    return *texture;
}

float Ratio(float current, float maximum)
{
    if (maximum <= 0.f)
    {
        return 0.f;
    }

    return std::clamp(current / maximum, 0.f, 1.f);
}
} // namespace

HealthArmorDisplayStrategy::HealthArmorDisplayStrategy(const std::string &backgroundKey, const std::string &healthKey,
                                                       const std::string &armorKey)
    : background(RequireTexture(backgroundKey)), health(RequireTexture(healthKey)), armor(RequireTexture(armorKey))
{
}

void HealthArmorDisplayStrategy::Render(const StatsComponent &stats, const Vector2Df &center)
{
    const Vector2Df leftCenter{center.x - width * 0.5f, center.y};
    DrawBar(background, 1.f, leftCenter);
    DrawBar(health, Ratio(stats.GetCurrentHealth(), stats.GetMaxHealth()), leftCenter);
    DrawBar(armor, Ratio(stats.GetArmor(), stats.GetMaxArmor()), leftCenter);
}

void HealthArmorDisplayStrategy::DrawBar(sf::Sprite &sprite, float ratio, const Vector2Df &leftCenter) const
{
    const sf::Vector2u size = sprite.getTexture().getSize();
    if (size.x == 0 || size.y == 0)
    {
        return;
    }

    const int barWidth = static_cast<int>(std::lround(static_cast<float>(size.x) * std::clamp(ratio, 0.f, 1.f)));
    if (barWidth <= 0)
    {
        return;
    }

    sprite.setTextureRect(sf::IntRect({0, 0}, {barWidth, static_cast<int>(size.y)}));
    sprite.setOrigin({0.f, static_cast<float>(size.y) * 0.5f});
    sprite.setScale({width / static_cast<float>(size.x), -height / static_cast<float>(size.y)});
    sprite.setPosition({leftCenter.x, leftCenter.y});
    RenderSystem::Instance()->Render(sprite);
}
} // namespace Engine
