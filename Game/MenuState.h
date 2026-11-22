#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <string>
#include <vector>

namespace Roguelike
{
class MenuState : public State
{
  public:
    MenuState(StateStack &stack, State::Context &context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event &event) override;

  private:
    void moveSelection(int step);
    void rebuildItems();
    void onItemSelected();

    std::vector<std::string> items;
    std::vector<sf::Text> itemTexts;
    int selectedIndex = 0;

    sf::Sprite background;
    sf::Text title;
    sf::VertexArray arrow;
};
} // namespace Roguelike
