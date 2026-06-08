# Level map files

The engine loads explicit map files named `world_W_L.map`, for example `world_1_1.map`. These files are generated artifacts and should come from `tools/extract_smb_levels.py` using your local SMBDIS-style disassembly.

Generate maps with:

```sh
make extract-levels SMB_DISASM=/path/to/smb1/src/levels/levels.asm
```

or directly:

```sh
python3 tools/extract_smb_levels.py /path/to/smb1/src/levels/levels.asm --out levels
```

The old in-code byte arrays are not a faithful SMB decoder. They are now only used when explicitly enabled:

```sh
ASCII_BROS_ALLOW_LEGACY_LEVELDATA=1 ./ascii-bros
```

Without extracted maps and without that flag, the engine loads a clean placeholder instead of showing corrupted pseudo-levels.

Each generated map contains exactly 13 rows. Rows may be different lengths; shorter rows are padded with empty space and the level width becomes the longest row, with a minimum width of the viewport. Lines beginning with `;` are comments and are ignored.

Tile symbols:

```text
. or space = empty
=          = ground
#          = brick
X          = solid block
?          = question block
_          = spent block
h          = hidden block
o          = coin
[ ]        = pipe left/right
~          = water
( )        = lift left/right
T          = trampoline
|          = flagpole
>          = flag
B          = Bowser bridge
A          = axe
```
