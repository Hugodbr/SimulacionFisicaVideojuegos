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

	virtual std::unique_ptr<Particle> clone() const;

	virtual void setOrigin(const physx::PxTransform &origin);
	virtual void setVelocity(const physx::PxVec3 &velocity);
	virtual void setAge(double age);

	virtual double getAge() const;
	virtual physx::PxVec3 getPosition() const;
	virtual physx::PxVec3 getVelocity() const;
	//virtual bool isAlive() const;

	//virtual void kill();

	virtual void update(double dt);


protected:

	virtual void createRenderItem();

	virtual void integrate(double dt);
	virtual void updateAge(double dt);

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

	double _age;
	bool _alive;

private:

	bool _firstIntegration;
};
