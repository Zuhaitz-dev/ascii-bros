# Terminal ASCII Bros

A terminal-based platformer engine written in C++17.

## Features

- Physics engine with gravity, jumping, swimming, sliding
- Power-ups and items
- Enemy AI
- All 32 levels playable
- No dependencies beyond standard C++17

## Controls

| Key | Action |
|-----|--------|
| `W` | Jump / Swim up |
| `A` / `D` | Move |
| `S` | Duck / Enter pipe |
| `F` | Fire (when powered up) |
| `1`-`8` | Warp to world |
| `Q` | Quit |

## Build

```sh
make
./ascii-bros
```

## Level Data

Pre-built level files are in `levels/`. If you have a disassembly of the original ROM data, you can regenerate them:

```sh
make extract-levels SMB_DISASM=/path/to/disassembly.asm
```

See `levels/README.md` for the map format.

## License

MIT
