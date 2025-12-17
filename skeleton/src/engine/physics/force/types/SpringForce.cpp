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
    _maxForce = (_maxLength - _restingLength) * _k; // To prevent extreme forces
}

SpringForce::~SpringForce()
{
}

void SpringForce::updateForce(double deltaTime)
{
    assert(_otherObject.first != nullptr && "No other object assigned at SpringForce!");

    physx::PxVec3 displacement = _otherObject.first->getPosition() - _mainObject.first->getPosition();
    float currentLength = displacement.magnitude();

    if (currentLength < Constants::Math::epsilon) {
        _force = physx::PxVec3(0);
        _magnitude = 0.0f;
        return;
    }
    _direction = displacement / currentLength; // Normalize

    physx::PxVec3 relativeVelocity = _otherObject.first->getVelocity() - _mainObject.first->getVelocity();

    // Breaks if over max length
    // if (currentLength > _maxLength) {
    //     this->setDead();
    //     return;
    // }

    float reducedMass = (_mainObject.first->getMass() * _otherObject.first->getMass()) / (_mainObject.first->getMass() + _otherObject.first->getMass()); // Reduced mass formula: rm = (m1 * m2) / (m1 + m2)
    float criticalDamp = 2.0f * sqrtf(_k * reducedMass); // Critical damping: c = 2 * sqrt(k * m)
    float damping = criticalDamp * 0.5f; // Damping coefficient (50% of critical damping)

    float vAlongSpring = relativeVelocity.dot(_direction);
    physx::PxVec3 forceDamping = -damping * vAlongSpring * _direction;

    float extension = currentLength - _restingLength;
    // std::cout << "Current Length: " << currentLength << ", Resting Length: " << _restingLength << ", Extension: " << extension << std::endl;
    extension = physx::PxClamp(extension, -_maxLength, _maxLength);

    if (_tensionOnly && extension <= 0.0f) {
        // Compression - no force applied
        _force = physx::PxVec3(0);
        _magnitude = 0.0f;
        return;
    }

    _magnitude = _k * extension;
    _force = -(_magnitude * _direction);
    // std::cout << "Force: " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;
    // std::cout << "Damping Force: " << forceDamping.x << ", " << forceDamping.y << ", " << forceDamping.z << std::endl;
    _force += forceDamping;

    // std::cout << "Force with damping: " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;

    if (_force.magnitude() > _maxForce) {
        _force = _force.getNormalized() * _maxForce;
    }
    
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

    if (_mainObject.second && &rigidBody == static_cast<RigidBody*>(_mainObject.first)) {
        // std::cout << "Applying spring force to main object." << std::endl;
        // std::cout << "Force computed: " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;
        // std::cout << "SpringForce::computeForceOnRigidBody: Applying force " << _force.x << ", " << _force.y << ", " << _force.z << std::endl;
        force = -_force;
    } 
    else if (_otherObject.second && &rigidBody == static_cast<RigidBody*>(_otherObject.first)) {
        // std::cout << "Applying spring force to other object." << std::endl;
        // std::cout << "Force computed: " << -_force.x << ", " << -_force.y << ", " << -_force.z << std::endl;
        force = _force;
    }

    // std::cout << "SpringForce::computeForceOnRigidBody: Applying force " << force.x << ", " << force.y << ", " << force.z << std::endl;
    return force;
}
