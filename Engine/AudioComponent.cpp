#include "pch.h"
#include "AudioComponent.h"

namespace Engine
{

	AudioComponent::AudioComponent(GameObject* gameObject)
		: Component(gameObject)
		, sound(nullptr)
	{
	}

	AudioComponent::~AudioComponent()
	{
		if (sound == nullptr)
		{
			return;
		}

		sound->stop();
		delete sound;
	}

	void AudioComponent::Update(float deltaTime)
	{
	}

	void AudioComponent::Render()
	{
	}

	void AudioComponent::SetAudio(const sf::SoundBuffer& audio)
	{
		delete sound;
		sound = new sf::Sound(audio);
	}

	void AudioComponent::SetLoop(bool loop)
	{
		if (sound == nullptr)
		{
			return;
		}

		sound->setLooping(loop);
	}

	void AudioComponent::SetVolume(float volume)
	{
		if (sound == nullptr)
		{
			return;
		}

		sound->setVolume(volume);
	}

	void AudioComponent::Play()
	{
		if (sound == nullptr || sound->getStatus() == sf::SoundSource::Status::Playing)
		{
			return;
		}

		sound->play();
	}

	void AudioComponent::Stop()
	{
		if (sound == nullptr)
		{
			return;
		}

		sound->stop();
	}

	void AudioComponent::Pause()
	{
		if (sound == nullptr)
		{
			return;
		}

		sound->pause();
	}

	void AudioComponent::Resume()
	{
		Play();
	}

}
