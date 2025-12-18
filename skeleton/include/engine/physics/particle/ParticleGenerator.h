#pragma once

#include <list>
#include <random>

#include <PxPhysicsAPI.h>

#include "ObjectGenerator.h"


// Abstract class
// The Generator handles spawn and death rules of particles based on probabilistics
// Derived classes implement their own probability distribution
class ParticleGenerator : public ObjectGenerator
{
public:
	ParticleGenerator() {};
	virtual ~ParticleGenerator() = default;
};
