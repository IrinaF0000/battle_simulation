// Program entry point: fills TypeRegistry, parses commands, runs simulation.

#include "App/ScenarioConfig.hpp"
#include "App/ScenarioRunner.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

int main(int argc, char** argv)
{
	using namespace battle_sim;

	if (argc != 2 && argc != 3)
	{
		throw std::runtime_error("Usage: battle_sim <commands-file> [config-file]");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	const auto config = argc == 3 ? app::loadGameConfig(argv[2]) : app::defaultGameConfig();
	app::runScenario(file, std::cout, app::runOptionsFromConfig(config));
	return 0;
}
