#include "Music.h"

#include <cassert>

namespace Roguelike
{

	Music::Music(const std::string& soundName)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Music: " + soundName);
		auto music = gameObject->AddComponent<Engine::AudioComponent>();

		const auto* buffer = Engine::ResourceSystem::Instance()->GetSoundBuffer(soundName);
		assert(buffer != nullptr && "Music buffer was not loaded");
		if (buffer == nullptr)
		{
			return;
		}

		music->SetAudio(*buffer);
		music->SetLoop(true);
		music->SetVolume(Engine::ResourceSystem::Instance()->GetMusicVolume());
		music->Play();
	}

}
