#include "Enemy.hpp"
#include <cstdlib>

Goomba::Goomba(float x, float y)
    : Entity(EntityType::Goomba, x, y) {
    velX = ENEMY_SPEED;
}

void Goomba::Update(Level& level) {
    if (!alive) return;
    x += velX;
    int checkX = velX > 0 ? static_cast<int>(x + width) : static_cast<int>(x) - 1;
    int checkY = static_cast<int>(y);
    int footY = static_cast<int>(y + height);
    char tileAhead = level.GetTile(checkX, checkY);
    char groundAhead = level.GetTile(checkX, footY + 1);
    if (tileAhead != Tile::Empty || groundAhead == Tile::Empty) velX = -velX;
    if (y > 15) alive = false;
}

char Goomba::GetChar() const { return 'O'; }

Koopa::Koopa(float x, float y)
    : Entity(EntityType::Koopa, x, y), state(State::Walking), shellTimer(0) {
    velX = ENEMY_SPEED;
}

void Koopa::Update(Level& level) {
    if (!alive) return;
    switch (state) {
        case State::Walking:
        case State::ShellSliding: {
            x += velX;
            int checkX = velX > 0 ? static_cast<int>(x + width) : static_cast<int>(x) - 1;
            int checkY = static_cast<int>(y);
            int footY = static_cast<int>(y + height);
            if (level.GetTile(checkX, checkY) != Tile::Empty ||
                level.GetTile(checkX, footY + 1) == Tile::Empty) velX = -velX;
            break;
        }
        case State::ShellStationary: break;
    }
    if (y > 15) alive = false;
}

char Koopa::GetChar() const {
    switch (state) {
        case State::Walking: return 'K';
        case State::ShellStationary: return '@';
        case State::ShellSliding: return '@';
    }
    return 'K';
}

void Koopa::OnStomp() {
    if (state == State::Walking) { state = State::ShellStationary; velX = 0; }
    else if (state == State::ShellStationary) { alive = false; }
}

int Koopa::GetScoreValue() const { return state == State::Walking ? 100 : 0; }

PiranhaPlant::PiranhaPlant(float x, float y)
    : Entity(EntityType::PiranhaPlant, x, y), timer(0), isUp(false), emergeDuration(80), hideDuration(120) {
    velX = 0; velY = 0;
}

void PiranhaPlant::Update(Level&) {
    if (!alive) return;

    if (!isUp) {
        timer++;
        if (timer >= hideDuration) {
            isUp = true;
            timer = 0;
        }
    } else {
        timer++;
        if (timer >= emergeDuration) {
            isUp = false;
            timer = 0;
        }
    }
}

char PiranhaPlant::GetChar() const {
    return isUp ? 'P' : ' ';
}

Bowser::Bowser(float x, float y)
    : Entity(EntityType::Bowser, x, y), health(5), jumpTimer(0), jumping(false) {
    velX = ENEMY_SPEED * 0.5f;
    width = 2.0f;
    height = 2.0f;
}

void Bowser::Update(Level& level) {
    if (!alive) return;

    velY += GRAVITY;
    y += velY;

    int footY = static_cast<int>(y + height);
    bool onGround = level.GetTile(static_cast<int>(x + width / 2), footY + 1) != Tile::Empty;

    if (onGround) {
        y = footY - height + 0.01f;
        velY = 0;
        jumping = false;

        jumpTimer++;
        if (jumpTimer > 40) {
            velY = -1.0f;
            jumping = true;
            jumpTimer = 0;
        }

        x += velX;
        int checkX = velX > 0 ? static_cast<int>(x + width) : static_cast<int>(x) - 1;
        int checkY = static_cast<int>(y);
        footY = static_cast<int>(y + height);
        if (level.GetTile(checkX, checkY) != Tile::Empty ||
            level.GetTile(checkX, footY + 1) == Tile::Empty) velX = -velX;
    } else if (y > 15) {
        alive = false;
    }
}

char Bowser::GetChar() const {
    return (health > 0) ? 'B' : ' ';
}

void Bowser::OnStomp() {}
