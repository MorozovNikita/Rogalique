#include "MainMenu.h"

#include <SFML/Graphics.hpp>

#include <optional>

namespace Roguelike
{
	bool MainMenu::Show(sf::RenderWindow& window)
	{
		sf::Font font;
		const bool hasFont = font.openFromFile("Resources/Fonts/PressStart2P-Regular.ttf");

		sf::RectangleShape startButton({ 360.f, 80.f });
		startButton.setFillColor(sf::Color(48, 72, 120));
		startButton.setOutlineColor(sf::Color(180, 200, 230));
		startButton.setOutlineThickness(2.f);

		const sf::Vector2f windowSize(
			static_cast<float>(window.getSize().x),
			static_cast<float>(window.getSize().y));
		startButton.setPosition({
			(windowSize.x - startButton.getSize().x) / 2.f,
			(windowSize.y - startButton.getSize().y) / 2.f });

		std::optional<sf::Text> startLabel;
		if (hasFont)
		{
			startLabel.emplace(font, "Start", 28u);
			startLabel->setFillColor(sf::Color::White);
			const sf::FloatRect textBounds = startLabel->getLocalBounds();
			startLabel->setOrigin(textBounds.position + textBounds.size / 2.f);
			startLabel->setPosition(startButton.getPosition() + startButton.getSize() / 2.f);
		}

		while (window.isOpen())
		{
			while (const std::optional<sf::Event> event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
				{
					window.close();
					return false;
				}

				if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>())
				{
					if (pressed->button == sf::Mouse::Button::Left)
					{
						const sf::Vector2f cursor(
							static_cast<float>(pressed->position.x),
							static_cast<float>(pressed->position.y));
						if (startButton.getGlobalBounds().contains(cursor))
							return true;
					}
				}
			}

			window.clear(sf::Color(16, 18, 28));
			window.draw(startButton);
			if (startLabel)
				window.draw(*startLabel);
			window.display();
		}

		return false;
	}
}
