#pragma once

namespace sf
{
	class Font;
	class SoundBuffer;
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

namespace SoundEffects
{
	enum ID
	{
	};
}

typedef ResourceHolder<sf::Font, Fonts::ID>                 FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffects::ID>   SoundBufferHolder;
