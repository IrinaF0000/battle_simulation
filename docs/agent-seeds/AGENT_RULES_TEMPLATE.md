# Agent Rules

## Read order

1. `AGENT_CONTEXT.md`
2. `AGENT_RULES.md`
3. `AGENT_WORKFLOW.md`
4. `AGENT_PROGRESS.md`
5. task files only

Use search before opening large files.

## Identity

- Keep current names and framing.
- Do not add unrelated history notes.
- Describe agents as assistance under human ownership.

## Architecture

- Preserve module boundaries.
- Put domain behavior in the owning module.
- Do not add cross-module dependencies unless allowed.

## Scope

- One task at a time.
- Edit only allowed files.
- Do not start future milestones.
- Do not fix unrelated issues.
- Do not commit generated artifacts or local files.

## Role boundaries

- Implementation edits allowed files only.
- Verification runs checks only and does not edit files.
- Review reports findings only and does not edit files.
- Fix edits only files needed for review findings.

## Verification

- Keep the project compiling.
- Run task checks.
- Report failed/skipped checks.
- Run `git diff --check` before review.
