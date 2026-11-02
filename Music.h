#pragma once

#include "GameObject.h"

#include <AudioComponent.h>
#include <GameWorld.h>
#include <ResourceSystem.h>

#include <string>

namespace Roguelike
{
	class Music
	{
	public:
		Music(const std::string& soundName);
	private:
		Engine::GameObject* gameObject;
	};
}
