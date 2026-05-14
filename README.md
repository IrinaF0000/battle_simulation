# C++20 Gameplay Architecture Sandbox

A compact C++20 turn-based battle simulation used as a gameplay architecture sample.

The project focuses on deterministic simulation, ECS-style runtime design, feature-owned gameplay modules, data-driven extension points, trace tooling, CI-backed regression safety, and controlled AI-agent-assisted development.

Start with the quick start below, then read `docs/README.md` for the architecture and workflow map.

## What it demonstrates

- ECS-style state management and resource storage.
- Feature-pack based gameplay extension.
- Action rules, selectors, effects, and mutations.
- Deterministic scenario tests and golden-output validation.
- Config-driven feature selection and settings.
- Data-driven unit archetypes, including simple JSON rule arrays.
- Optional JSON trace output and trace inspection.
- CMake, CTest, and GitHub Actions CI.
- AI-assisted development workflow with build/test/review quality gates.

## Quick Start

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/battle_sim commands_example.txt
```

Preset flow:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Optional scenario config:

```bash
./build/battle_sim commands_example.txt config/default.cfg
./build/battle_sim commands_example.txt config/data-driven-duelist.cfg
./build/battle_sim commands_example.txt config/data-driven-spearman.cfg
./build/battle_sim commands_validation.txt
```

Trace and inspect flow:

```bash
./build/battle_sim commands_example.txt --trace-json trace.json
./build/battle_sim inspect trace.json
```

For visual replay, open `tools/replay-viewer/index.html` and load the generated trace.

Enable warnings as errors locally when tightening changes:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBATTLE_SIM_WARNINGS_AS_ERRORS=ON
cmake --build build --parallel
```

## Sample Output

```text
[1] MAP_CREATED width=8 height=5
[1] UNIT_SPAWNED unitId=1 unitType=Swordsman x=0 y=2
[1] MARCH_STARTED unitId=1 x=0 y=2 targetX=3 targetY=2
[2] UNIT_MOVED unitId=1 x=1 y=2
[2] UNIT_ATTACKED attackerUnitId=2 targetUnitId=6 damage=4 targetHp=2
```

Trace inspection:

```text
Battle summary
Ticks: 15
Units spawned: 6
Moves: 8
Attacks: 19
Deaths: 5
```

## Highlights

- Domain-neutral `Core` with ECS storage, resources, scheduler, registries, event bus, and mutation queue.
- Feature-owned battle modules for components, systems, policies, selectors, effects, events, and archetypes.
- Runtime composition driven by config-selected feature packs.
- Scheduled battle loop through `EngineRunner` and `Scheduler`.
- Deterministic tests, scenario replay checks, architecture boundary checker, and CMake presets.
- Optional JSON traces, `inspect` summaries, and a documented AI-agent workflow with scoped review gates.
- Static replay viewer for stepping through JSON trace events.

## Modules

- `Core`: ECS storage, resources, scheduling, type-erased event bus, registries, mutations, and feature-pack interfaces.
- `Features/Battle`: battle mechanics, battle resources, selectors, effects, mutations, and systems.
- `Features/UnitsClassic`: built-in unit archetypes and classic action-rule recipes.
- `IO/LegacyCommands`: text command parser and legacy event-output adapter.
- `App`: CLI, scenario runner, config loading, and feature wiring.

The command-line scenario runner keeps the legacy text-command format as an IO surface, while gameplay runtime logic lives in feature-owned systems. New mechanics are added through feature-owned code and covered by tests.

## Example extension

`examples/add-new-mechanic.md` describes the `Lancer` unit extension. `config/archetypes/spearman.json` shows a JSON-defined unit using rule arrays.

## AI-assisted development

AI tools may assist with decomposition, navigation, implementation, test ideas, and review checklists. Architecture decisions and final validation stay human-owned. See `docs/ai-assisted-development.md`, `docs/agent-workflow.md`, and the JSON trace case study.

## Documentation

- `docs/README.md`
- `docs/architecture.md`
- `docs/game-loop.md`
- `docs/configuration.md`
- `docs/data-driven-archetypes.md`
- `docs/json-trace.md`
- `tools/replay-viewer/README.md`
- `docs/agent-workflow.md`
- `docs/portfolio-summary.md`

## License

MIT License.
