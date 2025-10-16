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

	virtual Particle* clone() const;

	virtual void setOrigin(const physx::PxTransform &origin);
	virtual void setVelocity(const physx::PxVec3 &velocity);

	virtual void setLifeTime(double time);

	virtual physx::PxVec3 getVelocity() const;
	virtual bool isAlive() const;

	virtual void update(double dt);


protected:

	virtual void createRenderItem();

	virtual void integrate(double dt);
	virtual void updateLife(double dt);

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


	Constants::Integration_Method _integrationMethod;

	physx::PxShape* _shape;
	physx::PxVec4 _color;

	double _size;

	double _lifeTime;
	bool _alive;

private:

	bool _firstIntegration;
};
