# C++20 Battle Simulation Sandbox

A compact C++20 turn-based battle simulator with an ECS-style runtime, feature-owned gameplay modules, deterministic scenario execution, JSON trace output, and lightweight local replay tooling.

The simulator reads text command files, creates a grid map, spawns units, applies movement orders, runs the battle loop, and writes a stable event log. Optional JSON traces can be inspected from the CLI or replayed in the browser.

## Functionality

- Text-command battle scenarios with stable stdout for regression checks.
- Feature-owned battle rules and built-in classic unit behaviors.
- Optional config files, fixed RNG seeds, tick limits, and JSON archetypes.
- JSON trace generation, CLI trace inspection, browser replay, and a local browser runner.
- CMake/CTest validation for deterministic behavior.

## Build And Test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Run

```bash
# Run a scenario
./build/battle_sim commands_example.txt

# Run with a config file
./build/battle_sim commands_example.txt config/default.cfg

# Generate a JSON trace
./build/battle_sim commands_example.txt --trace-json trace.json

# Inspect a JSON trace
./build/battle_sim inspect trace.json
```

For the local browser UI:

```bash
python tools/local-runner/server.py --exe build/battle_sim
```

Open `http://127.0.0.1:8765`. On Windows multi-config builds, pass `build/Debug/battle_sim.exe` or `build/Release/battle_sim.exe` if needed.

## Documentation

- [Documentation map](docs/README.md)
- [Architecture](docs/architecture.md)
- [Game loop](docs/game-loop.md)
- [Configuration](docs/configuration.md)
- [Data-driven archetypes](docs/data-driven-archetypes.md)
- [JSON trace format](docs/json-trace.md)
- [Deterministic simulation](docs/deterministic-simulation.md)
- [Replay viewer](tools/replay-viewer/README.md)
- [Local runner](tools/local-runner/README.md)
- [Adding a mechanic](examples/add-new-mechanic.md)
- [AI-assisted development workflow](docs/agent-workflow.md)

## License

MIT License.
