#pragma once

namespace sf
{
class Font;
class SoundBuffer;
} // namespace sf

namespace Fonts
{
enum ID
{
    Main,
};
} // namespace Fonts

template <typename Resource, typename Identifier> class ResourceHolder;

namespace SoundEffects
{
enum ID
{
};
} // namespace SoundEffects

typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffects::ID> SoundBufferHolder;
