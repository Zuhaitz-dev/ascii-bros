#pragma once
#include "Constants.hpp"
#include "WorldData.hpp"

namespace LevelData {

// World 1-1
const LevelAreaHeader World1_1_Header = { 400, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World1_1_Enemies = {
    {16, 10, EntityType::Goomba}, {28, 10, EntityType::Goomba},
    {42, 10, EntityType::Koopa},  {55, 10, EntityType::Goomba},
    {66, 10, EntityType::Goomba}, {76, 10, EntityType::Goomba},
    {92, 10, EntityType::Koopa},  {108, 10, EntityType::Goomba},
};
const std::vector<WorldData::BlockItem> World1_1_BlockItems = {
    {9, 7, EntityType::Mushroom},  {36, 7, EntityType::Mushroom},
    {62, 7, EntityType::OneUp},    {83, 7, EntityType::Starman},
    {116, 7, EntityType::Mushroom},
};
const std::vector<WorldData::FirebarSpawn> World1_1_Firebars = {};
const std::vector<WorldData::WarpEntry>    World1_1_Warps    = {};

// World 1-2
const LevelAreaHeader World1_2_Header = { 300, WorldData::TYPE_UNDERGROUND, true };
const std::vector<WorldData::EnemySpawn> World1_2_Enemies = {
    {10, 10, EntityType::Goomba}, {22, 10, EntityType::Goomba},
    {35, 10, EntityType::Koopa},  {48, 10, EntityType::Goomba},
    {65, 10, EntityType::Goomba},
};
const std::vector<WorldData::BlockItem> World1_2_BlockItems = {
    {9, 7, EntityType::Mushroom},  {32, 7, EntityType::Mushroom},
    {56, 7, EntityType::FireFlower},
};
const std::vector<WorldData::FirebarSpawn> World1_2_Firebars = {};
const std::vector<WorldData::WarpEntry>    World1_2_Warps    = {};

// World 1-3
const LevelAreaHeader World1_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World1_3_Enemies = {
    { 15, 10, EntityType::Goomba }, { 30, 10, EntityType::Goomba },
    { 50, 10, EntityType::Koopa  }, { 65, 10, EntityType::Goomba },
    { 85, 10, EntityType::Goomba }, { 110, 10, EntityType::Koopa },
};
const std::vector<WorldData::BlockItem> World1_3_BlockItems = {
    { 9, 7, EntityType::Mushroom },
    { 46, 7, EntityType::FireFlower },
    { 73, 7, EntityType::Starman },
    { 126, 7, EntityType::Mushroom },
};
const std::vector<WorldData::FirebarSpawn> World1_3_Firebars = {
    { 95, 8, 3, true },
};
const std::vector<WorldData::WarpEntry>    World1_3_Warps    = {};

// World 1-4
const LevelAreaHeader World1_4_Header = { 350, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World1_4_Enemies = {
    { 15, 10, EntityType::Koopa  },
    { 42, 10, EntityType::Koopa  },
    { 65, 10, EntityType::Koopa  },
    { 95, 10, EntityType::Koopa  },
    { 148, 8, EntityType::Bowser },
};
const std::vector<WorldData::BlockItem>    World1_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World1_4_Firebars   = {
    { 30, 8, 3, true },
    { 55, 8, 4, true },
    { 85, 8, 3, true },
    { 120, 8, 4, false },
};
const std::vector<WorldData::WarpEntry>    World1_4_Warps       = {};

// World 2-1
const LevelAreaHeader World2_1_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World2_1_Enemies = {
    { 15, 10, EntityType::Goomba }, { 28, 10, EntityType::Goomba },
    { 42, 10, EntityType::Koopa  }, { 55, 10, EntityType::Goomba },
    { 75, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem> World2_1_BlockItems = {
    { 20, 7, EntityType::Mushroom }, { 80, 7, EntityType::Mushroom },
};
const std::vector<WorldData::FirebarSpawn> World2_1_Firebars = {};
const std::vector<WorldData::WarpEntry>    World2_1_Warps    = {};

// World 2-2
const LevelAreaHeader World2_2_Header = { 300, WorldData::TYPE_UNDERWATER, false };
const std::vector<WorldData::EnemySpawn>   World2_2_Enemies    = {};
const std::vector<WorldData::BlockItem>    World2_2_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World2_2_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World2_2_Warps      = {};

// World 2-3
const LevelAreaHeader World2_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World2_3_Enemies = {
    { 15, 10, EntityType::Goomba }, { 30, 10, EntityType::Goomba },
    { 45, 10, EntityType::Koopa  }, { 60, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World2_3_BlockItems = { { 30, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World2_3_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World2_3_Warps      = {};

// World 2-4
const LevelAreaHeader World2_4_Header = { 300, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World2_4_Enemies = {
    { 20, 10, EntityType::Koopa }, { 45, 10, EntityType::Koopa }, { 70, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World2_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World2_4_Firebars   = { { 50, 8, 3, true } };
const std::vector<WorldData::WarpEntry>    World2_4_Warps      = {};

// World 3-1
const LevelAreaHeader World3_1_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World3_1_Enemies = {
    { 12, 10, EntityType::Goomba }, { 28, 10, EntityType::Goomba },
    { 42, 10, EntityType::Koopa  }, { 55, 10, EntityType::Goomba },
    { 75, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem> World3_1_BlockItems = {
    { 18, 7, EntityType::Mushroom }, { 65, 7, EntityType::Mushroom },
};
const std::vector<WorldData::FirebarSpawn> World3_1_Firebars = {};
const std::vector<WorldData::WarpEntry>    World3_1_Warps    = {};

// World 3-2
const LevelAreaHeader World3_2_Header = { 300, WorldData::TYPE_UNDERGROUND, true };
const std::vector<WorldData::EnemySpawn> World3_2_Enemies = {
    { 15, 10, EntityType::Goomba }, { 35, 10, EntityType::Koopa },
    { 55, 10, EntityType::Goomba }, { 75, 10, EntityType::Koopa },
};
const std::vector<WorldData::BlockItem>    World3_2_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World3_2_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World3_2_Warps      = {};

// World 3-3
const LevelAreaHeader World3_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World3_3_Enemies = {
    { 18, 10, EntityType::Goomba }, { 32, 10, EntityType::Koopa },
    { 48, 10, EntityType::Goomba }, { 62, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World3_3_BlockItems = { { 25, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World3_3_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World3_3_Warps      = {};

// World 3-4
const LevelAreaHeader World3_4_Header = { 300, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World3_4_Enemies = {
    { 25, 10, EntityType::Koopa }, { 55, 10, EntityType::Koopa }, { 85, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World3_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World3_4_Firebars   = { { 55, 8, 4, true }, { 80, 8, 3, false } };
const std::vector<WorldData::WarpEntry>    World3_4_Warps      = {};

// World 4-1
const LevelAreaHeader World4_1_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World4_1_Enemies = {
    { 10, 10, EntityType::Goomba }, { 22, 10, EntityType::Koopa },
    { 35, 10, EntityType::Goomba }, { 50, 10, EntityType::Goomba },
    { 68, 10, EntityType::Koopa  },
};
const std::vector<WorldData::BlockItem>    World4_1_BlockItems = { { 15, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World4_1_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World4_1_Warps      = {};

// World 4-2
const LevelAreaHeader World4_2_Header = { 300, WorldData::TYPE_UNDERGROUND, true };
const std::vector<WorldData::EnemySpawn> World4_2_Enemies = {
    { 18, 10, EntityType::Goomba }, { 38, 10, EntityType::Koopa },
    { 58, 10, EntityType::Goomba }, { 78, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World4_2_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World4_2_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World4_2_Warps      = {};

// World 4-3
const LevelAreaHeader World4_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World4_3_Enemies = {
    { 15, 10, EntityType::Goomba }, { 30, 10, EntityType::Goomba }, { 48, 10, EntityType::Koopa },
};
const std::vector<WorldData::BlockItem>    World4_3_BlockItems = { { 28, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World4_3_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World4_3_Warps      = {};

// World 4-4
const LevelAreaHeader World4_4_Header = { 300, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World4_4_Enemies = {
    { 20, 10, EntityType::Koopa }, { 50, 10, EntityType::Koopa },
    { 80, 10, EntityType::Goomba }, { 110, 10, EntityType::Koopa },
};
const std::vector<WorldData::BlockItem>    World4_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World4_4_Firebars   = {
    { 40, 8, 3, true }, { 70, 8, 4, true }, { 100, 8, 3, true },
};
const std::vector<WorldData::WarpEntry> World4_4_Warps = {};

// World 5-1
const LevelAreaHeader World5_1_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World5_1_Enemies = {
    { 12, 10, EntityType::Goomba }, { 28, 10, EntityType::Goomba },
    { 42, 10, EntityType::Koopa  }, { 58, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World5_1_BlockItems = { { 22, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World5_1_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World5_1_Warps      = {};

// World 5-2
const LevelAreaHeader World5_2_Header = { 300, WorldData::TYPE_UNDERGROUND, true };
const std::vector<WorldData::EnemySpawn> World5_2_Enemies = {
    { 15, 10, EntityType::Goomba }, { 35, 10, EntityType::Koopa }, { 55, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World5_2_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World5_2_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World5_2_Warps      = {};

// World 5-3
const LevelAreaHeader World5_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World5_3_Enemies = {
    { 20, 10, EntityType::Goomba }, { 40, 10, EntityType::Koopa },
    { 55, 10, EntityType::Goomba }, { 70, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World5_3_BlockItems = { { 35, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World5_3_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World5_3_Warps      = {};

// World 5-4
const LevelAreaHeader World5_4_Header = { 300, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World5_4_Enemies = {
    { 25, 10, EntityType::Koopa }, { 60, 10, EntityType::Koopa }, { 95, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World5_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World5_4_Firebars   = { { 50, 8, 3, true } };
const std::vector<WorldData::WarpEntry>    World5_4_Warps      = {};

// World 6-1
const LevelAreaHeader World6_1_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World6_1_Enemies = {
    { 10, 10, EntityType::Goomba }, { 25, 10, EntityType::Goomba },
    { 40, 10, EntityType::Koopa  }, { 55, 10, EntityType::Goomba },
    { 72, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World6_1_BlockItems = { { 20, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World6_1_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World6_1_Warps      = {};

// World 6-2
const LevelAreaHeader World6_2_Header = { 300, WorldData::TYPE_UNDERGROUND, true };
const std::vector<WorldData::EnemySpawn> World6_2_Enemies = {
    { 20, 10, EntityType::Goomba }, { 40, 10, EntityType::Koopa }, { 60, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World6_2_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World6_2_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World6_2_Warps      = {};

// World 6-3
const LevelAreaHeader World6_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World6_3_Enemies = {
    { 15, 10, EntityType::Goomba }, { 32, 10, EntityType::Goomba }, { 48, 10, EntityType::Koopa },
};
const std::vector<WorldData::BlockItem>    World6_3_BlockItems = { { 30, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World6_3_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World6_3_Warps      = {};

// World 6-4
const LevelAreaHeader World6_4_Header = { 300, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World6_4_Enemies = {
    { 30, 10, EntityType::Koopa }, { 65, 10, EntityType::Koopa }, { 100, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World6_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World6_4_Firebars   = { { 65, 8, 3, true }, { 90, 8, 4, true } };
const std::vector<WorldData::WarpEntry>    World6_4_Warps      = {};

// World 7-1
const LevelAreaHeader World7_1_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World7_1_Enemies = {
    { 10, 10, EntityType::Goomba }, { 28, 10, EntityType::Goomba },
    { 45, 10, EntityType::Koopa  }, { 60, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World7_1_BlockItems = { { 15, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World7_1_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World7_1_Warps      = {};

// World 7-2
const LevelAreaHeader World7_2_Header = { 300, WorldData::TYPE_UNDERWATER, false };
const std::vector<WorldData::EnemySpawn>   World7_2_Enemies    = {};
const std::vector<WorldData::BlockItem>    World7_2_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World7_2_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World7_2_Warps      = {};

// World 7-3
const LevelAreaHeader World7_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World7_3_Enemies = {
    { 18, 10, EntityType::Goomba }, { 35, 10, EntityType::Koopa },
    { 52, 10, EntityType::Goomba }, { 68, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World7_3_BlockItems = { { 25, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World7_3_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World7_3_Warps      = {};

// World 7-4
const LevelAreaHeader World7_4_Header = { 300, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World7_4_Enemies = {
    { 25, 10, EntityType::Koopa }, { 55, 10, EntityType::Koopa },
    { 85, 10, EntityType::Goomba }, { 120, 10, EntityType::Koopa },
};
const std::vector<WorldData::BlockItem>    World7_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World7_4_Firebars   = {
    { 45, 8, 3, true }, { 75, 8, 4, true }, { 105, 8, 3, true },
};
const std::vector<WorldData::WarpEntry> World7_4_Warps = {};

// World 8-1
const LevelAreaHeader World8_1_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World8_1_Enemies = {
    { 10, 10, EntityType::Goomba }, { 25, 10, EntityType::Goomba },
    { 40, 10, EntityType::Koopa  }, { 55, 10, EntityType::Goomba },
    { 75, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem> World8_1_BlockItems = {
    { 20, 7, EntityType::Mushroom }, { 50, 7, EntityType::Mushroom },
};
const std::vector<WorldData::FirebarSpawn> World8_1_Firebars = {};
const std::vector<WorldData::WarpEntry>    World8_1_Warps    = {};

// World 8-2
const LevelAreaHeader World8_2_Header = { 300, WorldData::TYPE_UNDERGROUND, true };
const std::vector<WorldData::EnemySpawn> World8_2_Enemies = {
    { 15, 10, EntityType::Goomba }, { 35, 10, EntityType::Koopa }, { 55, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World8_2_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World8_2_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World8_2_Warps      = {};

// World 8-3
const LevelAreaHeader World8_3_Header = { 382, WorldData::TYPE_OVERWORLD, false };
const std::vector<WorldData::EnemySpawn> World8_3_Enemies = {
    { 10, 10, EntityType::Goomba }, { 30, 10, EntityType::Koopa },
    { 48, 10, EntityType::Goomba }, { 62, 10, EntityType::Goomba },
};
const std::vector<WorldData::BlockItem>    World8_3_BlockItems = { { 30, 7, EntityType::Mushroom } };
const std::vector<WorldData::FirebarSpawn> World8_3_Firebars   = {};
const std::vector<WorldData::WarpEntry>    World8_3_Warps      = {};

// World 8-4
const LevelAreaHeader World8_4_Header = { 300, WorldData::TYPE_CASTLE, false };
const std::vector<WorldData::EnemySpawn> World8_4_Enemies = {
    { 20,  10, EntityType::Koopa  }, { 50,  10, EntityType::Koopa  },
    { 80,  10, EntityType::Goomba }, { 110, 10, EntityType::Koopa  },
    { 140, 10, EntityType::Koopa  }, { 180,  8, EntityType::Bowser },
};
const std::vector<WorldData::BlockItem>    World8_4_BlockItems = WorldData::EmptyBlockItems;
const std::vector<WorldData::FirebarSpawn> World8_4_Firebars   = {
    { 50, 8, 3, true }, { 80, 8, 4, true }, { 110, 8, 4, true }, { 140, 8, 4, true },
};
const std::vector<WorldData::WarpEntry> World8_4_Warps = {};

} // namespace LevelData
