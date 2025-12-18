#pragma once

#include "ObjectGenerator.h"

class ConstantGenerator : public ObjectGenerator
{
public:
    ConstantGenerator() = default;
    ~ConstantGenerator() = default;

    double getDistribution() const override {
        return 0.0;
    }
};