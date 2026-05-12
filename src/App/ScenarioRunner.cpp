// ScenarioRunner wires the application and executes a scenario stream.

#include "App/ScenarioRunner.hpp"

#include "Features/Battle/EntityArchetypeRegistry.hpp"
#include "Features/Battle/BattleSimulationFacade.hpp"
#include "Features/Register.hpp"
#include "Features/UnitsClassic/DataDrivenArchetypes.hpp"
#include "IO/LegacyCommands/CommandParser.hpp"
#include "IO/LegacyCommands/EventLog.hpp"
#include "IO/LegacyCommands/LegacyEventAdapter.hpp"
#include "IO/LegacyCommands/RegisterLegacyCommands.hpp"
#include "IO/System/TypeRegistry.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace battle_sim::app
{
	void runScenario(std::istream& input, std::ostream& output, RunOptions options)
	{
		TypeRegistry registry;
		auto eventLog = registry.emplace<EventLog>(output);
		auto sink = registry.emplace<io::LegacyEventAdapter, core::IWorldEventSink>(*eventLog);
		std::shared_ptr<features::battle::BattleSimulationFacade> sim;
		if (options.rngSeed.has_value())
		{
			sim = registry.emplace<features::battle::BattleSimulationFacade>(*sink, *options.rngSeed);
		}
		else
		{
			sim = registry.emplace<features::battle::BattleSimulationFacade>(*sink);
		}
		registry.emplace<features::battle::EntityArchetypeRegistry>();

		features::registerArchetypes(registry);

		auto archetypes = registry.get<features::battle::EntityArchetypeRegistry>();
		if (!archetypes)
		{
			throw std::runtime_error("EntityArchetypeRegistry not configured");
		}
		for (const auto& archetypeFile : options.archetypeFiles)
		{
			features::units_classic::registerDataDrivenArchetypeFile(*archetypes, archetypeFile);
		}

		io::CommandParser parser;
		io::legacy::registerLegacyCommands(parser, *sim, *sink, *archetypes);

		parser.parse(input);
		sim->run();
		if (options.debugSummary)
		{
			std::cerr << "[debug] scenario.complete rngSeed=" << (options.rngSeed.has_value() ? "fixed" : "random")
				<< " archetypeFiles=" << options.archetypeFiles.size() << '\n';
		}
	}
}
