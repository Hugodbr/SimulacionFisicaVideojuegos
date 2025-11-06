#include "CannonBall.h"

CannonBall::CannonBall(
	const physx::PxTransform& initTransform, 
	const physx::PxVec3&      realVelocity,
	const physx::PxVec3&      initAcceleration,
	float realMass,
	float size,
	const physx::PxVec4& color,
	float damping,
	float velocityFactor,
	float gravityFactor,
	Constants::Integration_Method integrationMethod
)
	: Projectile(
		initTransform,
		realVelocity,
		initAcceleration,
		realMass,
		size,
		color,
		damping,
		velocityFactor,
		gravityFactor,
		integrationMethod
	)
{ }