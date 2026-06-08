#pragma once
#include "Entity.hpp"
#include "Constants.hpp"
#include "Level.hpp"

class Mushroom : public Entity {
public:
    Mushroom(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
};

class FireFlower : public Entity {
public:
    FireFlower(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
};

class Starman : public Entity {
public:
    Starman(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
};

class OneUp : public Entity {
public:
    OneUp(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
};

class Fireball : public Entity {
public:
    Fireball(float x, float y, float dir);
    void Update(Level& level) override;
    char GetChar() const override;

    float dir;
    int lifetime;
    int bounceCount;
};
