# Agent Workflow

This file describes the optional AI-agent workflow used while developing this repository. It is included to make the process transparent and reproducible.

## Project identity

- Project: `battle_simulation`
- Executable: `battle_sim`
- Namespace: `battle_sim`
- Scope: C++20 ECS-style battle simulation sandbox
- Non-goals: finished game, UI demo, graphics demo, multiplayer backend

## Stable rules

- Keep `Core` generic; gameplay concepts belong in feature modules.
- Put battle behavior in feature-owned code.
- Preserve command syntax and `UNIT_*` compatibility output unless a task explicitly changes it.
- Do not commit build artifacts, generated binaries, caches, or local IDE files.
- Describe agents as support tools; humans own design, review, and validation.

## Roles

- **Coordinator**: selects one task, writes the task packet, and checks scope.
- **Implementation agent**: edits only files allowed by the task.
- **Verification agent**: runs checks only and does not edit files.
- **Review agent**: reviews scoped diffs and files only.
- **Fix agent**: fixes only review findings inside the current scope.

## Cycle

1. Write a task packet.
2. Implement the task.
3. Run build, tests, scenarios, and `git diff --check`.
4. Review behavior and architecture boundaries.
5. Fix review findings.
6. Update docs only after verification.

## Task packet template

```text
Step:
Goal:
Allowed files:
Forbidden files:
Non-goals:
Required context:
Acceptance criteria:
Verification commands:
Review scope:
```

## Baseline verification

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/battle_sim commands_example.txt
./build/battle_sim commands_validation.txt
git diff --check
```
