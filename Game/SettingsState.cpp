#include "SettingsState.h"

#include "ResourceHolder.h"
#include "ResourceSystem.h"
#include "Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <stdexcept>

namespace Roguelike
{
namespace
{
constexpr float ArrowSize = 15.f;
const sf::Color SelectedColor{80, 220, 80};
const sf::Color NormalColor{255, 255, 255};
const sf::Color HintColor{180, 180, 180};

sf::Sprite BackgroundSprite(const sf::Texture &texture, const sf::Vector2f &windowSize)
{
    sf::Sprite sprite(texture);
    const sf::Vector2u textureSize = texture.getSize();
    sprite.setScale(
        {windowSize.x / static_cast<float>(textureSize.x), windowSize.y / static_cast<float>(textureSize.y)});
    return sprite;
}
} // namespace

SettingsState::SettingsState(StateStack &stack, State::Context &context)
    : State(stack, context),
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
      title(context.fonts.get(Fonts::Main), "Settings", 40), label(context.fonts.get(Fonts::Main), "", 22),
      hint(context.fonts.get(Fonts::Main), "Enter - toggle    Esc - back", 12),
      arrow(sf::PrimitiveType::Triangles, 3)
{
    const sf::Vector2f windowSize(static_cast<float>(context.window.getSize().x),
                                  static_cast<float>(context.window.getSize().y));

    title.setFillColor(NormalColor);
    const sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.position + titleBounds.size / 2.f);
    title.setPosition({windowSize.x / 2.f, 150.f});

    hint.setFillColor(HintColor);
    const sf::FloatRect hintBounds = hint.getLocalBounds();
    hint.setOrigin(hintBounds.position + hintBounds.size / 2.f);
    hint.setPosition({windowSize.x / 2.f, windowSize.y - 72.f});

    for (std::size_t vertex = 0; vertex < arrow.getVertexCount(); ++vertex)
    {
        arrow[vertex].color = SelectedColor;
    }

    refreshLabel();
}

void SettingsState::refreshLabel()
{
    const float centerX = static_cast<float>(getContext().window.getSize().x) / 2.f;
    label.setString(getContext().settings.music ? "Music: on" : "Music: off");
    label.setFillColor(SelectedColor);
    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.position + bounds.size / 2.f);
    label.setPosition({centerX, 280.f});
}

void SettingsState::draw()
{
    sf::RenderWindow &window = getContext().window;
    window.setView(window.getDefaultView());
    window.draw(background);
    window.draw(title);
    window.draw(label);
    window.draw(hint);

    const sf::FloatRect bounds = label.getGlobalBounds();
    const float arrowX = bounds.position.x - ArrowSize - 12.f;
    const float arrowY = bounds.position.y + bounds.size.y / 2.f - ArrowSize / 2.f;
    arrow[0].position = {arrowX, arrowY};
    arrow[1].position = {arrowX, arrowY + ArrowSize};
    arrow[2].position = {arrowX + ArrowSize, arrowY + ArrowSize / 2.f};
    window.draw(arrow);
}

bool SettingsState::update(sf::Time)
{
    return false;
}

bool SettingsState::handleEvent(const sf::Event &event)
{
    const auto *keyPressed = event.getIf<sf::Event::KeyPressed>();
    if (keyPressed == nullptr)
    {
        return false;
    }

    const Game::InputBindings &input = getContext().settings.input;
    if (keyPressed->code == input.confirm)
    {
        getContext().settings.music = !getContext().settings.music;
        Engine::ResourceSystem::Instance()->SetMusicVolume(getContext().settings.music ? 100.f : 0.f);
        refreshLabel();
    }
    else if (keyPressed->code == input.back)
    {
        requestStackPop();
    }

    return false;
}
} // namespace Roguelike
