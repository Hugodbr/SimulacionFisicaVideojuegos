#pragma once

#include <memory>
#include <iostream>

#include "RenderUtils.hpp"

#include "Constants.h"


class RenderItem;


class Particle
{
private:
	static uint64_t _nextId; // Static counter for generating unique IDs
protected:
    uint64_t _id; // Protected to allow derived classes access

public:

	Particle();

	Particle(const physx::PxTransform &initTransform, 
		const physx::PxVec3 &initVelocity, 
		const physx::PxVec3 &initAcceleration, 
		double damping, 
		Constants::Integration_Method integrationMethod, 
		float size = Constants::Particle::Size
	);

	Particle(const physx::PxTransform &initTransform,
		const physx::PxVec3 &initVelocity, 
		const physx::PxVec3 &initAcceleration, 
		Constants::Integration_Method integrationMethod
	);

	Particle(const physx::PxTransform &initTransform,
		const physx::PxVec3 &initVelocity, 
		const physx::PxVec3 &initAcceleration, 
		Constants::Integration_Method integrationMethod,
		float size,
		double damping,
		const physx::PxVec4 &color
	);

	// Copy constructor
	Particle(const Particle& other);

	virtual ~Particle(); 

	virtual void setTransform(const physx::PxTransform &origin);
	virtual void setOriginalTransform(const physx::PxTransform &transform);
	virtual void setTransformRelative(const physx::PxTransform &transform);
	virtual void setVelocity(const physx::PxVec3 &velocity);
	virtual void setAge(double age);
	virtual void setAcceleration(const physx::PxVec3 &acceleration);
	virtual void setSize(double size);

	virtual double getAge() const { return _age; }
	virtual physx::PxVec3 getPosition() const { return _transform.p; }
	virtual physx::PxVec3 getVelocity() const { return _velocity; }
	virtual physx::PxTransform getTransform() const { return _transform; }
	virtual physx::PxTransform getOriginalTransform() const { return _transformOriginal; }
	virtual physx::PxTransform getRelativeTransform() const { return _relativeTransform; }

	virtual bool isActive() const { return _alive; }
	uint64_t getId() const { return _id; }

	virtual void activate();
	virtual void deactivate();

	virtual void update(double dt);

	virtual void setColor(const physx::PxVec4& color);
	virtual void setVisibility(bool visibility);


protected:

	virtual void init();

	virtual void createRenderItem();

	virtual void integrate(double dt);

	virtual void updateAge(double dt);

	// Integration methods
	virtual void eulerIntegration(double dt);
	virtual void eulerSemiImplicitIntegration(double dt);
	virtual void verletIntegration(double dt);


protected:

	RenderItem* _renderItem;

	physx::PxVec3 _acceleration;

	physx::PxVec3 _velocity;
	physx::PxVec3 _velocityPrevious;

	double _damping;

	physx::PxTransform _transform; // position and direction
	physx::PxTransform _transformPrevious; // position and direction
	physx::PxTransform _transformOriginal; // position and direction
	physx::PxTransform _relativeTransform; // position and direction

	Constants::Integration_Method _integrationMethod;

	physx::PxShape* _shape;
	physx::PxVec4 _color;

	double _size;

	double _age;
	bool _alive;

private:

	bool _firstIntegration;
};
