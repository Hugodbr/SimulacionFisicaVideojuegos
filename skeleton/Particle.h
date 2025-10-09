#pragma once

#include "RenderUtils.hpp"
#include "Constants.h"



class RenderItem;

class Particle
{
public:
	Particle(const physx::PxTransform &pose, const  physx::PxVec3 &velocity, const physx::PxVec3 &acceleration, double damping, Constants::Integration_Method integrateMethod);
	// Constructor with mass
	Particle(const physx::PxTransform &pose, const  physx::PxVec3 &velocity, const physx::PxVec3 &acceleration, double mass, double damping, Constants::Integration_Method integrateMethod);
	// Constructor with mass and gravity
	Particle(const physx::PxTransform &pose, const  physx::PxVec3 &velocity, const physx::PxVec3 &acceleration, const physx::PxVec3 &gravity, double mass, double damping, Constants::Integration_Method integrateMethod);

	virtual ~Particle();

	virtual void createRenderItem();

	virtual const physx::PxVec3& getVelocity() const;

	virtual void integrate(double dt);

protected:
	virtual void eulerIntegration(double dt);
	virtual void eulerSemiImplicitIntegration(double dt);
	virtual void verletIntegration(double dt);

	virtual void setSimulatedMass();
	virtual void setSimulatedVelocity();
	virtual void setSimulatedGravity();
	virtual void setSimulatedAcceleration();

protected:
	RenderItem* _renderItem;

	physx::PxVec3 _acc;

	physx::PxVec3 _gravity;
	physx::PxVec3 _gravityReal;

	physx::PxVec3 _vel;
	physx::PxVec3 _velPrevious;
	physx::PxVec3 _velReal;

	double _damping = 0.989;

	physx::PxTransform _pose; // position and direction
	physx::PxTransform _posePrevious; // position and direction


	Constants::Integration_Method _integrationMethod;

	physx::PxShape* _shape;
	physx::PxVec4 _color;

	double _size;

	double _mass;
	double _massReal;

	double _lifeTime;

private:
	bool _firstIntegration;
};
