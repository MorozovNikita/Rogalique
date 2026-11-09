#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "Matrix2D.h"

#include <SFML/Graphics.hpp>

using namespace Roguelike;

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Roguelike");
	RenderSystem::Instance()->SetMainWindow(&window);

	ResourceSystem::Instance()->LoadTexture("ball", "Resources/textures/ball.png");
	ResourceSystem::Instance()->LoadTexture("monster", "Resources/textures/Monster.png");

	ResourceSystem::Instance()->LoadTextureMap("level_floors", "Resources/textures/Floor.png", { 16, 16 }, 49, false);
	ResourceSystem::Instance()->LoadTextureMap("level_walls", "Resources/textures/Wall.png", { 16, 16 }, 48, false);

	ResourceSystem::Instance()->LoadSoundBuffer("music", "Resources/music/dev level music.mp3");

	auto developerLevel = std::make_shared<DeveloperLevel>();
	developerLevel->Start();

	::Engine::Engine::Instance()->Run();

	developerLevel->Stop();
	ResourceSystem::Instance()->Clear();
	RenderSystem::Instance()->SetMainWindow(nullptr);

	return 0;
}