#include "pch.h"
#include "Engine.h"
#include "GameWorld.h"
#include "RenderSystem.h"

#include <iostream>

namespace Engine
{
	Engine* Engine::Instance()
	{
		static Engine instance;
		return &instance;
	}

	Engine::Engine()
	{
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);
	}

	void Engine::Run()
	{
		sf::RenderWindow& window = RenderSystem::Instance()->GetMainWindow();
		window.setVerticalSyncEnabled(true);
		window.setFramerateLimit(60);

		sf::Clock gameClock;

		while (window.isOpen())
		{
			sf::Time dt = gameClock.restart();
			float deltaTime = dt.asSeconds();

			while (const std::optional<sf::Event> event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
				{
					window.close();
				}
			}

			if (!window.isOpen())
			{
				break;
			}

			window.clear();

			GameWorld::Instance()->FixedUpdate(deltaTime);
			GameWorld::Instance()->Render();
			GameWorld::Instance()->LateUpdate();

			window.display();
		}
	}
}