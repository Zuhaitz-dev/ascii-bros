#pragma once
#include "Entity.hpp"
#include "Constants.hpp"
#include "Level.hpp"
#include <vector>
#include <cmath>

class Goomba : public Entity {
public:
    Goomba(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
    int GetScoreValue() const override { return 100; }
};

class Koopa : public Entity {
public:
    enum class State { Walking, ShellStationary, ShellSliding };
    Koopa(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
    void OnStomp() override;
    int GetScoreValue() const override;
    State state;
    int shellTimer;
};

class PiranhaPlant : public Entity {
public:
    PiranhaPlant(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
    void OnStomp() override {}
    int GetScoreValue() const override { return 200; }
private:
    int timer;
    bool isUp;
    int emergeDuration;
    int hideDuration;
};

class Bowser : public Entity {
public:
    Bowser(float x, float y);
    void Update(Level& level) override;
    char GetChar() const override;
    void OnStomp() override;
    int GetScoreValue() const override { return 5000; }
    int health;
private:
    int jumpTimer;
    bool jumping;
};
