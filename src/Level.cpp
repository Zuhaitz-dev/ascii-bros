#include "Level.hpp"
#include "Constants.hpp"
#include "WorldData.hpp"
#include <algorithm>
#include <array>
#include <fstream>


static char NormalizeMapTile(char c) {
    switch (c) {
        case '.': return Tile::Empty;
        case ' ': return Tile::Empty;
        case '#': return Tile::Brick;
        case 'X': return Tile::SolidBlock;
        case '?': return Tile::Question;
        case '_': return Tile::SpentBlock;
        case 'h': return Tile::HiddenBlock;
        case 'o': return Tile::Coin;
        case '[': return Tile::PipeLeft;
        case ']': return Tile::PipeRight;
        case '=': return Tile::Ground;
        case '~': return Tile::Water;
        case '(': return Tile::LiftLeft;
        case ')': return Tile::LiftRight;
        case 'T': return Tile::Trampoline;
        case '|': return Tile::Flagpole;
        case '>': return Tile::Flag;
        case 'B': return Tile::BowserBridge;
        case 'A': return Tile::Axe;
        default:  return '\0';
    }
}

static void ApplyAreaPostProcessing(std::array<std::string, GRID_HEIGHT>& grid, int totalWidth,
                                    const LevelAreaHeader& header) {
    if (header.levelType != WorldData::TYPE_UNDERWATER) return;

    for (int x = 0; x < totalWidth; ++x) {
        for (int y = 9; y < 11; ++y) {
            if (grid[y][x] == Tile::Empty) grid[y][x] = Tile::Water;
        }
        grid[11][x] = Tile::Ground;
        grid[12][x] = Tile::Ground;
    }
}

Level::Level() {
    totalWidth = GRID_WIDTH;
    grid.fill(std::string(totalWidth, Tile::Empty));
}


void Level::LoadTemplate(const LevelAreaHeader& areaHeader, int width) {
    header = areaHeader;
    totalWidth = std::max(width, VIEW_COLS);
    grid.fill(std::string(totalWidth, Tile::Empty));

    for (int x = 0; x < totalWidth; ++x) {
        if (header.drawCeiling) {
            grid[0][x] = Tile::SolidBlock;
            grid[1][x] = Tile::SolidBlock;
        }
        grid[11][x] = (header.levelType == WorldData::TYPE_CASTLE) ? Tile::SolidBlock : Tile::Ground;
        grid[12][x] = (header.levelType == WorldData::TYPE_CASTLE) ? Tile::SolidBlock : Tile::Ground;
    }

    ApplyAreaPostProcessing(grid, totalWidth, header);
}

bool Level::LoadFromTextFile(const std::string& path,
                             const LevelAreaHeader& areaHeader,
                             std::string* error) {
    std::ifstream file(path);
    if (!file) {
        if (error) *error = "map file not found";
        return false;
    }

    std::vector<std::string> rows;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty() && line[0] == ';') continue;
        rows.push_back(line);
    }

    if (rows.size() != GRID_HEIGHT) {
        if (error) *error = "expected " + std::to_string(GRID_HEIGHT) +
            " map rows, got " + std::to_string(rows.size());
        return false;
    }

    int newWidth = VIEW_COLS;
    for (const auto& row : rows) {
        newWidth = std::max(newWidth, static_cast<int>(row.size()));
    }

    std::array<std::string, GRID_HEIGHT> nextGrid;
    nextGrid.fill(std::string(newWidth, Tile::Empty));
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (size_t x = 0; x < rows[y].size(); ++x) {
            char tile = NormalizeMapTile(rows[y][x]);
            if (tile == '\0') {
                if (error) *error = "invalid tile '" + std::string(1, rows[y][x]) +
                    "' at row " + std::to_string(y) + ", col " + std::to_string(x);
                return false;
            }
            nextGrid[y][x] = tile;
        }
    }

    header = areaHeader;
    totalWidth = newWidth;
    grid = std::move(nextGrid);
    ApplyAreaPostProcessing(grid, totalWidth, header);
    return true;
}

char Level::GetTile(int x, int y) const {
    if (x < 0 || x >= totalWidth || y < 0 || y >= GRID_HEIGHT) return Tile::Empty;
    return grid[y][x];
}

void Level::SetTile(int x, int y, char tileValue) {
    if (x >= 0 && x < totalWidth && y >= 0 && y < GRID_HEIGHT) {
        grid[y][x] = tileValue;
    }
}
