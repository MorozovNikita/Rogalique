#include "GameState.h"

#include "LevelGateAnimator.h"

#include "CameraComponent.h"
#include "GameWorld.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "SpriteClipAnimatorComponent.h"
#include "StatsComponent.h"

#include <exception>
#include <string>

namespace Roguelike
{
GameState::GameState(StateStack &stack, State::Context &context) : State(stack, context)
{
    Engine::RenderSystem::Instance()->SetMainWindow(&context.window);
    loadResources();
    level = std::make_unique<DeveloperLevel>();
    level->Start();
    LOG_INFO("Engine started");
}

GameState::~GameState()
{
    if (level != nullptr)
    {
        level->Stop();
    }
    Engine::RenderSystem::Instance()->SetMainWindow(nullptr);
}

void GameState::loadResources()
{
    Engine::ResourceSystem *resources = Engine::ResourceSystem::Instance();

    const std::string knightFolder = "Resources/textures/knight/Knight_1/";
    resources->LoadTextureMap("knight_idle", knightFolder + "Idle.png", {128, 128}, 4, false);
    resources->LoadTextureMap("knight_run", knightFolder + "Run.png", {128, 128}, 7, false);
    resources->LoadTextureMap("knight_attack", knightFolder + "Attack 1.png", {128, 128}, 5, false);
    resources->LoadTextureMap("knight_death", knightFolder + "Dead.png", {128, 128}, 6, false);

    resources->LoadTextureMap("level_floors", "Resources/textures/Floor.png", {16, 16}, 49, false);
    resources->LoadTextureMap("level_walls", "Resources/textures/Wall.png", {16, 16}, 48, false);

    const std::string vampiresFolder = "Resources/textures/Vampires/";
    for (int index = 1; index <= 3; ++index)
    {
        const std::string folderName = "Vampires" + std::to_string(index);
        const std::string key = "vampire" + std::to_string(index);
        resources->LoadTextureMap(key + "_idle",
                                  vampiresFolder + folderName + '/' + folderName + "_Idle_without_shadow.png", {64, 64},
                                  16, false);
        resources->LoadTextureMap(key + "_run",
                                  vampiresFolder + folderName + '/' + folderName + "_Run_without_shadow.png", {64, 64},
                                  32, false);
        resources->LoadTextureMap(key + "_attack",
                                  vampiresFolder + folderName + '/' + folderName + "_Attack_without_shadow.png",
                                  {64, 64}, 48, false);
        resources->LoadTextureMap(key + "_death",
                                  vampiresFolder + folderName + '/' + folderName + "_Death_without_shadow.png",
                                  {64, 64}, 44, false);
    }

    const std::string statsFolder = "Resources/textures/Stats/";
    resources->LoadTexture("bar_bg", statsFolder + "bar_BG.png", false);
    resources->LoadTexture("bar_hp", statsFolder + "bar_hp.png", false);
    resources->LoadTexture("bar_shield", statsFolder + "bar_shield.png", false);

    resources->LoadSoundBuffer("music", "Resources/music/dev level music.mp3");
    resources->LoadSoundBuffer("death", "Resources/music/DeadSound.mp3");
    resources->LoadSoundBuffer("vampire_death", "Resources/music/DeadVampire.mp3");
}

void GameState::draw()
{
    sf::RenderWindow &window = getContext().window;
    window.clear();

    for (Engine::GameObject *object : Engine::GameWorld::Instance()->GetGameObjects())
    {
        if (Engine::CameraComponent *camera = object->GetComponent<Engine::CameraComponent>())
        {
            camera->Apply();
        }
    }

    Engine::GameWorld::Instance()->Render();
}

bool GameState::update(sf::Time dt)
{
    try
    {
        Engine::GameWorld::Instance()->FixedUpdate(dt.asSeconds());
        Engine::GameWorld::Instance()->LateUpdate();

        bool nextLevel = false;
        for (Engine::GameObject *object : Engine::GameWorld::Instance()->GetGameObjects())
        {
            if (object->GetName() != "Player")
            {
                continue;
            }

            const Engine::StatsComponent *stats = object->GetComponent<Engine::StatsComponent>();
            const Engine::SpriteClipAnimatorComponent *animator =
                object->GetComponent<Engine::SpriteClipAnimatorComponent>();
            if (stats != nullptr && animator != nullptr && stats->GetCurrentHealth() <= 0.f &&
                animator->IsPlaying("death") && animator->IsFinished())
            {
                requestStackPop();
                break;
            }

            LevelGateAnimator *gate = object->GetComponent<LevelGateAnimator>();
            if (gate != nullptr && gate->ConsumeLevelChange())
            {
                nextLevel = true;
                break;
            }
        }

        if (nextLevel && level != nullptr)
        {
            level->Restart();
        }
    }
    catch (const std::exception &exception)
    {
        LOG_ERROR(std::string("Engine stopped: ") + exception.what());
        requestStateClear();
    }

    return false;
}

bool GameState::handleEvent(const sf::Event &)
{
    return false;
}
} // namespace Roguelike
