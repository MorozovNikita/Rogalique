#include "MonsterFactory.h"

#include "Logger.h"

#include <random>
#include <stdexcept>

namespace Roguelike
{
	namespace
	{
		MonsterAppearance VampireAppearance(const char* id)
		{
			const std::string key(id);
			return { key, key + "_idle", key + "_run", key + "_attack" };
		}
	}

	MonsterFactory::MonsterFactory(std::vector<MonsterAppearance> appearances)
		: appearances(std::move(appearances))
	{
		if (this->appearances.empty())
		{
			throw std::logic_error("Monster factory has no appearances");
		}
	}

	std::unique_ptr<Monster> MonsterFactory::Create(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings)
	{
		const MonsterAppearance& appearance = RandomAppearance();
		LOG_INFO("Created monster with appearance " + appearance.id);
		return CreateMonster(position, chaseSettings, appearance);
	}

	const MonsterAppearance& MonsterFactory::RandomAppearance()
	{
		static std::mt19937 random{ std::random_device{}() };
		std::uniform_int_distribution<size_t> distribution(0, appearances.size() - 1);
		return appearances[distribution(random)];
	}

	VampireFactory::VampireFactory()
		: MonsterFactory({
			VampireAppearance("vampire1"),
			VampireAppearance("vampire2"),
			VampireAppearance("vampire3")
		})
	{
	}

	std::unique_ptr<Monster> VampireFactory::CreateMonster(const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings, const MonsterAppearance& appearance)
	{
		return std::make_unique<Monster>(position, chaseSettings, appearance);
	}

	MonsterCatalog::MonsterCatalog()
	{
		Register(MonsterId::Vampire, std::make_unique<VampireFactory>());
	}

	void MonsterCatalog::Register(MonsterId id, std::unique_ptr<MonsterFactory> factory)
	{
		if (factory == nullptr)
		{
			throw std::invalid_argument("Monster factory is null");
		}

		factories[id] = std::move(factory);
	}

	std::unique_ptr<Monster> MonsterCatalog::Create(MonsterId id, const Engine::Vector2Df& position, const Engine::ChaseSettings& chaseSettings)
	{
		const auto found = factories.find(id);
		if (found == factories.end())
		{
			throw std::invalid_argument("Unknown monster id");
		}

		return found->second->Create(position, chaseSettings);
	}
}
