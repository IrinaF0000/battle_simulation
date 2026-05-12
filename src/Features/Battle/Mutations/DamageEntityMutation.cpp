#include "Features/Battle/Mutations/DamageEntityMutation.hpp"

#include "Core/Engine/GameContext.hpp"
#include "Features/Battle/Events/BattleEvents.hpp"
#include "Features/Battle/Components/BattleComponentAccess.hpp"
#include "Features/Battle/Policies/BattlePolicyAccess.hpp"

#include <stdexcept>

namespace battle_sim::features::battle
{
	namespace
	{
		void require(bool condition, const char* message)
		{
			if (!condition)
			{
				throw std::runtime_error(message);
			}
		}
	}

	core::engine::Mutation makeDamageEntityMutation(
		core::ecs::EntityId source,
		core::ecs::EntityId target,
		int amount)
	{
		return core::engine::Mutation{
			[source, target, amount](core::engine::GameContext& game) {
				auto& world = game.world;
				require(amount > 0, "Cannot apply non-positive battle damage");
				require(world.exists(source), "Cannot apply battle damage from missing source");
				require(world.exists(target), "Cannot apply battle damage to missing target");

				auto* health = healthOf(world, target);
				require(health != nullptr && health->hp > 0, "Cannot apply battle damage without positive health");

				health->hp -= amount;
				if (health->hp <= 0)
				{
					world.components().alive.remove(target);
				}
			},
			[source, target, amount](core::engine::GameContext& game) {
				const auto* health = healthOf(game.world, target);
				const int resultingHealth = health ? health->hp : 0;
				game.events.publish(
					features::battle::EffectAppliedEvent{game.world.tick(), source, target, features::battle::EffectType::Damage, amount, resultingHealth});

				if (resultingHealth <= 0 && battlePolicies(game).lifecycle.emitDeathBeforeCleanup)
				{
					game.events.publish(features::battle::EntityRemovedEvent{game.world.tick(), target, features::battle::RemovalReason::Death});
				}
			}};
	}
}
