#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "Logger.h"
#include "Matrix2D.h"

#include <SFML/Graphics.hpp>

#include <exception>
#include <string>

using namespace Roguelike;

int main()
{
	InitializeLogging("rogalique.log");

	try
	{
		sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Roguelike");
		RenderSystem::Instance()->SetMainWindow(&window);

		ResourceSystem::Instance()->LoadTexture("ball", "Resources/textures/ball.png");

		ResourceSystem::Instance()->LoadTextureMap("level_floors", "Resources/textures/Floor.png", { 16, 16 }, 49, false);
		ResourceSystem::Instance()->LoadTextureMap("level_walls", "Resources/textures/Wall.png", { 16, 16 }, 48, false);

		const std::string vampiresFolder = "Resources/textures/Vampires/";
		for (int index = 1; index <= 3; ++index)
		{
			const std::string folderName = "Vampires" + std::to_string(index);
			const std::string key = "vampire" + std::to_string(index);

			ResourceSystem::Instance()->LoadTextureMap(key + "_idle", vampiresFolder + folderName + '/' + folderName + "_Idle_without_shadow.png", {64, 64}, 16, false);
			ResourceSystem::Instance()->LoadTextureMap(key + "_run", vampiresFolder + folderName + '/' + folderName + "_Run_without_shadow.png", { 64, 64 }, 32, false);
			ResourceSystem::Instance()->LoadTextureMap(key + "_attack", vampiresFolder + folderName + '/' + folderName + "_Attack_without_shadow.png", { 64, 64 }, 48, false);
		}

		ResourceSystem::Instance()->LoadSoundBuffer("music", "Resources/music/dev level music.mp3");

		auto developerLevel = std::make_shared<DeveloperLevel>();
		developerLevel->Start();

		::Engine::Engine::Instance()->Run();

		developerLevel->Stop();
		ResourceSystem::Instance()->Clear();
		RenderSystem::Instance()->SetMainWindow(nullptr);
	}
	catch (const std::exception& exception)
	{
		LOG_ERROR(std::string("Fatal: ") + exception.what());
		return 1;
	}

	return 0;
}