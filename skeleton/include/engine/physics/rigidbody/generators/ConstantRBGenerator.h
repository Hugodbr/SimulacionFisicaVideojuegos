#pragma once

#include "RigidBodyGenerator.h"


class ConstantRigidBodyGenerator : public RigidBodyGenerator
{
public:
    ConstantRigidBodyGenerator() = default;
    ~ConstantRigidBodyGenerator() = default;

    double getDistribution() const override {
        return 0.0;
    }
    
};