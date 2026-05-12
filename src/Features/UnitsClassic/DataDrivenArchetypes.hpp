// Minimal JSON-defined archetype registration for the data-driven slice.

#pragma once

#include <iosfwd>
#include <string>

namespace battle_sim::features::battle
{
	class EntityArchetypeRegistry;
}

namespace battle_sim::features::units_classic
{
	void registerDataDrivenArchetypeJson(battle::EntityArchetypeRegistry& registry, std::istream& input);
	void registerDataDrivenArchetypeFile(battle::EntityArchetypeRegistry& registry, const std::string& path);
}
