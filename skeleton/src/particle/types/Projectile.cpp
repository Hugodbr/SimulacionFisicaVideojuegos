#include "Projectile.h"

#include "ForceGenerator.h"
#include "GravitationalForce.h"


Projectile::Projectile(        
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
    : ParticleWithMass(
        initTransform,
        realVelocity,
        initAcceleration,
        realMass,
        size,
        integrationMethod,
        damping
    )
{
	_massReal     = realMass;
	_velocityReal = realVelocity;

	_gravityFactor  = gravityFactor;
	_velocityFactor = velocityFactor;

	setSimulatedVelocity();
	// setSimulatedGravity();
	// setSimulatedAcceleration();
	setSimulatedMass();
}

void Projectile::setSimulatedVelocity()
{
	_velocity = _velocityReal * _velocityFactor;
	_speed = _velocity.magnitude();
}

// Calculate simulated mass based on kyinetic energy equivalence
void Projectile::setSimulatedMass()
{
	if (_velocity.magnitude() < Constants::Math::epsilon) {
		_mass = _massReal;
	} else {
		_mass = _massReal * pow((_velocityReal.magnitude() / _velocity.magnitude()), 2);
	}
	_inverseMass = (_mass != 0.0) ? 1.0 / _mass : 0.0;
}

void Projectile::applyForce(ForceGenerator &forceGenerator)
{
    // Special case for gravity forces
    if (dynamic_cast<GravitationalForce*>(&forceGenerator) != nullptr) {
        _resultingForce += forceGenerator.getForceOnParticle(*this) * _gravityFactor;
    } 
    else { // Default behavior
        ParticleWithMass::applyForce(forceGenerator);
    }

}

void Projectile::setVelocity(const physx::PxVec3 &realVelocity)
{
	_velocityReal = realVelocity;
	setSimulatedVelocity();
}

void Projectile::changeMass(float newMass)
{
    _massReal = newMass;
	setSimulatedMass();
}

// void Projectile::setSimulatedGravity()
// {
// 	_gravity = _gravityReal * _gravityFactor;
// }

// void Projectile::setSimulatedAcceleration()
// {
// 	_acceleration = _acceleration + _gravity;
// }