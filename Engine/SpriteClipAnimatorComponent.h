#pragma once

#include "Component.h"
#include "Vector.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace sf
{
	class Texture;
}

namespace Engine
{
	class SpriteRendererComponent;

	class SpriteClipAnimatorComponent : public Component
	{
	public:
		SpriteClipAnimatorComponent(GameObject* gameObject);

		void AddClip(const std::string& name, const std::string& textureMapName, int columns, int rows, float frameDuration, bool loop, int actionFrame);

		void Play(const std::string& name, bool restart = false);
		void SetFacing(const Vector2Df& axis);
		Vector2Df GetFacing() const;

		bool IsPlaying(const std::string& name) const;
		bool IsOneShotPlaying() const;
		bool IsFinished() const;
		bool ConsumeAction();

		void Update(float deltaTime) override;
		void Render() override;

	private:
		struct Clip
		{
			std::vector<const sf::Texture*> frames;
			int columns = 1;
			int rows = 1;
			float frameDuration = 0.1f;
			bool loop = true;
			int actionFrame = -1;
		};

		void ApplyFrame();

		SpriteRendererComponent* renderer = nullptr;
		std::unordered_map<std::string, Clip> clips;
		std::string currentName;
		const Clip* current = nullptr;

		int frame = 0;
		int facingRow = 0;
		float time = 0.f;
		bool finished = true;
		bool actionPending = false;
	};
}
