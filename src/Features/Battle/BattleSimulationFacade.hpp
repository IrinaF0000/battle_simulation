// Battle-facing scenario facade over feature-owned runtime systems.

#pragma once

#include "Core/CommonTypes.hpp"
#include "Core/Engine/GameContext.hpp"
#include "Features/Battle/EntityRecipe.hpp"

#include <cstdint>

namespace battle_sim::core
{
	class IWorldEventSink;
}

namespace battle_sim::features::battle
{
	class BattleSimulationFacade
	{
	public:
		explicit BattleSimulationFacade(core::IWorldEventSink& events);
		BattleSimulationFacade(core::IWorldEventSink& events, std::uint32_t rngSeed);

		void createMap(int width, int height);
		bool spawnEntity(const EntityRecipe& recipe);
		bool march(core::EntityId entityId, core::Position target);
		void run();

	private:
		core::engine::GameContext _game;

		void registerHandlers();
	};
}
