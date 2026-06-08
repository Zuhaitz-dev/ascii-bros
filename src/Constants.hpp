#pragma once

constexpr int GRID_WIDTH = 210;
constexpr int GRID_HEIGHT = 13;
constexpr int VIEW_COLS = 26;

constexpr float GRAVITY = 0.16f;
constexpr float JUMP_FORCE = -1.25f;
constexpr float WALK_SPEED = 0.50f;
constexpr float ENEMY_SPEED = -0.12f;

constexpr int STARMAN_DURATION = 600;
constexpr float STARMAN_SPEED_BOOST = 0.70f;
constexpr int INVINCIBILITY_FRAMES = 120;
constexpr int MAX_FIREBALLS = 2;
constexpr float FIREBALL_SPEED = 0.4f;
constexpr float FIREBALL_GRAVITY = 0.12f;
constexpr float FIREBALL_BOUNCE = -0.3f;

constexpr float FIREBAR_SPEED = 0.04f;

enum class PlayerState { Small, Super, Fire, Dead };

namespace Tile {
    constexpr char Empty        = ' ';
    constexpr char Brick        = '#';
    constexpr char SolidBlock   = 'X';
    constexpr char Question     = '?';
    constexpr char SpentBlock   = '_';
    constexpr char HiddenBlock  = 'h';
    constexpr char Coin        = 'o';
    constexpr char PipeLeft     = '[';
    constexpr char PipeRight    = ']';
    constexpr char Ground       = '=';
    constexpr char Water        = '~';
    constexpr char LiftLeft     = '(';
    constexpr char LiftRight    = ')';
    constexpr char Trampoline   = 'T';
    constexpr char Flagpole     = '|';
    constexpr char Flag         = '>';
    constexpr char BowserBridge = 'B';
    constexpr char Axe          = 'A';
}

static constexpr bool IsSolidTile(char t) {
    return t != Tile::Empty && t != Tile::Flagpole && t != Tile::Flag
        && t != Tile::Water && t != Tile::HiddenBlock && t != Tile::Coin;
}
