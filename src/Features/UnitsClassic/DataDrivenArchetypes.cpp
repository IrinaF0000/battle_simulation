// Minimal JSON-defined archetype registration for the M7 data slice.

#include "Features/UnitsClassic/DataDrivenArchetypes.hpp"

#include "Features/Battle/EntityArchetypeRegistry.hpp"
#include "Features/UnitsClassic/ClassicActionRuleRecipes.hpp"

#include <fstream>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace battle_sim::features::units_classic
{
	namespace
	{
		std::string readAll(std::istream& input)
		{
			std::ostringstream buffer;
			buffer << input.rdbuf();
			return buffer.str();
		}

		std::optional<std::string> stringField(const std::string& json, const std::string& key)
		{
			const std::regex pattern("\"" + key + "\"\\s*:\\s*\"([^\"]*)\"");
			std::smatch match;
			if (!std::regex_search(json, match, pattern))
			{
				return std::nullopt;
			}
			return match[1].str();
		}

		std::optional<int> intField(const std::string& json, const std::string& key)
		{
			const std::regex pattern("\"" + key + "\"\\s*:\\s*(-?[0-9]+)");
			std::smatch match;
			if (!std::regex_search(json, match, pattern))
			{
				return std::nullopt;
			}
			return std::stoi(match[1].str());
		}

		std::optional<bool> boolField(const std::string& json, const std::string& key)
		{
			const std::regex pattern("\"" + key + "\"\\s*:\\s*(true|false)");
			std::smatch match;
			if (!std::regex_search(json, match, pattern))
			{
				return std::nullopt;
			}
			return match[1].str() == "true";
		}

		core::ecs::ActionRulesComponent rulesForTemplate(const std::string& actionTemplate)
		{
			if (actionTemplate == "adjacent-striker")
			{
				return makeSwordsmanActionRules();
			}
			if (actionTemplate == "lancer-reach")
			{
				return makeLancerActionRules();
			}
			throw std::runtime_error("Unknown data-driven actionTemplate: " + actionTemplate);
		}
	}

	void registerDataDrivenArchetypeJson(features::battle::EntityArchetypeRegistry& registry, std::istream& input)
	{
		const auto json = readAll(input);
		const auto id = stringField(json, "id");
		if (!id.has_value() || id->empty())
		{
			throw std::runtime_error("Data-driven archetype requires string field: id");
		}

		const auto actionTemplate = stringField(json, "actionTemplate").value_or("adjacent-striker");
		const auto hp = intField(json, "hp");
		if (!hp.has_value() || *hp <= 0)
		{
			throw std::runtime_error("Data-driven archetype requires positive integer field: hp");
		}

		features::battle::StatComponents stats;
		stats.health.hp = *hp;
		stats.strength.value = intField(json, "strength").value_or(0);
		stats.agility.value = intField(json, "agility").value_or(0);
		stats.range.value = intField(json, "range").value_or(0);
		stats.spirit.value = intField(json, "spirit").value_or(0);
		stats.power.value = intField(json, "power").value_or(0);

		features::battle::EntityArchetype archetype;
		archetype.mobile = boolField(json, "mobile").value_or(true);
		archetype.occupiesCell = boolField(json, "blocksCell").value_or(true);
		archetype.attackable = boolField(json, "attackable").value_or(true);
		archetype.actionBudget.pointsPerTurn = intField(json, "actionBudget").value_or(1);
		if (archetype.actionBudget.pointsPerTurn <= 0)
		{
			throw std::runtime_error("Data-driven archetype requires positive actionBudget");
		}
		archetype.actionRules = rulesForTemplate(actionTemplate);
		archetype.baseStatComponents = stats;

		registry.registerArchetype(*id, std::move(archetype));
	}

	void registerDataDrivenArchetypeFile(features::battle::EntityArchetypeRegistry& registry, const std::string& path)
	{
		std::ifstream file(path);
		if (!file)
		{
			throw std::runtime_error("Data-driven archetype file not found: " + path);
		}
		registerDataDrivenArchetypeJson(registry, file);
	}
}
