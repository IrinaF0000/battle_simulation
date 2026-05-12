#include "Features/UnitsClassic/ClassicUnitArchetypes.hpp"

#include "Features/Battle/EntityArchetypeRegistry.hpp"
#include "Features/UnitsClassic/ClassicActionRuleRecipes.hpp"

namespace battle_sim::features::units_classic
{
	void registerClassicUnitArchetypes(features::battle::EntityArchetypeRegistry& registry)
	{
		registry.registerArchetype(
			"Swordsman",
			features::battle::EntityArchetype{true, true, true, core::ecs::ActionBudgetComponent{1}, makeSwordsmanActionRules()});
		registry.registerArchetype(
			"Lancer",
			features::battle::EntityArchetype{true, true, true, core::ecs::ActionBudgetComponent{1}, makeLancerActionRules()});
		registry.registerArchetype(
			"Hunter",
			features::battle::EntityArchetype{true, true, true, core::ecs::ActionBudgetComponent{1}, makeHunterActionRules()});
		registry.registerArchetype(
			"Tower",
			features::battle::EntityArchetype{false, true, true, core::ecs::ActionBudgetComponent{1}, makeTowerActionRules()});
		registry.registerArchetype(
			"Raven",
			features::battle::EntityArchetype{true, false, true, core::ecs::ActionBudgetComponent{1}, makeRavenActionRules()});
		registry.registerArchetype(
			"Healer",
			features::battle::EntityArchetype{true, true, true, core::ecs::ActionBudgetComponent{1}, makeHealerActionRules()});
		registry.registerArchetype(
			"Mine",
			features::battle::EntityArchetype{
				false,
				false,
				false,
				core::ecs::ActionBudgetComponent{1},
				makeMineActionRules(),
				features::battle::StatComponents{
					features::battle::HealthComponent{1},
					features::battle::StrengthComponent{},
					features::battle::AgilityComponent{},
					features::battle::RangeComponent{},
					features::battle::SpiritComponent{},
					features::battle::PowerComponent{}}});
	}
}
