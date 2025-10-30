#pragma once

#include "ForceGenerator.h"

class ForceField : public ForceGenerator
{
public:
    ForceField();
    virtual ~ForceField() = default;

    virtual void updateForce(double deltaTime) override;
};