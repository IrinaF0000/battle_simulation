// Battle-owned turn loop and action rule execution.

#pragma once

#include "Core/CommonTypes.hpp"

namespace battle_sim::core::engine
{
	struct GameContext;
}

namespace battle_sim::features::battle
{
	struct BattleTurnResult
	{
		int ticksExecuted{};
		int actionsExecuted{};
	};

	class BattleTurnSystem
	{
	public:
		static BattleTurnResult run(core::engine::GameContext& game);
		static int executeEntity(core::engine::GameContext& game, core::EntityId entity);
	};
}
