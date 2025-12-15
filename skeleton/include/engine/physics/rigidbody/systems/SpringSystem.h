#pragma once

#include "RigidBodySystem.h"
#include "RigidBody.h"
#include "force/types/SpringForce.h"

class SpringSystem : public RigidBodySystem
{
private:
	// Region _region;
	
public:
	// SpringSystem(const physx::PxVec3 &originAnchor, float totalLength, int numRigidBodies, float bodyMass, float k, float maxStretch, std::unique_ptr<RigidBody> modelPrototype);
	SpringSystem(const physx::PxVec3 &originAnchor, float totalLength, int numRigidBodies, float bodyMass, float k, float maxStretch, const std::string& filePath, float scale = 1.0f);
    // TODO make a constructor that takes two points and number of rigid bodies. Can be used to create a bridge between two fixed points for example.
	~SpringSystem();

	void init() override;
	void update(double deltaTime) override;

    virtual void attachObject(RigidBody* rb);
	// Sets all active particles to renderable or not
	// void setRenderable(bool renderable) override;

	// // Returns the reserve count per generator for this system
	// virtual uint64_t getReserveCountPerGenerator() const override { 
	// 	return Constants::System::Rain::ReserveCountPerGenerator; 
	// }

protected:
    // void initParticleGeneratorAndPool();
    void createRigidBodies(const std::string& filePath, float scale);

	// void createForceGenerator(std::unique_ptr<ForceGenerator>& forceGen);
	void createForceGenerator(); // TODO should be in base class?
	
    virtual void updateInternalForces(double deltaTime);
	// Clear all forces from the particles and apply forces from ForceManager
	virtual void applyForces() override;

protected:
	// std::vector<
    //     std::pair<
    //         std::unique_ptr<UniformParticleGenerator>,
    //         std::unique_ptr<ParticlePool<RainParticle>>
    //         >
    //     > _generatorsAndPools;


    // std::vector<std::unique_ptr<RigidBody>> _rigidbodies;
    //
    // std::unique_ptr<RigidBody> _modelPrototype;
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
