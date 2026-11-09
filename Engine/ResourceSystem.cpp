#include "pch.h"
#include "ResourceSystem.h"

namespace Engine
{
	ResourceSystem::~ResourceSystem()
	{
		Clear();
	}

	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}

	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
	{
		if (textures.find(name) != textures.end())
		{
			return;
		}

		sf::Texture* newTexture = new sf::Texture();
		if (newTexture->loadFromFile(sourcePath))
		{
			newTexture->setSmooth(isSmooth);
			textures.emplace(name, newTexture);
		}
		else
		{
			delete newTexture;
		}
	}

	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		auto found = textures.find(name);
		if (found == textures.end())
		{
			return nullptr;
		}

		return found->second;
	}

	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		auto found = textures.find(name);
		if (found == textures.end())
		{
			return nullptr;
		}

		return new sf::Texture(*found->second);
	}

	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto texturePair = textures.find(name);
		if (texturePair == textures.end())
		{
			return;
		}

		sf::Texture* deletingTexure = texturePair->second;
		textures.erase(texturePair);
		delete deletingTexure;
	}

	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
	{
		if (textureMaps.find(name) != textureMaps.end())
		{
			return;
		}

		sf::Texture textureMap;
		if (textureMap.loadFromFile(sourcePath))
		{
			std::vector<sf::Texture*> textureMapElements;

			auto textureSize = textureMap.getSize();
			int loadedElements = 0;

			for (int y = 0; y + static_cast<int>(elementPixelSize.y) <= static_cast<int>(textureSize.y); y += static_cast<int>(elementPixelSize.y))
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				for (int x = 0; x + static_cast<int>(elementPixelSize.x) <= static_cast<int>(textureSize.x); x += static_cast<int>(elementPixelSize.x))
				{
					if (loadedElements == totalElements)
					{
						break;
					}

					sf::Texture* newTextureMapElement = new sf::Texture();
					if (newTextureMapElement->loadFromFile(sourcePath, false, sf::IntRect({ x, y }, { static_cast<int>(elementPixelSize.x), static_cast<int>(elementPixelSize.y) })))
					{
						newTextureMapElement->setSmooth(isSmooth);
						textureMapElements.push_back(newTextureMapElement);
					}
					else
					{
						delete newTextureMapElement;
					}
					loadedElements++;
				}
			}

			textureMaps.emplace(name, std::move(textureMapElements));
		}
	}

	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end())
		{
			return nullptr;
		}

		const auto& mapTextures = textureMap->second;
		if (elementIndex < 0 || static_cast<size_t>(elementIndex) >= mapTextures.size())
		{
			return nullptr;
		}

		return mapTextures[elementIndex];
	}

	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		const sf::Texture* texture = GetTextureMapElementShared(name, elementIndex);
		if (texture == nullptr)
		{
			return nullptr;
		}

		return new sf::Texture(*texture);
	}

	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end())
		{
			return 0;
		}

		return static_cast<int>(textureMap->second.size());
	}

	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end())
		{
			return;
		}

		auto& deletingTextures = textureMap->second;

		for (size_t i = 0; i < deletingTextures.size(); i++)
		{
			delete deletingTextures[i];
		}

		textureMaps.erase(textureMap);
	}

	void ResourceSystem::LoadSoundBuffer(const std::string& name, std::string sourcePath)
	{
		if (soundBuffers.find(name) != soundBuffers.end())
		{
			return;
		}

		sf::SoundBuffer* newBuffer = new sf::SoundBuffer();
		if (newBuffer->loadFromFile(sourcePath))
		{
			soundBuffers.emplace(name, newBuffer);
		}
		else
		{
			delete newBuffer;
		}
	}

	const sf::SoundBuffer* ResourceSystem::GetSoundBuffer(const std::string& name) const
	{
		auto found = soundBuffers.find(name);
		if (found == soundBuffers.end())
		{
			return nullptr;
		}

		return found->second;
	}

	void ResourceSystem::DeleteSharedSoundBuffer(const std::string& name)
	{
		auto found = soundBuffers.find(name);
		if (found == soundBuffers.end())
		{
			return;
		}

		delete found->second;
		soundBuffers.erase(found);
	}

	void ResourceSystem::LoadMusic(const std::string& name, std::string sourcePath)
	{
		musicPaths[name] = std::move(sourcePath);
	}

	void ResourceSystem::PlayMusic(const std::string& nameOrPath)
	{
		std::string path = nameOrPath;
		auto found = musicPaths.find(nameOrPath);
		if (found != musicPaths.end())
		{
			path = found->second;
		}

		StopMusic();
		if (!currentMusic.openFromFile(path))
		{
			return;
		}

		currentMusic.setLooping(true);
		currentMusic.setVolume(musicVolume);
		currentMusic.play();
	}

	void ResourceSystem::StopMusic()
	{
		currentMusic.stop();
	}

	void ResourceSystem::SetMusicVolume(float volume)
	{
		musicVolume = volume;
		currentMusic.setVolume(musicVolume);
	}

	void ResourceSystem::SetEffectsVolume(float volume)
	{
		effectsVolume = volume;
	}

	void ResourceSystem::Clear()
	{
		StopMusic();
		DeleteAllTextures();
		DeleteAllTextureMaps();
		DeleteAllSoundBuffers();
	}

	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& texturePair : textures)
		{
			keysToDelete.push_back(texturePair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
	}

	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& textureMapPair : textureMaps)
		{
			keysToDelete.push_back(textureMapPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
	}

	void ResourceSystem::DeleteAllSoundBuffers()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& bufferPair : soundBuffers)
		{
			keysToDelete.push_back(bufferPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedSoundBuffer(key);
		}
	}
}