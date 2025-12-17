#include "PhysicsEngine.h"

#include <chrono>
#include <thread>

#include "ForceManager.h"
#include "ParticleSystem.h"
#include "RigidBodySystem.h"
#include "RigidBody.h"


using namespace physx;

PhysicsEngine::PhysicsEngine()
    : gContactReportCallback(this)
	, _interactive(true)
{
}

void PhysicsEngine::init()
{
    if (_initialized) {
        return; // already initialized
    }

	mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
    if(!mFoundation) {
        std::cerr << "PxCreateFoundation failed!" << std::endl;
        _initialized = false;
        return;
    }

	gPvd = PxCreatePvd(*mFoundation);
    mPvdTransport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	if (gPvd && mPvdTransport) {
		gPvd->connect(*mPvdTransport,PxPvdInstrumentationFlag::eALL);
	}

    // Custom createPhysics function to register additional PhysX modules if needed
	gPhysics = customCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(),true,gPvd);

	if (!gPhysics) {
		std::cerr << "PxCreateBasePhysics failed!" << std::endl;
		// cleanup partial init
		if (gPvd) { gPvd->release(); gPvd = nullptr; }
		if (mPvdTransport) { mPvdTransport->release(); mPvdTransport = nullptr; }
		if (mFoundation) { mFoundation->release(); mFoundation = nullptr; }
		_initialized = false;
		return;
	}

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Rigid Body
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;

    // Custom filter shader for contact reports. Implementation in callbacks.cpp
	sceneDesc.filterShader = contactReportFilterShader; 
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

    forceManager = &ForceManager::getInstance();
    
    std::cout << "Physics Engine initialized." << std::endl;
}

void PhysicsEngine::stepSimulation(double deltaTime)
{
    // forceManager->update(deltaTime);
    
    // // std::cout << "SIM STEP" << std::endl;
    // for (ParticleSystem* particleSystem : _particleSystems) {
    //     particleSystem->update(deltaTime);
    // }

    // for (RigidBodySystem* rigidBodySystem : _rigidBodySystems) {
    //     rigidBodySystem->update(deltaTime);
    // }

    // for (RigidBody* rigidBody : _rigidBodies) {
    //     rigidBody->update(deltaTime);
    // }

    // if (gScene) {
    //     gScene->simulate(deltaTime);
    //     gScene->fetchResults(true);
    // }

	// // std::this_thread::sleep_for(std::chrono::microseconds(10));

    const double h = deltaTime / (double)PHYSICS_SUBSTEPS;

    for (int i = 0; i < PHYSICS_SUBSTEPS; ++i) {

        forceManager->update(h);

        for (ParticleSystem* particleSystem : _particleSystems) {
            particleSystem->update(h);
        }

        for (RigidBodySystem* rigidBodySystem : _rigidBodySystems) {
            rigidBodySystem->update(h);
        }

        for (RigidBody* rigidBody : _rigidBodies) {
            rigidBody->update(h);
        }

        if (gScene) {
            gScene->simulate(h);
            gScene->fetchResults(true);
        }
    }
}

void PhysicsEngine::shutdown()
{
    reset();

    // Release in safe order and nulling pointers after release.
    if (gScene) { gScene->release(); gScene = nullptr; }

    if (gDispatcher) { gDispatcher->release(); gDispatcher = nullptr; }

    if (gMaterial) { gMaterial->release(); gMaterial = nullptr; }

    if (gPhysics) { gPhysics->release(); gPhysics = nullptr; }

    if (gPvd) { 
        gPvd->disconnect();
        gPvd->release();
        gPvd = nullptr;
    }

    if (mPvdTransport) { mPvdTransport->release(); mPvdTransport = nullptr; }

    if (mFoundation) { mFoundation->release(); mFoundation = nullptr; }

    _initialized = false;
}

void PhysicsEngine::reset()
{
    _particleSystems.clear();
}

physx::PxShape* PhysicsEngine::createShape(const physx::PxGeometry &geo, const physx::PxMaterial *mat)
{
	if (mat == nullptr)
		mat = gMaterial; // Default material

	physx::PxShape* shape = gPhysics->createShape(geo, *mat);
	return shape;
}

void PhysicsEngine::notifyContact(physx::PxActor *actor1, physx::PxActor *actor2)
{
    // Avoid compiler warnings for unused parameters
    PX_UNUSED(actor1);
    PX_UNUSED(actor2);

    if(onCollisionCallback) {
        onCollisionCallback(actor1, actor2);
    }
    else {
        std::cout << "Contact detected between two actors. No callback set." << std::endl;
    }
}

void PhysicsEngine::notifyConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
    PX_UNUSED(constraints);
    PX_UNUSED(count);

    if(onConstraintBreakCallback) {
        onConstraintBreakCallback(constraints, count);
    }
}

void PhysicsEngine::notifyWake(physx::PxActor** actors, physx::PxU32 count)
{
    PX_UNUSED(actors);
    PX_UNUSED(count);

    if(onWakeCallback) {
        onWakeCallback(actors, count);
    }
}

void PhysicsEngine::notifySleep(physx::PxActor** actors, physx::PxU32 count)
{
    PX_UNUSED(actors);
    PX_UNUSED(count);

    if(onSleepCallback) {
        onSleepCallback(actors, count);
    }
}

void PhysicsEngine::notifyTriggerEnter(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    if(onTriggerEnterCallback) {
        onTriggerEnterCallback(pairs, count);
    }
}

void PhysicsEngine::notifyTriggerExit(physx::PxTriggerPair *pairs, physx::PxU32 count)
{
    if(onTriggerExitCallback) {
        onTriggerExitCallback(pairs, count);
    }
}

void PhysicsEngine::notifyAdvance(const physx::PxRigidBody *const *bodies, const physx::PxTransform *poses, const physx::PxU32 count)
{
    PX_UNUSED(bodies);
    PX_UNUSED(poses);
    PX_UNUSED(count);

    if(onAdvanceCallback) {
        onAdvanceCallback(bodies, poses, count);
    }
}

physx::PxPhysics *PhysicsEngine::customCreatePhysics(physx::PxU32 version, physx::PxFoundation &foundation, const physx::PxTolerancesScale &scale, bool trackOutstandingAllocations, physx::PxPvd *pvd)
{
    physx::PxPhysics* physics = PxCreateBasePhysics(version, foundation, scale,
        trackOutstandingAllocations, pvd);

    if(!physics)
        return NULL;

    // Register optional PhysX modules here:

    // PxRegisterArticulations(*physics);
    // PxRegisterHeightFields(*physics);
    // PxRegisterCloth(*physics);
    // PxRegisterParticles(*physics);

    return physics;
}
