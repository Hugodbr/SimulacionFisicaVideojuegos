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

	Particle() = delete;

	Particle(const physx::PxTransform &initTransform, 
		const physx::PxVec3 &initVelocity, 
		const physx::PxVec3 &initAcceleration, 
		double damping, 
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET, 
		float size = Constants::Particle::Default::Size
	);

	Particle(const physx::PxTransform &initTransform,
		const physx::PxVec3 &initVelocity, 
		const physx::PxVec3 &initAcceleration, 
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	Particle(const physx::PxTransform &initTransform,
		const physx::PxVec3 &initVelocity, 
		const physx::PxVec3 &initAcceleration, 
		float size,
		double damping,
		const physx::PxVec4 &color,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	Particle(
		float size,
		const physx::PxVec4 &color,
		float speed,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	// Copy constructor
	Particle(const Particle& other);

	virtual ~Particle(); 

	virtual void setTransform(const physx::PxTransform &origin);
	virtual void setOriginalTransform(const physx::PxTransform &transform);
	virtual void setTransformRelative(const physx::PxTransform &transform);
	virtual void setVelocity(const physx::PxVec3 &velocity);
	virtual void setSpeed(float speed);
	virtual void setVelocityDirection(const physx::PxVec3 &direction);
	virtual void setAge(double age);
	virtual void setAcceleration(const physx::PxVec3 &acceleration);
	virtual void setSize(double size);

	virtual double getAge() const { return _age; }
	virtual const physx::PxVec3& getPosition() const { return _transform.p; }
	virtual const physx::PxVec3& getVelocity() const { return _velocity; }
	virtual const physx::PxVec3& getAcceleration() const { return _acceleration; }
	virtual double getSize() const { return _size; }
	virtual float getSpeed() const { return _speed; }
	virtual const physx::PxTransform& getTransform() const { return _transform; }
	virtual const physx::PxTransform& getOriginalTransform() const { return _transformOriginal; }
	virtual const physx::PxTransform& getRelativeTransform() const { return _relativeTransform; }

	virtual bool isActive() const { return _alive; }
	uint64_t getId() const { return _id; }

	virtual void activate();
	virtual void deactivate();

	virtual void update(double dt);

	virtual void setColor(const physx::PxVec4& color);

	virtual bool isVisible() const { return _renderItem->visible; }
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
	float _speed;

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

	bool _initialized = false;

	bool _firstIntegration;
};
