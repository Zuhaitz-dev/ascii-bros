#include "Engine.hpp"
#include "Constants.hpp"
#include "Enemy.hpp"
#include "Items.hpp"
#include <array>
#include <iostream>
#include <string>
#include <thread>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#define CLR "\033["
#define RST CLR "0m"
#define RED    CLR "31m"
#define GREEN  CLR "32m"
#define YEL    CLR "33m"
#define BLU    CLR "34m"
#define MAG    CLR "35m"
#define CYN    CLR "36m"
#define WHT    CLR "37m"
#define BRED   CLR "1;31m"
#define BGREEN CLR "1;32m"
#define BYEL   CLR "1;33m"
#define BBLU   CLR "1;34m"
#define BMAG   CLR "1;35m"
#define BCYN   CLR "1;36m"

static void Beep() { std::cout << "\a" << std::flush; }

static const char* TileColor(char t) {
    switch (t) {
        case Tile::Ground:       return YEL;
        case Tile::Brick:        return YEL;
        case Tile::SolidBlock:   return WHT;
        case Tile::Question:     return BYEL;
        case Tile::SpentBlock:   return YEL;
        case Tile::HiddenBlock:  return RST;
        case Tile::PipeLeft:     return BGREEN;
        case Tile::PipeRight:    return BGREEN;
        case Tile::Water:        return BBLU;
        case Tile::Flagpole:     return GREEN;
        case Tile::Flag:         return GREEN;
        case Tile::Trampoline:   return BMAG;
        case Tile::BowserBridge: return RED;
        case Tile::Axe:          return BRED;
        default:                 return RST;
    }
}

#ifdef _WIN32
    #include <conio.h>
    void SetupTerminal() {}
#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <fcntl.h>

    struct termios orig_termios;
    int orig_fcntl;

    void RestoreTerminal() {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
        fcntl(STDIN_FILENO, F_SETFL, orig_fcntl);
    }

    void SetupTerminal() {
        tcgetattr(STDIN_FILENO, &orig_termios);
        orig_fcntl = fcntl(STDIN_FILENO, F_GETFL, 0);
        atexit(RestoreTerminal);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON | ECHONL);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        fcntl(STDIN_FILENO, F_SETFL, orig_fcntl | O_NONBLOCK);
    }
#endif

static const char* const FONT[][5] = {
    { "  #  ", " # # ", "#####", " # # ", " # # " },
    { " ### ", "#    ", " ### ", "    #", " ### " },
    { " ### ", "#    ", "#    ", "#    ", " ### " },
    { "#####", "  #  ", "  #  ", "  #  ", "#####" },
    { "#### ", "#   #", "#### ", "#   #", "#### " },
    { "#### ", "#   #", "#### ", "#  # ", "#   #" },
    { " ### ", "#   #", "#   #", "#   #", " ### " },
};

enum class FontChar { A, S, C, I, B, R, O };

static std::string BuildLetterRow(FontChar ch, int row) {
    return FONT[static_cast<int>(ch)][row];
}

template<size_t N>
static std::string BuildWord(const std::array<FontChar, N>& chars, int row) {
    std::string result;
    for (size_t i = 0; i < N; i++) {
        if (i > 0) result += "  ";
        result += BuildLetterRow(chars[i], row);
    }
    return result;
}

std::string GetTileString(char tileType) {
    switch (tileType) {
        case Tile::Ground:       return "###";
        case Tile::Brick:        return "###";
        case Tile::SolidBlock:   return "XXX";
        case Tile::Question:     return "[?]";
        case Tile::SpentBlock:   return "[_]";
        case Tile::HiddenBlock:  return "   ";
        case Tile::Coin:         return " o ";
        case Tile::PipeLeft:     return " |[";
        case Tile::PipeRight:    return "]| ";
        case Tile::Water:        return "~~~";
        case Tile::LiftLeft:     return " |\\";
        case Tile::LiftRight:    return "/| ";
        case Tile::Trampoline:   return "[T]";
        case Tile::Flagpole:     return " ||";
        case Tile::Flag:         return "~P ";
        case Tile::BowserBridge: return "BBB";
        case Tile::Axe:          return "=A=";
        default:                 return "   ";
    }
}


static std::string GetExternalLevelMapPath(int world, int level) {
    std::ostringstream path;
    path << "levels/world_" << world << "_" << level << ".map";
    return path.str();
}

static void GetLevelResources(int w, int l,
    const LevelAreaHeader*& header,
    const std::vector<WorldData::EnemySpawn>*& enemies,
    std::vector<WorldData::FirebarSpawn>& firebars,
    std::vector<WorldData::WarpEntry>& warps,
    const std::vector<WorldData::BlockItem>*& blockItems)
{
    using namespace WorldData;
    using namespace LevelData;

    header = nullptr;
    enemies = nullptr;
    blockItems = &WorldData::EmptyBlockItems;
    firebars.clear();
    warps.clear();

    auto setCore = [&](auto& h, auto& e) {
        header = &h;
        enemies = &e;
    };
    auto setBlockItems = [&](auto& items) { blockItems = &items; };
    auto setFirebars = [&](auto& levelFirebars) { firebars = levelFirebars; };
    auto setWarps = [&](auto& levelWarps) { warps = levelWarps; };

    #define CORE(prefix) setCore(prefix##_Header, prefix##_Enemies)
    #define AUX(prefix) do { setBlockItems(prefix##_BlockItems); setFirebars(prefix##_Firebars); setWarps(prefix##_Warps); } while (0)
    #define LEVEL(prefix) do { CORE(prefix); AUX(prefix); } while (0)

    // This is ugly like this but well.
    if (w==1 && l==1) LEVEL(LevelData::World1_1);
    else if (w==1 && l==2) LEVEL(LevelData::World1_2);
    else if (w==1 && l==3) LEVEL(LevelData::World1_3);
    else if (w==1 && l==4) LEVEL(LevelData::World1_4);
    else if (w==2 && l==1) LEVEL(LevelData::World2_1);
    else if (w==2 && l==2) LEVEL(LevelData::World2_2);
    else if (w==2 && l==3) LEVEL(LevelData::World2_3);
    else if (w==2 && l==4) LEVEL(LevelData::World2_4);
    else if (w==3 && l==1) LEVEL(LevelData::World3_1);
    else if (w==3 && l==2) LEVEL(LevelData::World3_2);
    else if (w==3 && l==3) LEVEL(LevelData::World3_3);
    else if (w==3 && l==4) LEVEL(LevelData::World3_4);
    else if (w==4 && l==1) LEVEL(LevelData::World4_1);
    else if (w==4 && l==2) LEVEL(LevelData::World4_2);
    else if (w==4 && l==3) LEVEL(LevelData::World4_3);
    else if (w==4 && l==4) LEVEL(LevelData::World4_4);
    else if (w==5 && l==1) LEVEL(LevelData::World5_1);
    else if (w==5 && l==2) LEVEL(LevelData::World5_2);
    else if (w==5 && l==3) LEVEL(LevelData::World5_3);
    else if (w==5 && l==4) LEVEL(LevelData::World5_4);
    else if (w==6 && l==1) LEVEL(LevelData::World6_1);
    else if (w==6 && l==2) LEVEL(LevelData::World6_2);
    else if (w==6 && l==3) LEVEL(LevelData::World6_3);
    else if (w==6 && l==4) LEVEL(LevelData::World6_4);
    else if (w==7 && l==1) LEVEL(LevelData::World7_1);
    else if (w==7 && l==2) LEVEL(LevelData::World7_2);
    else if (w==7 && l==3) LEVEL(LevelData::World7_3);
    else if (w==7 && l==4) LEVEL(LevelData::World7_4);
    else if (w==8 && l==1) LEVEL(LevelData::World8_1);
    else if (w==8 && l==2) LEVEL(LevelData::World8_2);
    else if (w==8 && l==3) LEVEL(LevelData::World8_3);
    else if (w==8 && l==4) LEVEL(LevelData::World8_4);

    #undef LEVEL
    #undef AUX
    #undef CORE
}

Engine::Engine() :
    running(true), currentWorldNum(1), currentLevelNum(1), lives(3),
    playerX(4.0f), playerY(9.0f), velX(0.0f), velY(0.0f), isGrounded(false),
    cameraX(0), score(500), coins(2), gameTime(382), framesCounter(0),
    moveTimer(0), lastAccel(0.0f),
    playerState(PlayerState::Small), invincibilityFrames(0), starTimer(0), isDucking(false), duckTimer(0), wHeld(false),
    gameState(GameState::Title), transitionTimer(0), gameOverTimer(0),
    isSliding(false), slideNextWorld(1), slideNextLevel(1),
    nextSpawnIndex(0)
{
    SetupTerminal();
    level.LoadFromTextFile("levels/world_1_1.map", LevelData::World1_1_Header);
    gameTime = LevelData::World1_1_Header.timeLimit;
    enemySpawns = LevelData::World1_1_Enemies;
    warpPipes = LevelData::World1_1_Warps;
    blockItems = LevelData::World1_1_BlockItems;
}

int Engine::GetPlayerHeight() const {
    if (playerState == PlayerState::Small || isDucking) return 1;
    return 2;
}

bool Engine::IsPlayerVisible() const {
    if (playerState == PlayerState::Dead) return false;
    if (invincibilityFrames > 0 && (invincibilityFrames / 3) % 2 == 0) return false;
    return true;
}

bool Engine::IsInWater() const {
    return level.GetTile(static_cast<int>(playerX), static_cast<int>(playerY)) == Tile::Water;
}

void Engine::AddFloatingText(int x, int y, const std::string& text) {
    floatingTexts.push_back({x, y, text, 40, 40});
}

void Engine::LoadLevel(int world, int levelNum) {
    currentWorldNum = world;
    currentLevelNum = levelNum;
    playerX = 4.0f; playerY = 9.0f;
    velX = 0.0f; velY = 0.0f;
    cameraX = 0;
    entities.clear();
    firebars.clear();
    floatingTexts.clear();
    nextSpawnIndex = 0;
    isDucking = false;
    isSliding = false;

    const LevelAreaHeader* header = nullptr;
    const std::vector<WorldData::EnemySpawn>* enemies = nullptr;
    const std::vector<WorldData::BlockItem>* items = nullptr;
    std::vector<WorldData::FirebarSpawn> fbSpawns;
    std::vector<WorldData::WarpEntry> warps;
    GetLevelResources(world, levelNum, header, enemies, fbSpawns, warps, items);
    if (!header || !enemies) { gameState = GameState::Transition; transitionTimer = 10; return; }
    warpPipes = warps;
    blockItems = items ? *items : std::vector<WorldData::BlockItem>();

    if (!level.LoadFromTextFile(GetExternalLevelMapPath(world, levelNum), *header))
        level.LoadTemplate(*header);
    gameTime = header->timeLimit;
    enemySpawns = *enemies;
    for (auto& fb : fbSpawns) {
        firebars.push_back({static_cast<float>(fb.x), static_cast<float>(fb.y),
            fb.length, 0.0f, FIREBAR_SPEED, fb.clockwise, true, {}});
    }

    gameState = GameState::Transition;
    transitionTimer = 50;
}

bool Engine::CheckCollision(float x, float y) const {
    char tile = level.GetTile(static_cast<int>(x), static_cast<int>(y));
    return IsSolidTile(tile);
}

void Engine::HandleBlockHeadbutt(int tileX, int tileY) {
    char tile = level.GetTile(tileX, tileY);
    if (tile == Tile::Question || tile == Tile::HiddenBlock) {
        level.SetTile(tileX, tileY, Tile::SpentBlock);

        bool foundItem = false;
        for (auto& b : blockItems) {
            if (b.x == tileX && b.y == tileY) {
                SpawnEntity(b.type, static_cast<float>(tileX), static_cast<float>(tileY - 1));
                foundItem = true;
                break;
            }
        }
        if (!foundItem) {
            score += 200; coins += 1; Beep();
        }
    } else if (tile == Tile::Brick) {
        if (playerState != PlayerState::Small) {
            level.SetTile(tileX, tileY, Tile::Empty);
            score += 50;
            AddFloatingText(tileX, tileY - 1, "+50");
        }
    }
}

void Engine::SpawnEntity(EntityType type, float x, float y, float extra) {
    switch (type) {
        case EntityType::Goomba:   entities.push_back(std::make_unique<Goomba>(x, y)); break;
        case EntityType::Koopa:    entities.push_back(std::make_unique<Koopa>(x, y)); break;
        case EntityType::Mushroom: entities.push_back(std::make_unique<Mushroom>(x, y)); break;
        case EntityType::FireFlower: entities.push_back(std::make_unique<FireFlower>(x, y)); break;
        case EntityType::Starman:  entities.push_back(std::make_unique<Starman>(x, y)); break;
        case EntityType::OneUp:    entities.push_back(std::make_unique<OneUp>(x, y)); break;
        case EntityType::Fireball: entities.push_back(std::make_unique<Fireball>(x, y, extra)); break;
        case EntityType::PiranhaPlant: entities.push_back(std::make_unique<PiranhaPlant>(x, y)); break;
        case EntityType::Bowser:   entities.push_back(std::make_unique<Bowser>(x, y)); break;
    }
}

void Engine::Die() {
    lives--;
    if (lives > 0) {
        LoadLevel(currentWorldNum, currentLevelNum);
    } else {
        gameState = GameState::GameOver;
        gameOverTimer = 120;
    }
}

void Engine::DespawnOffscreen() {
    for (auto it = entities.begin(); it != entities.end(); ) {
        auto& e = *it;
        if (!e->alive || e->x < cameraX - 5 || e->x > cameraX + VIEW_COLS + 5 || e->y > GRID_HEIGHT + 2) {
            it = entities.erase(it);
        } else {
            ++it;
        }
    }
}

void Engine::SpawnFromData() {
    int spawnRange = VIEW_COLS + 10;
    while (nextSpawnIndex < enemySpawns.size()) {
        auto& spawn = enemySpawns[nextSpawnIndex];
        if (spawn.x < cameraX + spawnRange) {
            SpawnEntity(spawn.type, static_cast<float>(spawn.x), static_cast<float>(spawn.y));
            nextSpawnIndex++;
        } else {
            break;
        }
    }
}

void Engine::HandleItemCollect(Entity& item) {
    item.alive = false;
    switch (item.type) {
        case EntityType::Mushroom:
            if (playerState == PlayerState::Small) { playerState = PlayerState::Super; playerY -= 1.0f; }
            else { score += 1000; }
            break;
        case EntityType::FireFlower:
            if (playerState == PlayerState::Small) { playerState = PlayerState::Super; playerY -= 1.0f; }
            else { playerState = PlayerState::Fire; }
            score += 1000;
            break;
        case EntityType::Starman: starTimer = STARMAN_DURATION; score += 1000; break;
        case EntityType::OneUp: lives++; score += 500; break;
        default: break;
    }
}

void Engine::HandleEnemyCollision(Entity& enemy) {
    if (starTimer > 0) {
        enemy.OnStomp();
        score += enemy.GetScoreValue();
        AddFloatingText(static_cast<int>(enemy.x), static_cast<int>(enemy.y) - 1, "+" + std::to_string(enemy.GetScoreValue()));
        return;
    }
    if (velY > 0 && enemy.type != EntityType::Bowser) {
        enemy.OnStomp();
        float bounce = wHeld ? 1.0f : 0.6f;
        velY = JUMP_FORCE * bounce;
        score += enemy.GetScoreValue();
        AddFloatingText(static_cast<int>(enemy.x), static_cast<int>(enemy.y) - 1, "+" + std::to_string(enemy.GetScoreValue()));
        Beep();
        return;
    }
    if (invincibilityFrames > 0) return;
    if (playerState == PlayerState::Fire || playerState == PlayerState::Super) {
        playerState = PlayerState::Small;
        playerY += 1.0f;
        invincibilityFrames = INVINCIBILITY_FRAMES;
        isDucking = false;
        Beep();
    } else if (playerState == PlayerState::Small) {
        Die();
    }
}

void Engine::CheckWarpPipe() {
    int footY = static_cast<int>(playerY + GetPlayerHeight());
    char tileBelow = level.GetTile(static_cast<int>(playerX), footY);
    if (tileBelow != Tile::PipeLeft && tileBelow != Tile::PipeRight) return;
    for (auto& warp : warpPipes) {
        if ((static_cast<int>(playerX) == warp.pipeX || static_cast<int>(playerX) == warp.pipeX + 1) &&
            footY == warp.pipeY) {
            WarpTo(warp.targetWorld, warp.targetX, warp.targetY);
            return;
        }
    }
}

void Engine::CollectCoin() {
    int cx = static_cast<int>(playerX);
    int cy = static_cast<int>(playerY);
    int playerHeight = GetPlayerHeight();
    for (int h = 0; h < playerHeight; h++) {
        int ty = cy - h;
        if (level.GetTile(cx, ty) == Tile::Coin) {
            level.SetTile(cx, ty, Tile::Empty);
            coins++;
            score += 200;
            Beep();
            AddFloatingText(cx, ty - 1, "+200");
        }
    }
}

void Engine::WarpTo(int targetWorld, int targetX, int targetY) {
    LoadLevel(targetWorld, 1);
    playerX = static_cast<float>(targetX);
    playerY = static_cast<float>(targetY);
}

void Engine::ProcessInput() {
    bool horizontalInputThisFrame = false;
    bool duckHeld = false;
    wHeld = false;
    char key = 0;

    #ifdef _WIN32
    while (_kbhit()) { key = _getch();
    #else
    while (read(STDIN_FILENO, &key, 1) > 0) {
    #endif
        if (isSliding) { if (key == 'q' || key == 'Q') running = false; continue; }

        if (key == 'a' || key == 'A') { velX -= 0.12f; lastAccel = -0.12f; horizontalInputThisFrame = true; }
        if (key == 'd' || key == 'D') { velX += 0.12f; lastAccel = 0.12f; horizontalInputThisFrame = true; }

        bool inWater = IsInWater();
        if ((key == 'w' || key == 'W') && playerState != PlayerState::Dead) {
            if (inWater) { velY = -0.5f; isGrounded = false; }
            else if (isGrounded) {
                velY = JUMP_FORCE; isGrounded = false;
                if (std::abs(velX) > 0.1f) { if (moveTimer < 12) moveTimer = 12; }
                else { lastAccel = 0.0f; moveTimer = 0; }
            }
            wHeld = true;
            Beep();
        }
        if ((key == 's' || key == 'S') && isGrounded) {
            if (playerState != PlayerState::Small && playerState != PlayerState::Dead) duckHeld = true;
            CheckWarpPipe();
        }
        if (key == 'f' || key == 'F') {
            if (playerState == PlayerState::Fire) {
                int fireballCount = 0;
                for (auto& e : entities) { if (e->type == EntityType::Fireball && e->alive) fireballCount++; }
                if (fireballCount < MAX_FIREBALLS) {
                    float dir = (velX >= 0) ? 1.0f : -1.0f;
                    if (std::abs(velX) < 0.01f) dir = 1.0f;
                    SpawnEntity(EntityType::Fireball, playerX + dir * 0.5f, playerY - GetPlayerHeight() * 0.5f, dir);
                    Beep();
                }
            }
        }
        if (key > '0' && key <= '8') {
            LoadLevel(key - '0', 1);
        }
        if (key == 'm' || key == 'M') {
            if (playerState == PlayerState::Small) { playerState = PlayerState::Super; playerY -= 1.0f; }
            else if (playerState == PlayerState::Super) { playerState = PlayerState::Fire; }
            else if (playerState == PlayerState::Fire) { playerState = PlayerState::Small; playerY += 1.0f; }
        }
        if (key == 'q' || key == 'Q') {
            running = false;
            entities.clear(); firebars.clear(); floatingTexts.clear();
        }
    }

    isDucking = (duckTimer > 0) && isGrounded && playerState != PlayerState::Small && playerState != PlayerState::Dead;
    if (duckHeld) duckTimer = 8;
    if (duckTimer > 0) duckTimer--;
    if (isSliding) return;

    if (horizontalInputThisFrame) { moveTimer = 3; }
    else { if (moveTimer > 0) { moveTimer--; velX += lastAccel * 0.5f; } }

    float maxSpeed = (starTimer > 0) ? STARMAN_SPEED_BOOST : WALK_SPEED;
    if (IsInWater()) maxSpeed *= 0.5f;
    if (velX > maxSpeed) velX = maxSpeed;
    if (velX < -maxSpeed) velX = -maxSpeed;

    if (moveTimer <= 0) {
        if (isGrounded) velX *= 0.88f;
        else velX *= 0.99f;
    }
}

void Engine::UpdateFirebars() {
    for (auto& fb : firebars) {
        if (!fb.alive) continue;
        fb.angle += fb.speed * (fb.clockwise ? 1.0f : -1.0f);
        fb.segments.clear();
        for (int i = 0; i < fb.length; i++) {
            float dist = static_cast<float>(i + 1);
            fb.segments.push_back({
                static_cast<int>(std::round(fb.pivotX + std::cos(fb.angle + i * 0.5f) * dist)),
                static_cast<int>(std::round(fb.pivotY + std::sin(fb.angle + i * 0.5f) * dist))
            });
        }
    }
}

void Engine::UpdatePhysics() {
    SpawnFromData();

    if (isSliding) {
        velX = 0; velY = 0.3f; playerY += velY;
        if (playerY >= 10.0f) {
            isSliding = false;
            LoadLevel(slideNextWorld, slideNextLevel);
        }
        return;
    }

    int playerHeight = GetPlayerHeight();
    bool inWater = IsInWater();
    float gravity = inWater ? GRAVITY * 0.25f : GRAVITY;
    velY += gravity;
    playerY += velY;
    if (inWater && velY > 0.3f) velY = 0.3f;

    char groundTile = level.GetTile(static_cast<int>(playerX), static_cast<int>(playerY));
    if (groundTile == Tile::Axe) {
        for (int x = 0; x < level.GetWidth(); x++)
            for (int y = 0; y < GRID_HEIGHT; y++)
                if (level.GetTile(x, y) == Tile::BowserBridge)
                    level.SetTile(x, y, Tile::Empty);
        score += 3000;
        level.SetTile(static_cast<int>(playerX), static_cast<int>(playerY), Tile::Empty);
    }

    if (CheckCollision(playerX, playerY)) {
        if (velY > 0) {
            char landedTile = level.GetTile(static_cast<int>(playerX), static_cast<int>(playerY));
            playerY = static_cast<int>(playerY) - 0.01f;
            isGrounded = true; velY = 0;
            if (landedTile == Tile::Trampoline) { velY = JUMP_FORCE * 2.0f; isGrounded = false; }
        }
    }

    if (playerHeight > 1 && CheckCollision(playerX, playerY - 1)) {
        if (velY < 0) {
            HandleBlockHeadbutt(static_cast<int>(playerX), static_cast<int>(playerY - 1));
            playerY = static_cast<int>(playerY - 1) + 2.01f; velY = 0;
        }
    } else if (playerHeight == 1 && velY < 0 && CheckCollision(playerX, playerY)) {
        HandleBlockHeadbutt(static_cast<int>(playerX), static_cast<int>(playerY));
        playerY = static_cast<int>(playerY) + 1.01f; velY = 0;
    }

    playerX += velX;
    bool hitHorizontal = CheckCollision(playerX, playerY);
    if (playerHeight > 1) hitHorizontal = hitHorizontal || CheckCollision(playerX, playerY - 1);
    if (hitHorizontal) {
        if (velX > 0) playerX = static_cast<int>(playerX) - 0.01f;
        else if (velX < 0) playerX = static_cast<int>(playerX) + 1.01f;
    }

    if (playerY > GRID_HEIGHT) { Die(); }

    for (auto& entity : entities)
        entity->Update(level);
    UpdateFirebars();

    for (auto& fb : firebars) {
        if (!fb.alive) continue;
        bool hit = false;
        for (auto& seg : fb.segments) {
            if ((static_cast<int>(playerX) == seg.x && static_cast<int>(playerY) == seg.y) ||
                (playerHeight > 1 && static_cast<int>(playerX) == seg.x && static_cast<int>(playerY - 1) == seg.y))
                { hit = true; break; }
        }
        if (hit && invincibilityFrames <= 0) {
            if (playerState == PlayerState::Fire || playerState == PlayerState::Super) {
                playerState = PlayerState::Small; playerY += 1.0f;
                invincibilityFrames = INVINCIBILITY_FRAMES; isDucking = false;
            } else if (playerState == PlayerState::Small) { Die(); }
        }
    }

    for (auto& entity : entities) {
        if (!entity->alive) continue;
        int eX = static_cast<int>(entity->x), eY = static_cast<int>(entity->y);
        bool playerHit = (static_cast<int>(playerX) == eX && static_cast<int>(playerY) == eY) ||
            (playerHeight > 1 && static_cast<int>(playerX) == eX && static_cast<int>(playerY - 1) == eY);
        if (!playerHit) continue;
        if (entity->type == EntityType::Fireball) continue;
        bool isItem = (entity->type == EntityType::Mushroom || entity->type == EntityType::FireFlower ||
                       entity->type == EntityType::Starman || entity->type == EntityType::OneUp);
        if (isItem) HandleItemCollect(*entity);
        else HandleEnemyCollision(*entity);
    }

    for (auto& fb : entities) {
        if (!fb->alive || fb->type != EntityType::Fireball) continue;
        for (auto& enemy : entities) {
            if (!enemy->alive || enemy->type == EntityType::Fireball) continue;
            bool isItem = (enemy->type == EntityType::Mushroom || enemy->type == EntityType::FireFlower ||
                          enemy->type == EntityType::Starman || enemy->type == EntityType::OneUp);
            if (isItem) continue;
            if (static_cast<int>(fb->x) == static_cast<int>(enemy->x) &&
                static_cast<int>(fb->y) == static_cast<int>(enemy->y)) {
                enemy->alive = false; fb->alive = false;
                score += enemy->GetScoreValue();
                AddFloatingText(static_cast<int>(enemy->x), static_cast<int>(enemy->y) - 1, "+" + std::to_string(enemy->GetScoreValue()));
            }
        }
    }

    for (auto& fb : entities) {
        if (!fb->alive || fb->type != EntityType::Fireball) continue;
        for (auto& fbar : firebars) {
            if (!fbar.alive) continue;
            for (auto& seg : fbar.segments)
                if (static_cast<int>(fb->x) == seg.x && static_cast<int>(fb->y) == seg.y) { fb->alive = false; }
        }
    }

    for (size_t i = 0; i < entities.size(); i++) {
        auto& a = entities[i];
        if (!a->alive || a->type != EntityType::Koopa) continue;
        Koopa* koopa = dynamic_cast<Koopa*>(a.get());
        if (!koopa || koopa->state != Koopa::State::ShellSliding) continue;
        for (size_t j = 0; j < entities.size(); j++) {
            if (i == j) continue;
            auto& b = entities[j];
            if (!b->alive || b->type == EntityType::Fireball) continue;
            if (static_cast<int>(koopa->x) == static_cast<int>(b->x) &&
                static_cast<int>(koopa->y) == static_cast<int>(b->y)) {
                b->alive = false;
                score += b->GetScoreValue();
                AddFloatingText(static_cast<int>(b->x), static_cast<int>(b->y) - 1, "+" + std::to_string(b->GetScoreValue()));
            }
        }
    }

    CollectCoin();
    char currentTile = level.GetTile(static_cast<int>(playerX), static_cast<int>(playerY));
    if (!isSliding && (currentTile == Tile::Flagpole || currentTile == Tile::Flag)) {
        isSliding = true;
        velX = 0; score += 1000;
        slideNextWorld = currentWorldNum;
        slideNextLevel = currentLevelNum + 1;
        if (slideNextLevel > 4) { slideNextWorld++; slideNextLevel = 1; }
        if (slideNextWorld > 8) { slideNextWorld = 1; }
        if (currentWorldNum == 8 && currentLevelNum == 4) {
            isSliding = false;
            gameState = GameState::Win;
            score += 1000;
            AddFloatingText(static_cast<int>(playerX), static_cast<int>(playerY) - 2, "+1000");
            return;
        }
        AddFloatingText(static_cast<int>(playerX), static_cast<int>(playerY) - 2, "+1000");
        return;
    }

    if (playerX - cameraX > VIEW_COLS / 2) {
        cameraX = static_cast<int>(playerX) - VIEW_COLS / 2;
        if (cameraX < 0) cameraX = 0;
        int maxCameraX = std::max(0, level.GetWidth() - VIEW_COLS);
        if (cameraX > maxCameraX) cameraX = maxCameraX;
    }

    if (invincibilityFrames > 0) invincibilityFrames--;
    if (starTimer > 0) starTimer--;

    for (auto it = floatingTexts.begin(); it != floatingTexts.end(); ) {
        it->timer--;
        if (it->timer <= 0) it = floatingTexts.erase(it);
        else ++it;
    }

    DespawnOffscreen();
    framesCounter++;
    if (framesCounter % 30 == 0 && gameTime > 0) gameTime--;
}

static std::string Center(const std::string& s, int width) {
    int pad = width - static_cast<int>(s.size());
    if (pad <= 0) return s;
    return std::string(pad / 2, ' ') + s + std::string(pad - pad / 2, ' ');
}

void Engine::RenderTitle() {
    std::string f = "\033[2J\033[H";
    std::string bar(78, '=');
    std::string thin(78, '-');
    f += bar + "\n";
    f += " TERMINAL ASCII BROS (NES ENGINE) | Built by Zuhaitz-dev\n";
    f += bar + "\n\n";
    std::array<FontChar, 5> asciiChars = {FontChar::A, FontChar::S, FontChar::C, FontChar::I, FontChar::I};
    std::array<FontChar, 4> brosChars  = {FontChar::B, FontChar::R, FontChar::O, FontChar::S};
    for (int row = 0; row < 5; row++) f += Center(BuildWord(asciiChars, row), 78) + "\n";
    f += "\n\n";
    for (int row = 0; row < 5; row++) f += Center(BuildWord(brosChars, row), 78) + "\n";
    f += "\n" + thin + "\n\n";
    f += Center("A TERMINAL PORT OF THE NES CLASSIC", 78) + "\n\n";
    std::string buttonLine = "+" + std::string(56, '-') + "+";
    std::string contentLine = "|                 PRESS ANY KEY TO START                 |";
    f += Center(buttonLine, 78) + "\n";
    f += Center(contentLine, 78) + "\n";
    f += Center(buttonLine, 78) + "\n";
    f += "\n" + Center("BUILT BY ZUHAITZ-DEV", 78) + "\n";
    f += "\n" + Center("[W] JUMP  [A/D] MOVE  [S] DUCK  [F] FIRE  [1-8] WRLD  [Q] EXIT", 78) + "\n";
    f += "\n" + thin + "\n" + bar + "\n";
    std::cout << f;
}

void Engine::RenderTransition() {
    std::string f = "\033[2J\033[H";
    std::string bar(78, '=');
    f += bar + "\n";
    f += Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n";
    f += Center("WORLD " + std::to_string(currentWorldNum) + "-" + std::to_string(currentLevelNum), 78) + "\n";
    f += Center(" ", 78) + "\n";
    f += Center("SCORE: " + std::to_string(score), 78) + "\n";
    f += Center("COINS: x" + std::to_string(coins), 78) + "\n";
    f += Center("LIVES: " + std::to_string(lives), 78) + "\n";
    f += Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n";
    f += std::string(78, '-') + "\n" + bar + "\n";
    std::cout << f;
}

void Engine::RenderGameOver() {
    std::string f = "\033[2J\033[H";
    std::string bar(78, '=');
    f += bar + "\n";
    f += Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n";
    f += Center("GAME OVER", 78) + "\n";
    f += Center(" ", 78) + "\n";
    f += Center("SCORE: " + std::to_string(score), 78) + "\n";
    f += Center(" ", 78) + "\n";
    f += Center("PRESS ANY KEY TO CONTINUE", 78) + "\n";
    f += Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n";
    f += std::string(78, '-') + "\n" + bar + "\n";
    std::cout << f;
}

void Engine::RenderWin() {
    std::string f = "\033[2J\033[H";
    std::string bar(78, '=');
    f += bar + "\n";
    f += Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n";
    f += Center("CONGRATULATIONS!", 78) + "\n";
    f += Center("YOU WIN!", 78) + "\n";
    f += Center(" ", 78) + "\n";
    f += Center("SCORE: " + std::to_string(score), 78) + "\n";
    f += Center(" ", 78) + "\n";
    f += Center("PRESS ANY KEY TO CONTINUE", 78) + "\n";
    f += Center(" ", 78) + "\n" + Center(" ", 78) + "\n" + Center(" ", 78) + "\n";
    f += std::string(78, '-') + "\n" + bar + "\n";
    std::cout << f;
}

void Engine::Render() {
    std::string frameBuffer = "\033[2J\033[H";
    std::string doubleRow(78, '=');
    std::string dashedRow(78, '-');
    frameBuffer += doubleRow + "\n";
    frameBuffer += " TERMINAL ASCII BROS (NES ENGINE) | Built by Zuhaitz-dev\n";
    frameBuffer += doubleRow + "\n";

    std::stringstream hudStream;
    hudStream << "  SCORE: " << std::setw(6) << std::setfill('0') << score
              << "      COINS: x" << std::setw(2) << std::setfill('0') << coins
              << "      WORLD: " << currentWorldNum << "-" << currentLevelNum
              << "      TIME: " << std::setw(3) << std::setfill('0') << gameTime;
    if (isSliding) hudStream << "      [SLIDING]";
    hudStream << "\n";
    frameBuffer += hudStream.str();
    frameBuffer += dashedRow + "\n\n";

    int playerHeight = GetPlayerHeight();
    bool playerVisible = IsPlayerVisible();

    std::string stateIndicator;
    switch (playerState) {
        case PlayerState::Small: stateIndicator = "SMALL"; break;
        case PlayerState::Super: stateIndicator = "SUPER"; break;
        case PlayerState::Fire:  stateIndicator = "FIRE";  break;
        case PlayerState::Dead:  stateIndicator = "DEAD";  break;
    }

    char playerChar = 'M';
    if (starTimer > 0) playerChar = ((framesCounter / 4) % 2 == 0) ? 'M' : '*';

    int levelType = level.GetHeader().levelType;
    bool drawCeiling = level.GetHeader().drawCeiling;

    struct TextOverlay { int x; int y; char c; };
    std::vector<TextOverlay> textOverlays;
    for (auto& ft : floatingTexts) {
        if (ft.timer <= 0) continue;
        int offset = (ft.initialTimer - ft.timer) / 6;
        int ty = ft.y - offset;
        for (size_t i = 0; i < ft.text.size(); i++)
            textOverlays.push_back({ft.x + static_cast<int>(i), ty, ft.text[i]});
    }

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        std::string viewportRowStr = "";
        for (int x = 0; x < VIEW_COLS; ++x) {
            int absoluteWorldX = cameraX + x;
            char tileUnit = level.GetTile(absoluteWorldX, y);

            if (drawCeiling && y <= 1 && tileUnit == Tile::Empty) tileUnit = Tile::SolidBlock;
            if (levelType == WorldData::TYPE_CASTLE) {
                if (tileUnit == Tile::Ground) tileUnit = Tile::SolidBlock;
                if (tileUnit == Tile::Brick) tileUnit = Tile::SolidBlock;
            }

            std::string tv = GetTileString(tileUnit);
            char dc = tv[1];
            const char* tcol = TileColor(tileUnit);
            const char* ecol = RST;

            char fbChar = ((framesCounter / 3) % 2 == 0) ? '*' : '#';
            for (auto& fb : firebars) {
                if (!fb.alive) continue;
                for (auto& seg : fb.segments)
                    if (absoluteWorldX == seg.x && y == seg.y) { dc = fbChar; ecol = BRED; }
            }

            for (auto& entity : entities) {
                if (!entity->alive) continue;
                if (absoluteWorldX == static_cast<int>(entity->x) && y == static_cast<int>(entity->y)) {
                    dc = entity->GetChar();
                    switch (entity->type) {
                        case EntityType::Goomba:   ecol = YEL; break;
                        case EntityType::Koopa:    ecol = GREEN; break;
                        case EntityType::Mushroom: ecol = BRED; break;
                        case EntityType::FireFlower: ecol = BRED; break;
                        case EntityType::Starman:  ecol = BYEL; break;
                        case EntityType::OneUp:    ecol = GREEN; break;
                        case EntityType::Fireball: ecol = BRED; break;
                        default: ecol = RST; break;
                    }
                }
            }

            if (playerVisible)
                for (int h = 0; h < playerHeight; h++)
                    if (absoluteWorldX == static_cast<int>(playerX) && y == static_cast<int>(playerY - h)) {
                        dc = playerChar; ecol = (starTimer > 0) ? BYEL : BRED;
                    }

            for (auto& ov : textOverlays)
                if (absoluteWorldX == ov.x && y == ov.y) { dc = ov.c; ecol = BYEL; }

            viewportRowStr += tcol;
            viewportRowStr += tv[0];
            viewportRowStr += ecol;
            viewportRowStr += dc;
            viewportRowStr += tcol;
            viewportRowStr += tv[2];
            viewportRowStr += RST;
        }
        frameBuffer += viewportRowStr + "\n";
    }
    frameBuffer += "\n" + dashedRow + "\n";

    frameBuffer += " ENGINE LOGISTICS:\n";
    std::stringstream logicStream;
    std::string stateLabel = !isGrounded ? (velY < 0 ? "JUMPING" : "FALLING") : (isDucking ? "DUCKING" : "RUNNING");

    const char* themeLabel = "OVERWORLD";
    if (levelType == WorldData::TYPE_UNDERGROUND) themeLabel = "UNDERGROUND";
    else if (levelType == WorldData::TYPE_CASTLE) themeLabel = "CASTLE";
    else if (levelType == WorldData::TYPE_UNDERWATER) themeLabel = "UNDERWATER";

    logicStream << " Pos: X:" << std::setw(3) << std::setfill('0') << static_cast<int>(playerX)
                << ", Y:" << std::setw(3) << std::setfill('0') << static_cast<int>(playerY)
                << " | Vel: X:" << (velX >= 0 ? "+" : "") << std::fixed << std::setprecision(1) << velX * 10
                << ", Y:" << (velY >= 0 ? "+" : "") << std::fixed << std::setprecision(1) << velY * 10
                << " | Theme: " << themeLabel
                << " | " << stateLabel
                << " | " << stateIndicator
                << (starTimer > 0 ? " [STAR]" : "")
                << (invincibilityFrames > 0 ? " [INV]" : "")
                << (IsInWater() ? " [WET]" : "")
                << (isSliding ? " [SLIDE]" : "")
                << "\n";
    frameBuffer += logicStream.str();
    frameBuffer += " Hotkeys: [A/D] Walk | [W] Jump | [S] Duck | [F] Fire | [1-8] Load World | [Q] Exit\n";
    frameBuffer += doubleRow + "\n";
    std::cout << frameBuffer;
}

void Engine::Run() {
    while (running) {
        if (gameState == GameState::Title) {
            char key = 0;
            if (read(STDIN_FILENO, &key, 1) > 0) {
                if (key == 'q' || key == 'Q') { running = false; }
                else { LoadLevel(1, 1); }
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                continue;
            }
            RenderTitle();
        } else if (gameState == GameState::Transition) {
            if (transitionTimer > 0) transitionTimer--;
            if (transitionTimer == 0) gameState = GameState::Playing;
            RenderTransition();
        } else if (gameState == GameState::GameOver) {
            if (gameOverTimer > 0) { gameOverTimer--; RenderGameOver(); }
            else {
                char key = 0;
                if (read(STDIN_FILENO, &key, 1) > 0) {
                    lives = 3; score = 0; coins = 0;
                    playerState = PlayerState::Small;
                    invincibilityFrames = 0; starTimer = 0;
                    entities.clear(); firebars.clear(); floatingTexts.clear();
                    nextSpawnIndex = 0;
                    gameState = GameState::Title;
                }
                RenderGameOver();
            }
        } else if (gameState == GameState::Win) {
            char key = 0;
            if (read(STDIN_FILENO, &key, 1) > 0) {
                lives = 3; score = 0; coins = 0;
                playerState = PlayerState::Small;
                invincibilityFrames = 0; starTimer = 0;
                entities.clear(); firebars.clear(); floatingTexts.clear();
                nextSpawnIndex = 0;
                gameState = GameState::Title;
            }
            RenderWin();
        } else {
            ProcessInput();
            UpdatePhysics();
            Render();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}
