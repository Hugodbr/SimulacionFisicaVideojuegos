#pragma once

#include <list>
#include <random>

#include <PxPhysicsAPI.h>

#include "ObjectGenerator.h"


// Abstract class
// The Generator handles spawn and death rules of rigid bodies based on probabilistics
// Derived classes implement their own probability distribution
class RigidBodyGenerator : public ObjectGenerator
{
public:
	RigidBodyGenerator() {};
	virtual ~RigidBodyGenerator() = default;
};
