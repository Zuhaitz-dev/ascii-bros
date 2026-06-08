#include "Entity.hpp"

Entity::Entity(EntityType t, float x_, float y_)
    : type(t), x(x_), y(y_), velX(0), velY(0), width(1.0f), height(1.0f), alive(true) {}

void Entity::OnStomp() {
    alive = false;
}

int Entity::GetScoreValue() const {
    return 0;
}
