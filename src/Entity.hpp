#pragma once

class Level;

enum class EntityType {
    Goomba,
    Koopa,
    Mushroom,
    FireFlower,
    Starman,
    OneUp,
    Fireball,
    PiranhaPlant,
    Bowser
};

class Entity {
public:
    Entity(EntityType type, float x, float y);
    virtual ~Entity() = default;

    virtual void Update(Level&) {}
    virtual char GetChar() const = 0;
    virtual void OnStomp();
    virtual int GetScoreValue() const;

    float GetLeft() const { return x; }
    float GetRight() const { return x + width; }
    float GetTop() const { return y; }
    float GetBottom() const { return y + height; }

    EntityType type;
    float x, y;
    float velX, velY;
    float width, height;
    bool alive;
};
