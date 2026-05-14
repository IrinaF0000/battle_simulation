# Replay Viewer Development Plan

## Goal

Add a lightweight static HTML replay viewer for deterministic JSON traces produced by the battle simulation.

The viewer should make the project easier to understand visually while keeping the C++ runtime unchanged.

## Scope

Add:

- `tools/replay-viewer/index.html`
- `tools/replay-viewer/README.md`

Update:

- `README.md`
- `docs/json-trace.md`
- `docs/README.md`

## Constraints

- Do not change C++ source code.
- Do not change JSON trace schema.
- Do not add npm, React, Vite, or a frontend build system.
- Do not add backend/server code.
- Use plain HTML, CSS, and vanilla JavaScript.
- Keep the UI simple and readable.
- Keep the implementation easy to inspect.
- Do not add large generated trace files.

## Step 1. Inspect trace format

- Review `docs/json-trace.md`.
- Review existing trace tests and examples.
- List supported event names and fields.
- Confirm whether trace root is a JSON array.

## Step 2. Create viewer shell

- Add `tools/replay-viewer/index.html`.
- Add basic page layout.
- Add file input for loading `trace.json`.
- Add empty sections for map, controls, summary, selected unit, and event log.

## Step 3. Parse trace file

- Load local JSON through browser file input.
- Validate that the trace is an array.
- Show a clear error for invalid JSON.
- Store events in memory.
- Group events by `tick`.

## Step 4. Build replay state model

Track:

- map size
- current tick index
- units by id
- event counts
- selected unit id
- latest events for highlighting

Unit state should include:

- id
- type
- x
- y
- hp, if known
- alive/dead state
- last event

## Step 5. Implement event application

Support these events:

- `MAP_CREATED`
- `UNIT_SPAWNED`
- `MARCH_STARTED`
- `MARCH_ENDED`
- `UNIT_MOVED`
- `UNIT_ATTACKED`
- `UNIT_HEALED`
- `UNIT_DIED`

Apply events by updating replay state only.

Do not recalculate battle rules in JavaScript.

## Step 6. Render map grid

- Render map after `MAP_CREATED`.
- Show one cell per coordinate.
- Show units in their current cells.
- Display compact unit labels such as `S1`, `H2`, `T3`.
- Mark dead units as faded or crossed out.
- Highlight units involved in current-tick actions.

## Step 7. Add timeline controls

Add controls:

- Reset
- Previous tick
- Next tick
- Play/Pause
- Speed selector
- Current tick display

Implementation note:

- For Previous tick, rebuild state from the beginning up to the target tick.
- For small traces, prefer simple rebuild logic over complex reverse events.

## Step 8. Add event log

- Render all events in order.
- Highlight events from the current tick.
- Fade future events.
- Allow clicking an event to jump to its tick.
- Prefer compact readable formatting over raw JSON.

## Step 9. Add summary panel

Show:

- total ticks
- current tick
- units spawned
- alive units
- dead units
- moves
- marches
- attacks
- heals
- deaths
- event counts by type

## Step 10. Add selected unit panel

- Select a unit by clicking it on the map.
- Show id, type, position, hp if known, alive/dead state, and last event.
- Keep panel empty or show placeholder when no unit is selected.

## Step 11. Add user-friendly error states

Handle:

- no file loaded
- invalid JSON
- empty trace
- missing `MAP_CREATED`
- unsupported event name
- event missing expected fields

Unsupported events should be shown in the event log but should not crash the viewer.

## Step 12. Document viewer usage

Create `tools/replay-viewer/README.md` with:

- purpose
- how to generate a trace
- how to open the viewer
- supported events
- limitations

Example usage:

```bash
./build/battle_sim examples/basic-battle.commands --trace-json trace.json
```

Then open:

```text
tools/replay-viewer/index.html
```

and load `trace.json`.

## Step 13. Update main documentation

Update `README.md`:

- mention the replay viewer
- add short usage snippet
- link to `tools/replay-viewer/README.md`

Update `docs/json-trace.md`:

- mention that traces can be viewed in the static replay viewer

Update `docs/README.md`:

- add link to replay viewer docs

## Step 14. Add tiny demo trace only if useful

Optional:

- Add a very small trace fixture under `tools/replay-viewer/examples/`.
- Keep it tiny.
- Do not add large generated outputs.

Skip this step if existing examples are enough.

## Step 15. Final review

Check that:

- viewer opens directly in a browser
- valid trace loads successfully
- map appears
- units appear
- stepping by tick works
- Play/Pause works
- event log updates
- summary updates
- README links are correct
- no source code behavior changed

## Acceptance criteria

- Static viewer exists at `tools/replay-viewer/index.html`.
- No frontend build step is required.
- Existing JSON traces can be loaded through file input.
- Map and units are displayed.
- Tick-by-tick replay works.
- Event log and summary are visible.
- Dead units are visually distinguishable.
- Current-tick actions are highlighted.
- Documentation explains how to use the viewer.

## Out of scope

- Smooth animation between cells.
- Canvas rendering.
- Sprites or graphics assets.
- Scenario editor.
- Browser-based simulation.
- Rule evaluation in JavaScript.
- Trace schema redesign.
- C++ runtime changes.
