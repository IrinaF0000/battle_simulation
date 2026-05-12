# C++20 Gameplay Architecture Sandbox

This repository is a compact C++20 turn-based battle simulation focused on gameplay architecture, feature modularity, deterministic validation, and AI-assisted development with human-owned review.

The project is intended as a readable engineering sample for gameplay logic, extensible C++ architecture, and verification practices.

## What it demonstrates

- ECS-style state management and resource storage.
- Feature-pack based gameplay extension.
- Action rules, selectors, effects, and mutations.
- Deterministic scenario tests and golden-output validation.
- Config-driven feature selection and settings.
- A minimal data-driven unit archetype path.
- CMake, CTest, and GitHub Actions CI.
- AI-assisted development workflow with build/test/review quality gates.

## Build and test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/battle_sim commands_example.txt
./build/battle_sim commands_validation.txt
```

Optional scenario config:

```bash
./build/battle_sim commands_example.txt config/default.cfg
./build/battle_sim commands_example.txt config/data-driven-duelist.cfg
```

Enable warnings as errors locally when tightening changes:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBATTLE_SIM_WARNINGS_AS_ERRORS=ON
cmake --build build --parallel
```

## Modules

- `Core`: ECS storage, resources, scheduling, type-erased event bus, registries, mutations, and feature-pack interfaces.
- `Features/Battle`: battle mechanics, battle resources, selectors, effects, mutations, and systems.
- `Features/UnitsClassic`: built-in unit archetypes and classic action-rule recipes.
- `IO/LegacyCommands`: text command parser and legacy event-output adapter.
- `App`: CLI, scenario runner, config loading, and feature wiring.

The command-line scenario runner keeps the legacy text-command format as an IO surface, while gameplay runtime logic lives in feature-owned systems. New mechanics are added through feature-owned code and covered by tests.

## Example extension

`examples/add-new-mechanic.md` describes the `Lancer` unit extension. It adds a new classic unit archetype and action rule without changing `Core`, `App`, `IO`, CMake, or scenario command files.

## AI-assisted development

AI tools may assist with decomposition, navigation, boilerplate, test ideas, and review checklists. Architecture decisions and validation stay human-owned. See `docs/ai-assisted-development.md` and `docs/agent-workflow.md`.

## Documentation

- `docs/architecture.md`
- `docs/development-plan.md`
- `docs/architecture-roadmap.md`
- `docs/ai-assisted-development.md`
- `docs/agent-workflow.md`
- `docs/deterministic-simulation.md`
- `docs/game-loop.md`
- `docs/system-order.md`
- `docs/configuration.md`
- `docs/data-driven-archetypes.md`
- `docs/performance-notes.md`

## License

MIT License.
