#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace Roguelike
{
class SettingsState : public State
{
  public:
    SettingsState(StateStack &stack, State::Context &context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event &event) override;

  private:
    void refreshLabel();

    sf::Sprite background;
    sf::Text title;
    sf::Text label;
    sf::Text hint;
    sf::VertexArray arrow;
};
} // namespace Roguelike
