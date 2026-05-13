# JSON Trace

The CLI can write an optional deterministic JSON event trace while keeping legacy stdout unchanged:

```bash
./build/battle_sim commands_example.txt --trace-json trace.json
./build/battle_sim commands_example.txt config/default.cfg --trace-json trace.json
```

Trace summary:

```bash
./build/battle_sim inspect trace.json
```

The trace is a JSON array. Each record contains `tick`, `event`, and event-specific fields.

Current event names:

- `MAP_CREATED`
- `UNIT_SPAWNED`
- `MARCH_STARTED`
- `MARCH_ENDED`
- `UNIT_MOVED`
- `UNIT_ATTACKED`
- `UNIT_HEALED`
- `UNIT_DIED`

The format is intended for inspection and regression tooling, not as a gameplay scripting API.
