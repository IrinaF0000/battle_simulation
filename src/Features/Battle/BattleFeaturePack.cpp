// Battle feature-pack skeleton.

#include "Features/Battle/BattleFeaturePack.hpp"

#include "Core/Engine/GameContext.hpp"
#include "Features/Battle/Conditions/BattleConditionHandlers.hpp"
#include "Features/Battle/Effects/BattleEffects.hpp"
#include "Features/Battle/Policies/BattlePolicyAccess.hpp"
#include "Features/Battle/Selectors/BattleTargetSelectors.hpp"
#include "Features/Battle/Values/BattleValueResolvers.hpp"

namespace battle_sim::features::battle
{
	core::registry::HandlerId BattleFeaturePack::id() const
	{
		return core::registry::HandlerId::fromString("battle.basic");
	}

	void BattleFeaturePack::registerFeature(core::engine::GameContext& context, const core::config::FeatureConfig&)
	{
		battlePolicies(context);
		registerBattleValueResolvers(context.registries);
		registerBattleConditionHandlers(context.registries);
		registerBattleTargetSelectors(context.registries);
		registerBattleEffects(context.registries);
	}
}
