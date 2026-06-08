#include "Items.hpp"

static bool CollidesWithLevel(const Level& level, float x, float y) {
    char tile = level.GetTile(static_cast<int>(x), static_cast<int>(y));
    return (tile != Tile::Empty && tile != Tile::Flagpole && tile != Tile::Flag
         && tile != Tile::Coin);
}

Mushroom::Mushroom(float x, float y)
    : Entity(EntityType::Mushroom, x, y) {
    velX = 0.1f;
}

void Mushroom::Update(Level& level) {
    if (!alive) return;
    x += velX;
    int checkX = velX > 0 ? static_cast<int>(x + width) : static_cast<int>(x) - 1;
    int checkY = static_cast<int>(y);
    int footY = static_cast<int>(y + height);
    if (level.GetTile(checkX, checkY) != Tile::Empty || level.GetTile(checkX, footY + 1) == Tile::Empty) {
        velX = -velX;
    }
}

char Mushroom::GetChar() const {
    return 'M';
}

FireFlower::FireFlower(float x, float y)
    : Entity(EntityType::FireFlower, x, y) {}

void FireFlower::Update(Level&) {}

char FireFlower::GetChar() const {
    return 'F';
}

Starman::Starman(float x, float y)
    : Entity(EntityType::Starman, x, y) {
    velY = -0.2f;
}

void Starman::Update(Level&) {
    if (!alive) return;
    y += velY;
    if (y <= 8) velY = 0.2f;
    if (y >= 10) velY = -0.2f;
}

char Starman::GetChar() const {
    return 'S';
}

OneUp::OneUp(float x, float y)
    : Entity(EntityType::OneUp, x, y) {}

void OneUp::Update(Level&) {}

char OneUp::GetChar() const {
    return 'U';
}

Fireball::Fireball(float x, float y, float dir)
    : Entity(EntityType::Fireball, x, y), dir(dir), lifetime(120), bounceCount(0) {
    velX = dir * FIREBALL_SPEED;
    velY = -0.3f;
}

void Fireball::Update(Level& level) {
    if (!alive) return;

    lifetime--;
    if (lifetime <= 0) { alive = false; return; }

    velY += FIREBALL_GRAVITY;
    y += velY;

    if (y > GRID_HEIGHT + 2) { alive = false; return; }

    if (CollidesWithLevel(level, x, y)) {
        if (velY > 0) {
            y = static_cast<int>(y) - 0.01f;
            velY = FIREBALL_BOUNCE;
            bounceCount++;
            if (bounceCount > 3) alive = false;
        } else if (velY < 0) {
            y = static_cast<int>(y) + 1.01f;
            velY = 0;
        }
    }

    x += velX;
    if (CollidesWithLevel(level, x, y)) {
        alive = false;
    }
}

char Fireball::GetChar() const {
    return ((lifetime / 6) % 2 == 0) ? 'o' : '*';
}
