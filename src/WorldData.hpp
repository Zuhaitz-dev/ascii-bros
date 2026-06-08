#pragma once
#include <vector>
#include "Entity.hpp"

namespace WorldData {
    constexpr int TYPE_OVERWORLD    = 0;
    constexpr int TYPE_UNDERGROUND  = 1;
    constexpr int TYPE_CASTLE       = 2;
    constexpr int TYPE_UNDERWATER   = 3;

    struct EnemySpawn {
        int x, y;
        EntityType type;
    };

    struct FirebarSpawn {
        int x, y;
        int length;
        bool clockwise;
    };

    struct WarpEntry {
        int pipeX, pipeY;
        int targetWorld;
        int targetX, targetY;
    };

    struct BlockItem {
        int x, y;
        EntityType type;
    };

        const std::vector<FirebarSpawn> EmptyFirebars = {};


    const std::vector<WarpEntry>    EmptyWarps    = {};
    const std::vector<BlockItem>    EmptyBlockItems = {};
}
