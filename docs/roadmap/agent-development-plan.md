# Agent Development Plan

This document defines the planned AI-assisted development tasks for evolving the project as a compact C++20 gamedev architecture sandbox.

The project is intended to demonstrate senior-level C++ engineering: modular architecture, ECS-style runtime design, feature-owned gameplay modules, deterministic execution, CI-backed regression safety, and controlled AI-agent-assisted development.

The goal is not to turn the project into a large game. The goal is to make the existing architecture cleaner, more explicit, more observable, and more convincing as a portfolio-quality engineering project.

Cleanup related to C-style `(void)` ignored-result casts is intentionally out of scope for this plan.

---

## Lead-agent operating mode

Codex should act as the lead engineering agent for this plan.

Codex is expected to:

- execute the tasks autonomously in the recommended order;
- split large tasks into small, reviewable implementation steps;
- spawn or simulate specialized sub-agents when useful, such as implementation, testing, review, documentation, and architecture agents;
- preserve existing behavior unless a task explicitly requires a behavior change;
- add or update tests for every behavior change;
- run build and test checks after meaningful changes;
- perform a review pass before moving to the next task;
- document assumptions and continue when a non-destructive decision is ambiguous;
- ask for user input only for destructive, irreversible, or security-sensitive decisions.

Each completed task should leave the repository in a buildable, testable state.

---

## Global constraints

- Keep Core independent from Features, IO, and App.
- Keep Features independent from App.
- Treat App as the runtime composition layer.
- Preserve legacy command output unless a task explicitly adds a new optional output mode.
- Avoid global singleton registries.
- Avoid broad formatting-only changes.
- Avoid unrelated cleanup.
- Do not move battle-specific gameplay rules into Core.
- Do not introduce graphics or UI into the C++ runtime.
- Do not add ML or external AI services.
- Do not turn JSON gameplay data into an arbitrary scripting engine.
- Do not disable or weaken tests to make the build pass.

---

## Standard verification

After each meaningful step, run the closest available equivalent of:

```bash
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

If CMake presets are added, also verify the documented preset flow.

Before completing the full plan, run the complete available verification suite.

---

## Task 1. Fix `MARCH_ENDED` coordinates

### Goal

Fix legacy `MARCH_ENDED` output so it reports the actual final unit position instead of default `0/0` coordinates.

### Suggested direction

- Extend the battle event data so the final position is carried by the event itself.
- Update the event publishing site.
- Update the legacy event adapter.
- Add a regression test with a unit marching to a non-zero coordinate.

Prefer an event shape similar to:

```cpp
struct MarchEndedEvent
{
    core::Tick tick{};
    core::EntityId entity{};
    core::Position position{};
};
```

### Constraints

- Do not make the legacy event adapter query the world.
- Preserve the existing legacy output format.
- Keep the change small and focused.

### Acceptance criteria

- `MARCH_ENDED` prints the actual final `x/y`.
- Existing legacy output format is preserved.
- Existing tests pass.
- A new regression test covers the fixed behavior.

---

## Task 2. Add architecture boundary checker

### Goal

Add a lightweight script that checks forbidden include dependencies between major project layers.

### Required rules

```text
src/Core must not include src/Features
src/Core must not include src/App
src/Core must not include src/IO

src/Features must not include src/App

src/IO must not include src/App
```

`src/App` is the composition layer and may depend on Core, Features, and IO.

### Suggested direction

- Add `scripts/check_architecture_boundaries.py`.
- Scan C++ `#include` directives.
- Produce clear diagnostics for violations.
- Add the check to CI if CI exists.

### Constraints

- Do not introduce external Python dependencies.
- Keep the script simple and maintainable.
- Do not rewrite includes unless a real violation is discovered.

### Acceptance criteria

- The script passes on the current repository.
- The script fails with a readable message if a forbidden include is introduced.
- CI runs the check.

---

## Task 3. Add `CMakePresets.json`

### Goal

Add CMake presets to make local and CI builds easier to reproduce.

### Suggested presets

- `dev`
- `release`
- `warnings`
- `asan-ubsan`

Suggested command flow:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

### Constraints

- Do not break existing manual CMake commands.
- Keep presets cross-platform where practical.
- The sanitizer preset may be Linux/Clang/GCC oriented.
- Do not require optional tools that are not already part of the project.

### Acceptance criteria

- Existing manual CMake commands still work.
- At least one documented preset builds and runs tests.
- README briefly documents how to use the presets.

---

## Task 4. Make enabled feature packs drive runtime registration

### Goal

Refactor runtime composition so config-selected feature packs actually drive runtime registration.

Target direction:

```text
config/default.cfg
    -> parse GameConfig
    -> validate feature ids
    -> create GameContext
    -> register core handlers
    -> register enabled feature packs through FeaturePackCatalog
    -> load configured archetype files
    -> run scenario
```

### Suggested direction

- Move runtime assembly logic into the App layer.
- Consider adding `src/App/RuntimeAssembly.hpp` and `src/App/RuntimeAssembly.cpp`.
- Reduce hard-wired feature registration in scenario running code.
- Avoid duplicated archetype registration paths.
- Prefer passing an already assembled `GameContext` into the battle facade instead of letting the facade configure its own private runtime.

### Constraints

- Preserve existing scenario outputs.
- Do not remove config validation.
- Do not make Core depend on Features or IO.
- Do not introduce global singleton registries.
- Prefer `GameContext` resources over separate gameplay registries where appropriate.

### Acceptance criteria

- Default scenario behavior is unchanged.
- Config-selected feature packs are registered through `FeaturePackCatalog`.
- `units.classic` archetypes come from the feature-pack/runtime composition path, not from a separate hard-wired path.
- Unknown feature pack ids fail with clear diagnostics.
- Disabled required features fail with clear diagnostics.
- Existing deterministic tests pass.

---

## Task 5. Move battle runtime onto `Scheduler` / `EngineRunner`

### Goal

Make the scheduled engine path the real battle runtime path.

Current shape to replace:

```text
BattleSimulationFacade::run()
    -> BattleTurnSystem::run(game)
```

Target shape:

```text
BattleSimulationFacade::run()
    -> EngineRunner(game).run()
        -> Scheduler phases
```

### Suggested direction

- Extract one-tick behavior from the current battle turn loop.
- Register battle systems into scheduler phases through the battle feature pack.
- Keep compatibility wrappers temporarily if needed.
- Preserve deterministic behavior and legacy output.

Possible phases:

```text
BeforeTick: check whether battle can continue
Tick: execute one battle tick
Cleanup: remove dead entities
AfterTick: stop if no actions happened or battle ended
Shutdown: optional final reporting hook
```

### Constraints

- Keep deterministic output identical.
- Do not move battle-specific rules into Core.
- Do not over-engineer the scheduler.
- Do not add graphics or UI.

### Acceptance criteria

- CLI battle runtime goes through `EngineRunner` / `Scheduler`.
- Legacy scenario output is unchanged.
- Existing deterministic tests pass.
- At least one test proves the scheduler path runs the battle.
- Architecture docs describe the real runtime path.

---

## Task 6. Add optional JSON trace output

### Goal

Add a second observable output format without replacing legacy stdout.

Preferred CLI shape:

```bash
battle_sim commands_example.txt --trace-json trace.json
```

Trace MVP:

```json
[
  {
    "tick": 1,
    "event": "MAP_CREATED",
    "width": 10,
    "height": 10
  },
  {
    "tick": 1,
    "event": "UNIT_SPAWNED",
    "unitId": 1,
    "unitType": "Swordsman",
    "x": 2,
    "y": 3
  }
]
```

### Suggested direction

- Add a JSON trace writer under IO.
- Emit deterministic event records.
- Keep the trace schema small.
- Document the schema.
- Add a golden trace test.

### Constraints

- Do not replace legacy output.
- JSON trace must be deterministic.
- Do not add a heavy JSON dependency unless clearly justified.
- If writing JSON manually, escape strings correctly.

### Acceptance criteria

- Running with trace enabled writes a valid JSON file.
- Legacy stdout remains unchanged.
- Trace includes tick, event type, and key event fields.
- Tests verify deterministic trace output.
- Documentation explains the trace format.

---

## Task 7. Add `inspect trace.json` command

### Goal

Add a small CLI command that reads a trace file and prints a summary.

Target usage:

```bash
battle_sim inspect trace.json
```

Example output:

```text
Battle summary
Ticks: 4
Units spawned: 5
Moves: 3
Attacks: 7
Deaths: 2
```

### Suggested direction

- Add an App-level trace inspection command.
- Keep it read-only.
- Do not require the full simulation runtime to inspect a trace.
- Handle malformed trace files with clear errors.

### Acceptance criteria

- `battle_sim inspect trace.json` prints a readable summary.
- Inspector has tests with a small fixture.
- Malformed trace files return clear errors.

---

## Task 8. Add one AI-agent workflow case study

### Goal

Add one concrete case study showing how a scoped agent task was executed, verified, and reviewed.

Suggested file:

```text
docs/case-studies/add-json-trace-agent-run.md
```

Alternative case-study topics:

- fixing `MARCH_ENDED` coordinates
- adding architecture boundary checks
- moving runtime onto scheduler

Suggested structure:

```md
# Case study: <task name>

## Context

## Task packet

## Agents involved

## Allowed files

## Forbidden changes

## Acceptance criteria

## Implementation summary

## Verification output

## Review findings

## Final patch notes

## Lessons learned
```

### Constraints

- Keep the case study concise and credible.
- Present the AI workflow as an engineering process, not marketing copy.
- Include concrete verification commands and review findings.
- Link the case study from README or AI workflow docs.

### Acceptance criteria

- The case study is readable from GitHub.
- It demonstrates scoped AI-agent work, tests, CI, and human review.
- README or AI workflow docs link to the case study.

---

## Task 9. Extend JSON archetypes from templates to simple rule arrays

### Goal

Add a small data-driven gameplay MVP that allows one new unit mechanic to be created without C++ changes.

This should not become a general scripting engine.

Suggested JSON shape:

```json
{
  "schemaVersion": 1,
  "id": "Spearman",
  "hp": 6,
  "strength": 2,
  "rules": [
    {
      "priority": 20,
      "condition": "core.always",
      "selector": "battle.effective-range",
      "targetPicker": "core.random-one",
      "effect": "battle.damage",
      "amount": 2,
      "minDistance": 2,
      "maxDistance": 2,
      "cost": 1
    },
    {
      "priority": 10,
      "condition": "core.always",
      "selector": "core.none",
      "effect": "battle.move",
      "stepDistance": 1,
      "cost": 1,
      "allowEmptyTargets": true
    }
  ]
}
```

Adjust field names to match existing project terminology.

Required validation diagnostics:

- missing `id`
- missing `hp`
- unknown handler id
- invalid numeric field
- empty `rules`
- unsupported `schemaVersion`
- invalid rule object

### Constraints

- Do not implement arbitrary scripting.
- Do not evaluate expressions from JSON.
- Do not duplicate battle logic in the parser.
- JSON should reference existing registered handlers by id.
- Keep schema small and documented.
- Add exactly one convincing JSON-defined unit example.

### Acceptance criteria

- A new unit type can be added through JSON only.
- Unknown handler ids produce clear validation errors.
- A golden scenario test proves the JSON-defined unit works.
- Documentation explains the supported schema and limitations.

---

## Recommended order

```text
1. Fix MARCH_ENDED coordinates
2. Add architecture boundary checker
3. Add CMakePresets.json
4. Make enabled feature packs drive runtime registration
5. Move battle runtime onto Scheduler / EngineRunner
6. Add optional JSON trace output
7. Add inspect trace.json command
8. Add one AI-agent workflow case study
9. Extend JSON archetypes to simple rule arrays
```

---

## Final acceptance criteria

The plan is complete when:

- Existing behavior is preserved unless explicitly changed by a task.
- Build passes.
- Tests pass.
- Architecture boundary check exists and passes.
- Runtime composition is driven by config-selected feature packs.
- Battle runtime uses the scheduled engine path.
- Optional JSON trace output exists.
- Trace inspector command exists.
- At least one AI-agent case study is documented.
- At least one new unit mechanic can be added through JSON only.
- README or docs explain the new architecture and workflow.

---

## Final report expected from Codex

After completing the plan, Codex should produce a concise engineering report with:

```md
# Final Report

## Summary

## Completed tasks

## Changed files by area

## Architecture changes

## Behavior changes

## Tests added or updated

## Verification commands run

## Known limitations

## Suggested next tasks
```
