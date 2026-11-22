#include "MenuState.h"

#include "ResourceHolder.h"
#include "ResourceSystem.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <stdexcept>

namespace Roguelike
{
namespace
{
constexpr float ArrowSize = 15.f;
constexpr float FirstItemY = 280.f;
constexpr float ItemGap = 64.f;

const sf::Color SelectedColor{80, 220, 80};
const sf::Color NormalColor{255, 255, 255};

sf::Sprite BackgroundSprite(const sf::Texture &texture, const sf::Vector2f &windowSize)
{
    sf::Sprite sprite(texture);
    const sf::Vector2u textureSize = texture.getSize();
    sprite.setScale(
        {windowSize.x / static_cast<float>(textureSize.x), windowSize.y / static_cast<float>(textureSize.y)});
    return sprite;
}
} // namespace

MenuState::MenuState(StateStack &stack, State::Context &context)
    : State(stack, context), items{"Play", "Settings", "Exit"},
      background(BackgroundSprite(*[]() {
          const sf::Texture *texture = Engine::ResourceSystem::Instance()->GetTextureShared("menu_background");
          if (texture == nullptr)
          {
              throw std::runtime_error("Menu background was not loaded");
          }
          return texture;
      }(),
                                 {static_cast<float>(context.window.getSize().x),
                                  static_cast<float>(context.window.getSize().y)})),
      title(context.fonts.get(Fonts::Main), "Rogalique", 40), arrow(sf::PrimitiveType::Triangles, 3)
{
    const sf::Vector2f windowSize(static_cast<float>(context.window.getSize().x),
                                  static_cast<float>(context.window.getSize().y));
    title.setFillColor(NormalColor);
    const sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.position + titleBounds.size / 2.f);
    title.setPosition({windowSize.x / 2.f, 150.f});

    for (std::size_t vertex = 0; vertex < arrow.getVertexCount(); ++vertex)
    {
        arrow[vertex].color = SelectedColor;
    }

    rebuildItems();
    Engine::ResourceSystem::Instance()->PlayMusic("menu");
}

void MenuState::moveSelection(int step)
{
    const int count = static_cast<int>(items.size());
    selectedIndex = (selectedIndex + step + count) % count;
    rebuildItems();
}

void MenuState::rebuildItems()
{
    const sf::Font &font = title.getFont();
    const float centerX = static_cast<float>(getContext().window.getSize().x) / 2.f;
    itemTexts.clear();
    for (std::size_t index = 0; index < items.size(); ++index)
    {
        sf::Text text(font, items[index], 22);
        text.setFillColor(static_cast<int>(index) == selectedIndex ? SelectedColor : NormalColor);
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.position + bounds.size / 2.f);
        text.setPosition({centerX, FirstItemY + static_cast<float>(index) * ItemGap});
        itemTexts.push_back(std::move(text));
    }
}

void MenuState::onItemSelected()
{
    switch (selectedIndex)
    {
    case 0:
        Engine::ResourceSystem::Instance()->StopMusic();
        requestStackPush(States::Game);
        break;
    case 1:
        requestStackPush(States::Settings);
        break;
    case 2:
        Engine::ResourceSystem::Instance()->StopMusic();
        requestStackPop();
        break;
    default:
        break;
    }
}

void MenuState::draw()
{
    sf::RenderWindow &window = getContext().window;
    window.setView(window.getDefaultView());
    window.draw(background);
    window.draw(title);
    for (const sf::Text &text : itemTexts)
    {
        window.draw(text);
    }

    const sf::Text &selected = itemTexts[static_cast<std::size_t>(selectedIndex)];
    const sf::FloatRect bounds = selected.getGlobalBounds();
    const float arrowX = bounds.position.x - ArrowSize - 12.f;
    const float arrowY = bounds.position.y + bounds.size.y / 2.f - ArrowSize / 2.f;
    arrow[0].position = {arrowX, arrowY};
    arrow[1].position = {arrowX, arrowY + ArrowSize};
    arrow[2].position = {arrowX + ArrowSize, arrowY + ArrowSize / 2.f};
    window.draw(arrow);
}

bool MenuState::update(sf::Time)
{
    if (!Engine::ResourceSystem::Instance()->IsMusicPlaying())
    {
        Engine::ResourceSystem::Instance()->PlayMusic("menu");
    }

    return true;
}

bool MenuState::handleEvent(const sf::Event &event)
{
    const auto *keyPressed = event.getIf<sf::Event::KeyPressed>();
    if (keyPressed == nullptr)
    {
        return true;
    }

    const Game::InputBindings &input = getContext().settings.input;
    if (keyPressed->code == input.menuUp)
    {
        moveSelection(-1);
    }
    else if (keyPressed->code == input.menuDown)
    {
        moveSelection(1);
    }
    else if (keyPressed->code == input.confirm)
    {
        onItemSelected();
    }
    else if (keyPressed->code == input.back)
    {
        Engine::ResourceSystem::Instance()->StopMusic();
        requestStackPop();
    }

    return false;
}
} // namespace Roguelike
