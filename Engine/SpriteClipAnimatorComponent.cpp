#include "pch.h"
#include "SpriteClipAnimatorComponent.h"

#include "ResourceSystem.h"
#include "SpriteRendererComponent.h"

#include <cassert>
#include <cmath>
#include <stdexcept>

namespace Engine
{
	SpriteClipAnimatorComponent::SpriteClipAnimatorComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		renderer = gameObject->GetComponent<SpriteRendererComponent>();
		if (renderer == nullptr)
		{
			throw std::logic_error("SpriteRenderer required for sprite clips");
		}
	}

	void SpriteClipAnimatorComponent::AddClip(const std::string& name, const std::string& textureMapName, int columns, int rows, float frameDuration, bool loop, int actionFrame)
	{
		if (columns <= 0 || rows <= 0 || frameDuration <= 0.f)
		{
			throw std::invalid_argument("Sprite clip '" + name + "' has invalid layout");
		}

		const int frameCount = ResourceSystem::Instance()->GetTextureMapElementsCount(textureMapName);
		assert(frameCount >= columns * rows && "Sprite clip is missing frames");
		if (frameCount < columns * rows)
		{
			throw std::runtime_error("Sprite clip '" + name + "' is missing frames");
		}

		Clip clip;
		clip.columns = columns;
		clip.rows = rows;
		clip.frameDuration = frameDuration;
		clip.loop = loop;
		clip.actionFrame = actionFrame;
		clip.frames.reserve(static_cast<size_t>(columns * rows));
		for (int index = 0; index < columns * rows; ++index)
		{
			const sf::Texture* texture = ResourceSystem::Instance()->GetTextureMapElementShared(textureMapName, index);
			assert(texture != nullptr && "Sprite clip frame was not loaded");
			clip.frames.push_back(texture);
		}

		clips[name] = std::move(clip);
	}

	void SpriteClipAnimatorComponent::Play(const std::string& name, bool restart)
	{
		if (!restart && currentName == name && !(finished && current != nullptr && !current->loop))
		{
			return;
		}

		const auto found = clips.find(name);
		if (found == clips.end())
		{
			throw std::runtime_error("Sprite clip '" + name + "' was not added");
		}

		currentName = name;
		current = &found->second;
		frame = 0;
		time = 0.f;
		finished = false;
		actionPending = false;
		ApplyFrame();
	}

	void SpriteClipAnimatorComponent::SetFacing(const Vector2Df& axis)
	{
		const float absoluteX = std::fabs(axis.x);
		const float absoluteY = std::fabs(axis.y);
		if (absoluteX < 0.01f && absoluteY < 0.01f)
		{
			return;
		}

		if (absoluteX >= absoluteY)
		{
			facingRow = axis.x < 0.f ? 2 : 3;
		}
		else
		{
			facingRow = axis.y > 0.f ? 1 : 0;
		}

		ApplyFrame();
	}

	Vector2Df SpriteClipAnimatorComponent::GetFacing() const
	{
		switch (facingRow)
		{
		case 1:
			return { 0.f, 1.f };
		case 2:
			return { -1.f, 0.f };
		case 3:
			return { 1.f, 0.f };
		default:
			return { 0.f, -1.f };
		}
	}

	bool SpriteClipAnimatorComponent::IsPlaying(const std::string& name) const
	{
		return currentName == name;
	}

	bool SpriteClipAnimatorComponent::IsOneShotPlaying() const
	{
		return current != nullptr && !current->loop && !finished;
	}

	bool SpriteClipAnimatorComponent::IsFinished() const
	{
		return finished;
	}

	bool SpriteClipAnimatorComponent::ConsumeAction()
	{
		if (!actionPending)
		{
			return false;
		}

		actionPending = false;
		return true;
	}

	void SpriteClipAnimatorComponent::Update(float deltaTime)
	{
		if (current == nullptr || finished)
		{
			return;
		}

		time += deltaTime;
		while (time >= current->frameDuration && !finished)
		{
			time -= current->frameDuration;
			++frame;
			if (frame >= current->columns)
			{
				if (current->loop)
				{
					frame = 0;
				}
				else
				{
					frame = current->columns - 1;
					finished = true;
				}
			}

			if (!finished && frame == current->actionFrame)
			{
				actionPending = true;
			}
		}

		ApplyFrame();
	}

	void SpriteClipAnimatorComponent::Render()
	{
	}

	void SpriteClipAnimatorComponent::ApplyFrame()
	{
		if (current == nullptr || renderer == nullptr)
		{
			return;
		}

		int row = facingRow;
		if (row < 0)
		{
			row = 0;
		}
		if (row >= current->rows)
		{
			row = current->rows - 1;
		}

		const int index = row * current->columns + frame;
		if (index < 0 || static_cast<size_t>(index) >= current->frames.size() || current->frames[static_cast<size_t>(index)] == nullptr)
		{
			return;
		}

		renderer->SetTexture(*current->frames[static_cast<size_t>(index)]);
	}
}
