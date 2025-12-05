#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <PxPhysicsAPI.h>
#include "core.hpp"


class PhysicsEngine; // Forward declaration


// Custom filter shader function for contact reporting
// This function is called by PhysX to determine how to handle collisions between pairs of objects.
physx::PxFilterFlags contactReportFilterShader(
	physx::PxFilterObjectAttributes attributes0, // Object 0 type info (static/dynamic/kinematic/trigger)
	physx::PxFilterData filterData0,             // Custom user-defined filter data for object 0 (groups, masks, etc.)
	physx::PxFilterObjectAttributes attributes1, 
	physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags,               // Output flags. Tells Physx what kind of contact data to produce.
	const void* constantBlock,                   // Optional for advanced filtering
	physx::PxU32 constantBlockSize
);


/* ! Recommendation: see PxSimulationEventCallback for more details on contact reporting ! */

// A custom class inheriting from PxSimulationEventCallback to handle contact reports
// (rewrote to modern C++ conventions)
class ContactReportCallback 
	: public physx::PxSimulationEventCallback // Interface for receiving simulation events
{
private:
	PhysicsEngine* _engine = nullptr; // Pointer to the main physics engine to interact with the simulation

public:
	explicit ContactReportCallback(PhysicsEngine* engine) : _engine(engine) {}

	// PxSimulationEventCallback is an abstract interface that defines these virtual methods to be overridden:
	
	// During the simulation step (gScene->simulate(dt)), 
	// this function will be called when contact events occur between pairs of objects.
	// Entry point for PhysX for contact notifications.
	void onContact(const physx::PxContactPairHeader& pairHeader, 
		const physx::PxContactPair* pairs, 
		physx::PxU32 nbPairs
	);
	
	// Called when a breakable constraint breaks.
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
	
	// Called when actors wake up from sleep.
	void onWake(physx::PxActor** actors, physx::PxU32 count) override;
	
	// Called when actors go to sleep.
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
	
	// Called when trigger events occur (e.g., a trigger volume is entered).
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
	
	// Called during the advance phase of the simulation step. Provides early access to the new pose of moving rigid bodies.
	void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) override;
};

#endif // __CALLBACKS_H__