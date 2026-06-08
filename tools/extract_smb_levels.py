#!/usr/bin/env python3
"""Extract area object data into ascii-bros map files.

This tool expects a local SMB disassembly checkout/source file. It does not ship
Nintendo level data; it parses the data you provide and converts the area object
streams into the tile alphabet consumed by Level::LoadFromTextFile.

Known compatible source shape: doppelganger/SMBDIS-style labels, including
src/levels/levels.asm and src/levels/objects.asm from asm6-style reorganizations.
"""
from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Dict, Iterable, List, Tuple

GRID_HEIGHT = 13
MIN_WIDTH = 26
DEFAULT_WIDTH = 210

LEVEL_COMMENT_RE = re.compile(r";\s*level\s+([^\n]+)", re.IGNORECASE)
LABEL_RE = re.compile(r"^([A-Za-z_][A-Za-z0-9_]*):")
DB_RE = re.compile(r"\.db\s+(.+)$", re.IGNORECASE)
INCLUDE_RE = re.compile(r'\.include\s+"([^"]+)"', re.IGNORECASE)
HEX_RE = re.compile(r"\$([0-9a-fA-F]{1,2})")
LEVEL_TOKEN_RE = re.compile(r"(\d)-(\d)")

AREA_TYPE_BY_LABEL = {
    "Water": 0,
    "Ground": 1,
    "Underground": 2,
    "Castle": 3,
}


def strip_comment(line: str) -> str:
    return line.split(";", 1)[0].strip()


def read_asm_tree(root: Path) -> str:
    if root.is_dir():
        candidates = [root / "src" / "levels" / "levels.asm", root / "smb1.asm", root / "SMBDIS.ASM"]
        for candidate in candidates:
            if candidate.exists():
                return read_asm_tree(candidate)
        raise FileNotFoundError(f"could not find levels.asm/smb1.asm under {root}")

    seen: set[Path] = set()

    def read_file(path: Path) -> List[str]:
        path = path.resolve()
        if path in seen:
            return []
        seen.add(path)
        out: List[str] = []
        for line in path.read_text(errors="replace").splitlines():
            m = INCLUDE_RE.search(line)
            if m:
                inc = m.group(1)
                inc_path = path.parent / inc
                if not inc_path.exists():
                    for base in [path.parent, *path.parents]:
                        candidate = base / inc
                        if candidate.exists():
                            inc_path = candidate
                            break
                if inc_path.exists():
                    out.extend(read_file(inc_path))
                continue
            out.append(line)
        return out

    return "\n".join(read_file(root))


def split_db_values(expr: str) -> Iterable[str]:
    expr = strip_comment(expr)
    for part in expr.split(','):
        value = part.strip()
        if value:
            yield value


def parse_int_token(token: str, label_offsets: Dict[str, int]) -> int | str:
    token = token.strip()
    if token.startswith('<') or token.startswith('>'):
        return token[1:].strip()
    if token.startswith('$'):
        return int(token[1:], 16)
    if token.startswith('%'):
        return int(token[1:], 2)
    if token.isdigit():
        return int(token)
    if '-' in token:
        left, right = [p.strip() for p in token.split('-', 1)]
        if left in label_offsets and right in label_offsets:
            return label_offsets[left] - label_offsets[right]
    return token


def parse_labels_and_data(text: str) -> Tuple[Dict[str, List[int]], Dict[str, List[str]], Dict[str, List[str]]]:
    labels: Dict[str, List[int]] = {}
    raw_labels: Dict[str, List[str]] = {}
    level_labels: Dict[str, List[str]] = {}
    current: str | None = None
    pending_levels: List[str] = []
    label_offsets: Dict[str, int] = {}
    offset = 0

    lines = text.splitlines()
    for line in lines:
        comment = LEVEL_COMMENT_RE.search(line)
        if comment:
            pending_levels = [f"{w}-{l}" for w, l in LEVEL_TOKEN_RE.findall(comment.group(1))]

        label = LABEL_RE.match(line)
        if label:
            current = label.group(1)
            labels.setdefault(current, [])
            raw_labels.setdefault(current, [])
            label_offsets[current] = offset
            for level in pending_levels:
                level_labels.setdefault(level, []).append(current)
            pending_levels = []
            continue

        m = DB_RE.search(line)
        if not m or current is None:
            continue

        for token in split_db_values(m.group(1)):
            raw_labels[current].append(token)
            parsed = parse_int_token(token, label_offsets)
            if isinstance(parsed, int):
                labels[current].append(parsed & 0xff)
            offset += 1

    return labels, raw_labels, level_labels


def infer_area_type(label: str) -> int:
    for name, area_type in AREA_TYPE_BY_LABEL.items():
        if name in label:
            return area_type
    return 1


def make_blank(width: int, area_type: int) -> List[List[str]]:
    rows = [['.' for _ in range(width)] for _ in range(GRID_HEIGHT)]
    if area_type == 3:
        for y in (11, 12):
            for x in range(width):
                rows[y][x] = 'X'
    elif area_type == 0:
        for y in range(9, 11):
            for x in range(width):
                rows[y][x] = '~'
        for y in (11, 12):
            for x in range(width):
                rows[y][x] = '='
    else:
        for y in (11, 12):
            for x in range(width):
                rows[y][x] = '='
    return rows


def put(rows: List[List[str]], x: int, y: int, ch: str) -> None:
    if 0 <= y < GRID_HEIGHT and 0 <= x < len(rows[0]):
        rows[y][x] = ch


def hline(rows: List[List[str]], x: int, y: int, length: int, ch: str) -> None:
    for dx in range(max(0, length)):
        put(rows, x + dx, y, ch)


def vline(rows: List[List[str]], x: int, y: int, length: int, ch: str) -> None:
    for dy in range(max(0, length)):
        put(rows, x, y + dy, ch)


def pipe(rows: List[List[str]], x: int, y: int, height: int) -> None:
    for yy in range(y, min(11, y + max(1, height))):
        put(rows, x, yy, '[')
        put(rows, x + 1, yy, ']')


def flagpole(rows: List[List[str]], x: int) -> None:
    for y in range(3, 11):
        put(rows, x, y, '|')
    put(rows, x - 1, 3, '>')
    put(rows, x, 11, 'X')


def castle(rows: List[List[str]], x: int, y: int, length: int) -> None:
    for dx in range(max(4, length + 1)):
        height = 5 if dx in (0, max(3, length)) else 4
        for yy in range(max(2, y), 11):
            if yy >= 11 - height:
                put(rows, x + dx, yy, 'X')


def staircase(rows: List[List[str]], x: int, descending: bool = False) -> None:
    for step in range(8):
        height = 8 - step if descending else step + 1
        for dy in range(height):
            put(rows, x + step, 10 - dy, 'X')


def render_object(rows: List[List[str]], x: int, row: int, b2: int, area_type: int) -> None:
    high = (b2 & 0x70) >> 4
    low = b2 & 0x0f

    if row < 12:
        if high == 0:
            if low in (0, 1):
                put(rows, x, row, '?')
            elif low in (2, 3):
                put(rows, x, row, 'h')
            elif low in (4, 5, 6, 7, 8):
                put(rows, x, row, '#')
            elif low == 9:
                pipe(rows, x, row, 2)
            elif low == 10:
                put(rows, x, row, 'X')
            elif low == 11:
                put(rows, x, row, 'T')
            return

        if high in (0, 7):
            pipe(rows, x, row, max(2, low + 1))
        elif high == 1:
            hline(rows, x, row, low + 1, 'X' if area_type in (0, 3) else '#')
        elif high == 2:
            hline(rows, x, row, low + 1, '#')
        elif high == 3:
            hline(rows, x, row, low + 1, 'X')
        elif high == 4:
            hline(rows, x, row, low + 1, 'o')
        elif high == 5:
            vline(rows, x, row, low + 1, '#')
        elif high == 6:
            vline(rows, x, row, low + 1, 'X')
        return

    if row == 12:
        if high == 0:
            for dx in range(low + 1):
                put(rows, x + dx, 11, '.')
                put(rows, x + dx, 12, '.')
        elif high in (2, 3, 4):
            bridge_row = {2: 6, 3: 7, 4: 9}[high]
            hline(rows, x, bridge_row, low + 1, 'X')
        elif high == 5:
            for dx in range(low + 1):
                put(rows, x + dx, 10, '~')
                put(rows, x + dx, 11, '~')
                put(rows, x + dx, 12, '~')
        elif high == 6:
            hline(rows, x, 3, low + 1, '?')
        elif high == 7:
            hline(rows, x, 7, low + 1, '?')
        return

    if row == 13:
        if not (b2 & 0x40):
            return
        obj = b2 & 0x3f
        if obj == 0:
            pipe(rows, x, 7, 4)
        elif obj == 1:
            flagpole(rows, x)
        elif obj == 2:
            put(rows, x, 8, 'A')
        elif obj == 3:
            put(rows, x, 8, 'B')
        elif obj == 4:
            hline(rows, x, 11, 13, 'B')
        return

    if row == 14:
        return

    if row == 15:
        if high == 0:
            vline(rows, x, 0, 11, '|')
        elif high == 1:
            vline(rows, x, 1, 10, '|')
        elif high == 2:
            castle(rows, x, low, 4)
        elif high == 3:
            staircase(rows, x, bool(low & 0x08))
        elif high == 4:
            pipe(rows, x, max(7, low), 4)
        elif high == 5:
            vline(rows, x, 2, min(9, low + 1), 'o')


def decode_area(label: str, data: List[int], width: int) -> List[str]:
    area_type = infer_area_type(label)
    rows = make_blank(width, area_type)
    page = 0
    i = 2  # first two bytes are the area header
    while i + 1 < len(data):
        b1 = data[i]
        if b1 == 0xfd:
            break
        b2 = data[i + 1]
        row = b1 & 0x0f
        col = (b1 >> 4) & 0x0f

        if b2 & 0x80:
            page += 1
        if row == 13 and not (b2 & 0x40):
            page = b2 & 0x1f
            i += 2
            continue

        x = page * 16 + col
        render_object(rows, x, row, b2, area_type)
        i += 2
    return [''.join(r).rstrip('.') for r in rows]


def resolve_level_labels(level_labels: Dict[str, List[str]]) -> Dict[str, str]:
    resolved: Dict[str, str] = {}
    for level, labels in level_labels.items():
        # Prefer the main play area over intro/cloud/bonus labels when comments produce multiples.
        non_bonus = [l for l in labels if 'Area' in l and 'Bonus' not in l]
        if non_bonus:
            resolved[level] = non_bonus[-1]
    return resolved


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path, help='SMBDIS-style asm file or disassembly root directory')
    parser.add_argument('--out', type=Path, default=Path('levels'), help='output directory for world_W_L.map files')
    parser.add_argument('--width', type=int, default=DEFAULT_WIDTH, help='output map width in 16px columns')
    parser.add_argument('--list', action='store_true', help='list resolved level labels and exit')
    args = parser.parse_args()

    text = read_asm_tree(args.source)
    labels, _raw, level_labels = parse_labels_and_data(text)
    resolved = resolve_level_labels(level_labels)

    if args.list:
        for level in sorted(resolved, key=lambda s: tuple(map(int, s.split('-')))):
            print(f'{level}: {resolved[level]}')
        return 0

    args.out.mkdir(parents=True, exist_ok=True)
    count = 0
    for level, label in sorted(resolved.items(), key=lambda item: tuple(map(int, item[0].split('-')))):
        data = labels.get(label)
        if not data:
            raise SystemExit(f'missing data for label {label}')
        rows = decode_area(label, data, max(MIN_WIDTH, args.width))
        world, stage = level.split('-')
        out_path = args.out / f'world_{world}_{stage}.map'
        out_path.write_text('\n'.join(rows) + '\n')
        count += 1

    print(f'wrote {count} level maps to {args.out}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
