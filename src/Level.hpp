#pragma once
#include <array>
#include <string>
#include <utility>
#include "Constants.hpp"

struct LevelAreaHeader {
    int timeLimit = 400;
    int levelType = 0;
    bool drawCeiling = false;
};

class Level {
private:
    std::array<std::string, GRID_HEIGHT> grid;
    int totalWidth;
    LevelAreaHeader header;

public:
    Level();
    bool LoadFromTextFile(const std::string& path,
                          const LevelAreaHeader& areaHeader,
                          std::string* error = nullptr);
    void LoadTemplate(const LevelAreaHeader& areaHeader, int width = GRID_WIDTH);
    void SetTile(int x, int y, char tileValue);
    char GetTile(int x, int y) const;
    int GetWidth() const { return totalWidth; }
    void SetHeader(const LevelAreaHeader& h) { header = h; }
    const LevelAreaHeader& GetHeader() const { return header; }
};
