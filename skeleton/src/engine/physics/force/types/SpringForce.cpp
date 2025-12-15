#include "SpringForce.h"

#include <assert.h>

#include "PhysicalObject.h"
#include "RigidBody.h"

SpringForce::SpringForce(PhysicalObject* mainObject, PhysicalObject* otherObject, float k, float restingLength, float maxLength)
    : ForcePair(mainObject, otherObject)
    , _k(k)
    , _restingLength(restingLength)
    , _maxLength(maxLength)
{
}

SpringForce::~SpringForce()
{
}

void SpringForce::updateForce(double deltaTime)
{
    assert(_otherObject != nullptr && "No other object assigned at SpringForce!");

    physx::PxVec3 displacement = _otherObject->getPosition() - _mainObject->getPosition();
    float currentLength = displacement.magnitude();
    _direction = displacement.getNormalized();

    physx::PxVec3 relativeVelocity = _otherObject->getVelocity() - _mainObject->getVelocity();

    // Breaks if over max length
    // if (currentLength > _maxLength) {
    //     this->setDead();
    //     return;
    // }

    float reducedMass = (_mainObject->getMass() * _otherObject->getMass()) / (_mainObject->getMass() + _otherObject->getMass()); // Reduced mass formula: rm = (m1 * m2) / (m1 + m2)
    float criticalDamp = 2.0f * sqrtf(_k * reducedMass); // Critical damping: c = 2 * sqrt(k * m)
    float damping = criticalDamp * 0.5f; // Damping coefficient (50% of critical damping)

    float vAlongSpring = relativeVelocity.dot(_direction);
    physx::PxVec3 forceDamping = -damping * vAlongSpring * _direction;

    float extension = currentLength - _restingLength;
    _magnitude = _k * extension;
    _force = -(_magnitude * _direction);
    // std::cout << "Force: " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;
    // std::cout << "Damping Force: " << forceDamping.x << ", " << forceDamping.y << ", " << forceDamping.z << std::endl;
    _force += forceDamping;
    // std::cout << "Force with damping: " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;
}

physx::PxVec3 SpringForce::getForceOnRigidBody(RigidBody &rigidBody)
{
    return computeForceOnRigidBody(rigidBody);
}

physx::PxVec3 SpringForce::computeForceOnRigidBody(RigidBody &rigidBody)
{
    physx::PxVec3 force = physx::PxVec3(0);

    // std::cout << "Object: " << &rigidBody << std::endl;
    // std::cout << "MainObject: " << _mainObject << ", OtherObject: " << _otherObject << std::endl;

    if (&rigidBody == static_cast<RigidBody*>(_mainObject)) {
        // std::cout << "Applying spring force to main object." << std::endl;
        // std::cout << "Force computed: " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;
        // std::cout << "SpringForce::computeForceOnRigidBody: Applying force " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;
        force = -_force;
    } 
    else if (&rigidBody == static_cast<RigidBody*>(_otherObject)) {
        // std::cout << "Applying spring force to other object." << std::endl;
        // std::cout << "Force computed: " << -_force.x << ", " << -_force.y << ", " << -_force.z << std::endl;
        force = _force;
    }

    // std::cout << "SpringForce::computeForceOnRigidBody: Applying force " << force.x << ", " << force.y << ", " << force.z << std::endl;
    return force;
}
