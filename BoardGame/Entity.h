#pragma once
#include "stdafx.h"

class Entity
{
public:
    virtual ~Entity() = default;
    virtual void render() const = 0;

    int getPosition() const { return Position; }
    void setPosition(int pos) { Position = pos; }
    virtual bool isCollidable() { return false; };

protected:
    int Position; 
};
