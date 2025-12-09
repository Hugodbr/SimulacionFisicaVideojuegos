#pragma once

#include <PxPhysicsAPI.h>

#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "callbacks.hpp"

class ForceManager;
class ParticleSystem;
class RigidBody;

class PhysicsEngine {

private:
    // Whether the physics engine is running in interactive mode (with rendering) or not.
    bool _interactive;

    // Lifecycle tracking
    bool _initialized = false;
    bool _shutdown = false;

    // 'Foundation' is the base for all PhysX SDK objects.
    // Handles memory allocation and error reporting.
    // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Startup.html#introduction
    physx::PxFoundation*           mFoundation = nullptr;
    // Memory allocator for foundation that PhysX uses internally.
    physx::PxDefaultAllocator      gAllocator;
    // Handles warning and error messages from PhysX.
    physx::PxDefaultErrorCallback gErrorCallback;

    // PxPhysics: The main physics SDK object.
    // Provides all functions to create physical objects, scenes, materials, etc.
    physx::PxPhysics*              gPhysics    = nullptr;
    // PxMaterial: Define how surfaces interact (static/dynamic friction, restitution)
    physx::PxMaterial*             gMaterial   = nullptr;

    // PhysX Visual Debugger. Is an optional tool that lets you visualize physics data in real-time.
    physx::PxPvd*                   gPvd        = nullptr;

    physx::PxPvdTransport*          mPvdTransport = nullptr;

    // PxCpuDispatcher: Dispatches tasks to CPU worker threads.
    // Run physics tasks in parallel on multiple threads.
    physx::PxDefaultCpuDispatcher*  gDispatcher = nullptr;
    // PxScene: Container for all physical objects and simulation => "The World"
    physx::PxScene*                 gScene      = nullptr;

    // PxSimulationEventCallback: Custom callback class to handle simulation events.
    // Two actors colliding, trigger events, constraint breaks, etc.
    ContactReportCallback           gContactReportCallback;

    // User-defined collision callback function
    std::function<void(physx::PxActor*, physx::PxActor*)>                                               onCollisionCallback;
    std::function<void(physx::PxConstraintInfo* constraints, physx::PxU32 count)>                       onConstraintBreakCallback;
    std::function<void(physx::PxActor** actors, physx::PxU32 count)>                                    onWakeCallback;
    std::function<void(physx::PxActor** actors, physx::PxU32 count)>                                    onSleepCallback;
    std::function<void(physx::PxTriggerPair* pairs, physx::PxU32 count)>                                onTriggerEnterCallback;
    std::function<void(physx::PxTriggerPair* pairs, physx::PxU32 count)>                                onTriggerExitCallback;
    std::function<void(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32)> onAdvanceCallback;



public:

    static PhysicsEngine& getInstance()
    {
        static PhysicsEngine instance;
        return instance;
    }

    void init();
    void stepSimulation(double deltaTime);
    void shutdown();

    // Reset the physics engine state, clearing all particle systems.
    void reset();

    physx::PxPhysics* getPhysics() const { return gPhysics; }
    physx::PxScene* getScene() const { return gScene; }
    physx::PxMaterial* getMaterial() const { return gMaterial; }

    physx::PxShape* createShape(const physx::PxGeometry& geo, const physx::PxMaterial* mat);
    // void pushParticle(class Particle* particle) { _particles.push_back(particle); }
    void pushParticleSystem(ParticleSystem* particleSystem) { _particleSystems.push_back(particleSystem); }
    void pushRigidBody(RigidBody* rigidBody) { _rigidBodies.push_back(rigidBody); }

    // Notification methods called by ContactReportCallback
    void notifyContact(physx::PxActor* actor1, physx::PxActor* actor2);
    void notifyConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count);
    void notifyWake(physx::PxActor** actors, physx::PxU32 count);
    void notifySleep(physx::PxActor** actors, physx::PxU32 count);
    void notifyTriggerEnter(physx::PxTriggerPair* pairs, physx::PxU32 count);
    void notifyTriggerExit(physx::PxTriggerPair* pairs, physx::PxU32 count);
    void notifyAdvance(const physx::PxRigidBody*const* bodies, const physx::PxTransform* poses, const physx::PxU32 count);

    // Setters for user-defined callbacks
    // Called by game to register custom behavior for each event.
    void setOnCollisionCallback(std::function<void(physx::PxActor*, physx::PxActor*)> callback) { onCollisionCallback = callback; }
    void setOnConstraintBreakCallback(std::function<void(physx::PxConstraintInfo*, physx::PxU32)> callback) { onConstraintBreakCallback = callback; }
    void setOnWakeCallback(std::function<void(physx::PxActor**, physx::PxU32)> callback) { onWakeCallback = callback; }
    void setOnSleepCallback(std::function<void(physx::PxActor**, physx::PxU32)> callback) { onSleepCallback = callback; }
    void setOnTriggerEnterCallback(std::function<void(physx::PxTriggerPair*, physx::PxU32)> callback) { onTriggerEnterCallback = callback; }
    void setOnTriggerExitCallback(std::function<void(physx::PxTriggerPair*, physx::PxU32)> callback) { onTriggerExitCallback = callback; }
    void setOnAdvanceCallback(std::function<void(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32)> callback) { onAdvanceCallback = callback; }

private:

    PhysicsEngine();
    ~PhysicsEngine() = default;

    PhysicsEngine(const PhysicsEngine&) = delete;
    PhysicsEngine& operator=(const PhysicsEngine&) = delete;

    // Custom PhysX Physics Creation Function.
    // For setting optional SDK components.
    // Current optional features: Articulations, Height Field, Cloth, Particles.
    physx::PxPhysics* customCreatePhysics(
        physx::PxU32                       version,
        physx::PxFoundation&               foundation,
        const physx::PxTolerancesScale&    scale,
        bool                               trackOutstandingAllocations,
        physx::PxPvd*                      pvd
    );

private:
    std::vector<ParticleSystem*> _particleSystems = {};
    std::vector<RigidBody*> _rigidBodies = {};

    ForceManager* forceManager;
};