#pragma once

#include "StatsDisplayComponent.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Engine
{
class HealthArmorDisplayStrategy : public StatsDisplayStrategy
{
  public:
    HealthArmorDisplayStrategy(const std::string &backgroundKey, const std::string &healthKey,
                               const std::string &armorKey);

    void Render(const StatsComponent &stats, const Vector2Df &center) override;

  private:
    void DrawBar(sf::Sprite &sprite, float ratio, const Vector2Df &leftCenter) const;

    sf::Sprite background;
    sf::Sprite health;
    sf::Sprite armor;
    float width = 112.f;
    float height = 28.f;
};
} // namespace Engine
