# Development Plan

This document records the publication-oriented development plan that shaped the current repository. The plan is complete; remaining items are optional future work rather than publication blockers.

## Project

C++20 sandbox for ECS-style battle simulation, feature packs, action rules, mutations, deterministic scenarios, tests, and AI-assisted development.

AI tools may help with decomposition, navigation, planning, documentation, tests, and review. Final responsibility stays with human review, builds, tests, and boundary checks.

## Completed milestones

1. Verification foundation.
2. Deterministic scenarios.
3. Architecture boundary cleanup.
4. Game loop and system-order documentation.
5. Extensibility proof.
6. Config-driven features.
7. Data-driven archetype slice.
8. Debug output and performance notes.

## Target boundary

```text
Core: ECS, resources, type-erased event bus, scheduler, registries, mutations, feature-pack interfaces.
Features/Battle: battle components, battle events, map resource, rules, selectors, effects, systems.
Features/UnitsClassic: built-in unit archetypes.
IO/LegacyCommands: text command parser, handlers, event-output adapter.
App: CLI, scenario runner, config loading, feature-pack catalog.
```

## Publication baseline

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/battle_sim commands_example.txt
./build/battle_sim commands_validation.txt
git diff --check
```

## Optional future cleanup

- Add broader data-driven action graph support.
- Add focused micro-benchmarks.
- Add richer scenario inspection tooling while keeping UI/graphics out of scope.
