#include "game_object.h"

// Construtor
GameObject::GameObject(int startX, int startY, int objWidth, int objHeight, int moveSpeed)
    : x(startX), y(startY), width(objWidth), height(objHeight), speed(moveSpeed) {
    /
}

int GameObject::getX() const {
    
    return x;
}

int GameObject::getY() const {
    return y;
}

int GameObject::getWidth() const {
    return width;
}

int GameObject::getHeight() const {
    return height;
}