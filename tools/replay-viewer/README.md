# Replay Viewer

`tools/replay-viewer/index.html` is a static browser tool for inspecting deterministic JSON traces from the battle simulator.

## Usage

Generate a trace:

```bash
./build/battle_sim commands_example.txt --trace-json trace.json
```

Open `tools/replay-viewer/index.html` in a browser, load `trace.json`, then step through ticks or play the replay.

## Supported Events

- `MAP_CREATED`
- `UNIT_SPAWNED`
- `MARCH_STARTED`
- `MARCH_ENDED`
- `UNIT_MOVED`
- `UNIT_ATTACKED`
- `UNIT_HEALED`
- `UNIT_DIED`

Unsupported events stay visible in the event log and are skipped by the map replay.

## Scope

The viewer does not simulate gameplay rules. It reconstructs visible state from trace events, highlights current-tick actions, shows unit details, and reports malformed or incomplete events.
