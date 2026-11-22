#pragma once

#include "DeveloperLevel.h"
#include "State.h"

#include <memory>

namespace Roguelike
{
class GameState : public State
{
  public:
    GameState(StateStack &stack, State::Context &context);
    ~GameState() override;

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event &event) override;

  private:
    void loadResources();

    std::unique_ptr<DeveloperLevel> level;
};
} // namespace Roguelike
