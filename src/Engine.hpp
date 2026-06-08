#pragma once
#include "Level.hpp"
#include "Entity.hpp"
#include "WorldData.hpp"
#include "LevelData.hpp"
#include "Constants.hpp"
#include <memory>
#include <vector>

struct FirebarSegment { int x, y; };
struct FirebarData {
    float pivotX = 0, pivotY = 0;
    int length = 0;
    float angle = 0;
    float speed = 0;
    bool clockwise = false;
    bool alive = false;
    std::vector<FirebarSegment> segments;
};
struct FloatingText {
    int x, y;
    std::string text;
    int timer;
    int initialTimer;
};

enum class GameState { Title, Playing, Transition, GameOver, Win };

class Engine {
private:
    Level level;
    bool running;
    int currentWorldNum;
    int currentLevelNum;
    int lives;

    float playerX, playerY;
    float velX, velY;
    bool isGrounded;

    int cameraX;
    int score;
    int coins;
    int gameTime;
    int framesCounter;

    int moveTimer;
    float lastAccel;

    PlayerState playerState;
    int invincibilityFrames;
    int starTimer;
    bool isDucking;
    int duckTimer;
    bool wHeld;

    GameState gameState;
    int transitionTimer;
    int gameOverTimer;

    bool isSliding;
    int slideNextWorld;
    int slideNextLevel;

    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<WorldData::EnemySpawn> enemySpawns;
    size_t nextSpawnIndex;

    std::vector<FirebarData> firebars;
    std::vector<WorldData::WarpEntry> warpPipes;
    std::vector<WorldData::BlockItem> blockItems;
    std::vector<FloatingText> floatingTexts;

    int GetPlayerHeight() const;
    bool IsPlayerVisible() const;
    bool IsInWater() const;

    void ProcessInput();
    void UpdatePhysics();
    void Render();
    void RenderTitle();
    void RenderTransition();
    void RenderGameOver();
    void RenderWin();
    void Die();
    bool CheckCollision(float x, float y) const;
    void HandleBlockHeadbutt(int tileX, int tileY);
    void LoadLevel(int world, int level);
    void SpawnEntity(EntityType type, float x, float y, float extra = 0);
    void DespawnOffscreen();
    void SpawnFromData();
    void HandleItemCollect(Entity& item);
    void HandleEnemyCollision(Entity& enemy);
    void UpdateFirebars();
    void CheckWarpPipe();
    void CollectCoin();
    void WarpTo(int targetWorld, int targetX, int targetY);
    void AddFloatingText(int x, int y, const std::string& text);

public:
    Engine();
    void Run();
};
