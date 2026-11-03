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
		sf::Clock gameClock;

		while (RenderSystem::Instance()->GetMainWindow().isOpen())
		{
			sf::Time dt = gameClock.restart();
			float deltaTime = dt.asSeconds();

			while (const std::optional<sf::Event> event = RenderSystem::Instance()->GetMainWindow().pollEvent())
			{
				if (event->is<sf::Event::Closed>())
				{
					RenderSystem::Instance()->GetMainWindow().close();
				}
			}

			if (!RenderSystem::Instance()->GetMainWindow().isOpen())
			{
				break;
			}

			RenderSystem::Instance()->GetMainWindow().clear();

			GameWorld::Instance()->Update(deltaTime);
			GameWorld::Instance()->FixedUpdate(deltaTime);
			GameWorld::Instance()->Render();
			GameWorld::Instance()->LateUpdate();

			RenderSystem::Instance()->GetMainWindow().display();
		}
	}
}