// ScenarioRunner wires the application and executes a scenario stream.

#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

namespace battle_sim::app
{
	struct RunOptions
	{
		std::optional<std::uint32_t> rngSeed;
		std::vector<std::string> archetypeFiles;
		bool debugSummary{false};
	};

	void runScenario(std::istream& input, std::ostream& output, RunOptions options = {});
}
