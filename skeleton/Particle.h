#pragma once

#include "RenderUtils.hpp"
#include "Constants.h"


class RenderItem;

class Particle
{
public:

	Particle(const physx::PxTransform &initTransform, const physx::PxVec3 &initVelocity, const physx::PxVec3 &initAcceleration, double damping, Constants::Integration_Method integrationMethod, float size = Constants::Particle::Size);
	Particle(const physx::PxTransform &initTransform, const physx::PxVec3 &initVelocity, const physx::PxVec3 &initAcceleration, Constants::Integration_Method integrationMethod);

	virtual ~Particle();

	virtual void createRenderItem();

	virtual const physx::PxVec3& getVelocity() const;

	virtual void integrate(double dt);

protected:

	virtual void eulerIntegration(double dt);
	virtual void eulerSemiImplicitIntegration(double dt);
	virtual void verletIntegration(double dt);

protected:

	RenderItem* _renderItem;

	physx::PxVec3 _acceleration;

	physx::PxVec3 _velocity;
	physx::PxVec3 _velocityPrevious;

	double _damping = 0.989;

	physx::PxTransform _transform; // position and direction
	physx::PxTransform _transformPrevious; // position and direction


	Constants::Integration_Method _integrationMethod;

	physx::PxShape* _shape;
	physx::PxVec4 _color;

	double _size;

	double _lifeTime;

private:

	bool _firstIntegration;
};
