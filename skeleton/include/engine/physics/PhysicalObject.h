#pragma once

#include <memory>
#include <iostream>

#include "Constants.h"

class Abs_Entity;

// Abstract class for physical objects in the simulation (particles and rigidbodies)
class PhysicalObject
{
public:

	PhysicalObject() = delete;

	PhysicalObject(const physx::PxTransform &initTransform, 
		const physx::PxVec3 &initVelocity,
		double damping, 
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET, 
		float size = Constants::Particle::Default::Size
	);

	PhysicalObject(const physx::PxTransform &initTransform,
		const physx::PxVec3 &initVelocity,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	PhysicalObject(const physx::PxTransform &initTransform,
		const physx::PxVec3 &initVelocity,
		float size,
		double damping,
		const physx::PxVec4 &color,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	PhysicalObject(
		float size,
		const physx::PxVec4 &color,
		float speed,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	// Copy constructor
	PhysicalObject(const PhysicalObject& other);

	virtual ~PhysicalObject() = 0; 

	virtual void init();

	// ! Sets the renderable entity associated with this physical object
	// virtual void setRenderableEntity(std::unique_ptr<Abs_Entity> renderable);

	virtual void setTransform(const physx::PxTransform &origin);
	virtual void setOriginalTransform(const physx::PxTransform &transform);
	virtual void setTransformRelative(const physx::PxTransform &transform);
	virtual void setVelocity(const physx::PxVec3 &velocity);
	virtual void setSpeed(float speed);
	virtual void setVelocityDirection(const physx::PxVec3 &direction);
	virtual void setAge(double age);
	virtual void setAcceleration(const physx::PxVec3 &acceleration);
	virtual void setSize(double size);

	// ! Returns the renderable entity associated with this physical object
	// Abs_Entity* getRenderable() const { return _renderable.get(); }
	
	// TODO OVERRIDE IN RIDIG BODY TO GET FROM POSE PHYSX
	virtual double getAge() const { return _age; }
	virtual const physx::PxVec3& getPosition() const { return _transform.p; }
	virtual const physx::PxVec3& getVelocity() const { return _velocity; }
	virtual const physx::PxVec3& getAcceleration() const { return _acceleration; }
	virtual double getSize() const { return _size; }
	virtual float getSpeed() const { return _speed; }
	virtual const physx::PxTransform& getTransform() const { return _transform; }
	virtual const physx::PxTransform& getOriginalTransform() const { return _transformOriginal; }
	virtual const physx::PxTransform& getRelativeTransform() const { return _relativeTransform; }
	virtual float getMass();

	virtual bool isActive() const { return _alive; }

	virtual void activate();
	virtual void deactivate();

	virtual void update(double dt);

	virtual void setColor(const physx::PxVec4& color);

	virtual bool isVisible() const;
	virtual void setVisibility(bool visibility);


protected:
	virtual void onInit() {}; // Hook for derived classes to implement custom initialization

	virtual void integrate(double dt);
	// virtual void updateRenderableEntity();
	virtual void updateAge(double dt);

	// Integration methods
	virtual void eulerIntegration(double dt);
	virtual void eulerSemiImplicitIntegration(double dt);
	virtual void verletIntegration(double dt);


protected:
	// std::unique_ptr<Abs_Entity> _renderable;
	bool _isVisible = true;

	physx::PxVec3 _acceleration = physx::PxVec3(0.0f, 0.0f, 0.0f);

	physx::PxVec3 _velocity;
	physx::PxVec3 _velocityPrevious;
	float _speed;

	double _damping;

	physx::PxTransform _transform; // position and direction
	physx::PxTransform _transformPrevious; // position and direction
	physx::PxTransform _transformOriginal; // position and direction
	physx::PxTransform _relativeTransform; // position and direction

	Constants::Integration_Method _integrationMethod;

	// physx::PxShape* _shape;
	physx::PxVec4 _color;

	double _size;

	double _age;
	bool _alive;

	bool _initialized = false;

	bool _firstIntegration;
};
