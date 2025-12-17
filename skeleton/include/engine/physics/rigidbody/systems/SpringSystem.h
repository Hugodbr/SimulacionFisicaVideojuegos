#pragma once

#include "RigidBodySystem.h"
#include "RigidBody.h"
#include "force/types/SpringForce.h"

class SpringSystem : public RigidBodySystem
{
public:
	SpringSystem(const physx::PxVec3 &originAnchor, float totalLength, int numRigidBodies, float bodyMass, float k, float maxStretch, const std::string& filePath, float scale = 1.0f);
	// SpringSystem(
    //     const physx::PxVec3 &beginning, RigidBody& beginningBody, const physx::PxVec3 &beginningAttachPoint,
    //     const physx::PxVec3 &end, RigidBody& endBody, const physx::PxVec3 &endAttachPoint,
    //     int numRigidBodies, float bodyMass, float k, float maxStretch, const std::string& filePath, float scale = 1.0f
    // );

	~SpringSystem();

	void init() override;
	void update(double deltaTime) override;

    virtual void attachObject(RigidBody* rb);
    virtual PhysicalObject* getEnd();

protected:
    // void initParticleGeneratorAndPool();
    void createRigidBodies(const std::string& filePath, float scale);

    virtual void updateInternalForces(double deltaTime);
	// Clear all forces from the particles and apply forces from ForceManager
	virtual void applyForces() override;

protected:

    physx::PxVec3 _originAnchor = physx::PxVec3(0.0f, 0.0f, 0.0f); // fixed body at the top
    float _segmentLength; // rest length of each spring segment
    float _maxStretch; // maximum stretch allowed
    float _maxSegmentLength; // maximum length of each segment
    float _k; // spring constant

    float _bodyMass; // mass of each rigid body
    //
    std::vector<physx::PxD6Joint*> _d6Joints;
    physx::PxD6Joint* _looseEndJoint = nullptr;
    //
    std::vector<std::unique_ptr<ForceGenerator>> _internalForceGenerators;
};
