#pragma once

#include <memory>
#include <iostream>

#include <PxPhysicsAPI.h>

#include "EntityRenderable.h"
#include "Constants.h"
#include "PhysicalObject.h"


class Particle : public PhysicalObject
{
private:
	static uint64_t _nextId; // Static counter for generating unique IDs
protected:
    uint64_t _id; // Protected to allow derived classes access
public:

	Particle() = delete;

    Particle::Particle(
        const physx::PxTransform &initTransform,
        const physx::PxVec3 &initVelocity,
        double damping,
        Constants::Integration_Method integrationMethod,
        float size
    );

	Particle(
		float size,
		float speed,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	Particle(
		float size,
		const physx::PxVec4& color,
		float speed,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	// Copy constructor
	Particle(const Particle& other);

	virtual ~Particle(); 


protected:

	virtual void onInit() override;

};