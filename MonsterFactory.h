#pragma once

#include "Monster.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace Engine
{
	struct ChaseSettings;
}

namespace Roguelike
{
	enum class MonsterId
	{
		Vampire
	};

	class MonsterFactory
	{
	public:
		virtual ~MonsterFactory() = default;

		std::unique_ptr<Monster> Create(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings);

	protected:
		explicit MonsterFactory(std::vector<MonsterAppearance> appearances);

		virtual std::unique_ptr<Monster> CreateMonster(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings, const MonsterAppearance& appearance) = 0;

		const MonsterAppearance& RandomAppearance();

	private:
		std::vector<MonsterAppearance> appearances;
	};

	class VampireFactory : public MonsterFactory
	{
	public:
		VampireFactory();

	protected:
		std::unique_ptr<Monster> CreateMonster(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings, const MonsterAppearance& appearance) override;
	};

	class MonsterCatalog
	{
	public:
		MonsterCatalog();

		void Register(MonsterId id, std::unique_ptr<MonsterFactory> factory);
		std::unique_ptr<Monster> Create(MonsterId id, const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings);

	private:
		std::unordered_map<MonsterId, std::unique_ptr<MonsterFactory>> factories;
	};
}
