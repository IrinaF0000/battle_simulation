# Architecture Roadmap

The publication-oriented roadmap was completed in small, buildable milestones. The current repository keeps the roadmap as context for design decisions and optional future work.

## Completed milestones

### M1. Verification foundation

- Linux CI workflow.
- Repeatable CMake build.
- Test and scenario commands documented.
- Build/cache ignores.

### M2. Deterministic scenarios

- Deterministic simulation documentation.
- Golden-output validation.
- Repeated scenario replay test.

### M3. Architecture boundary cleanup

- Legacy command parsing and legacy output moved under `IO/LegacyCommands`.
- Direct `Core -> Features/*` include dependency removed.
- Battle components, events, recipes, map resource, systems, and mutations are feature-owned.

### M4. Game loop and system order

- Runtime loop documented.
- Scheduler/system order documented.

### M5. Extensibility proof

- `Lancer` unit added through feature-owned code.
- Scenario and rule tests added.
- `examples/add-new-mechanic.md` documents the pattern.

### M6. Config-driven features

- Default config and optional CLI config path.
- Feature selection validation.
- Deterministic RNG seed override.

### M7. Data-driven archetype slice

- Minimal flat JSON archetype loader.
- `Duelist` example.
- Generic `SPAWN` scenario coverage.

### M8. Debug output and performance notes

- Optional debug output.
- Performance notes.

## Optional future work

- Broaden data-driven action graphs beyond the minimal archetype slice.
- Add focused micro-benchmarks for scheduler/rule execution.
- Add richer scenario inspection tools without introducing UI/graphics scope.

## Non-goals

- Real UI or graphics.
- Multiplayer/networking.
- Full scripting/editor system.
- Large gameplay scope before boundary cleanup.
