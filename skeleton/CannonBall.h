#pragma once
#include "Particle.h"

class CannonBall : public Particle
{
public:
	CannonBall(const physx::PxTransform& pose, const  physx::PxVec3& realVelocity, const physx::PxVec3& acceleration, const physx::PxVec3& realGravity, double realMass, double damping, Constants::Integration_Method integrateMethod);

	~CannonBall();

protected:

	void setSimulatedMass() override;
	void setSimulatedVelocity() override;
	void setSimulatedGravity() override;
	void setSimulatedAcceleration() override;

	double _gFactor = 0.5;
	double _vFactor = 0.5;
};

