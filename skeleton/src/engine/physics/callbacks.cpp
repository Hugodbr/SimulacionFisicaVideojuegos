#include "callbacks.hpp"

#include "PhysicsEngine.h"

// =========================================================================================
// contactReportFilterShader Function Implementation
// =========================================================================================

physx::PxFilterFlags contactReportFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	// PX_UNUSED macro to avoid compiler warnings for unused parameters:
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	/* Description of possible pairFlags settings:
	=========== eSOLVE_CONTACT =====================		 
	Description: Generate physical collision response.
	// Apply normal physics response (bounce, friction, etc.) 
	-> without this flag, objects will pass through each other.

	=========== eDETECT_DISCRETE_CONTACT ============
	Description: Detect discrete collisions (once per simulation step)
	-> makes PhysX evaluate contact points per tick.

	=========== eNOTIFY_TOUCH_FOUND =================
	Description: Notify when a contact is first detected between two objects.
	-> calls onContact() - useful for one-time events like playing a sound effect.

	=========== eNOTIFY_TOUCH_PERSISTS ==============
	Description: Call onContact() for each simulation step while the contact persists
	-> useful for continuous effects like applying damage over time.

	=========== eNOTIFY_CONTACT_POINTS ==============
	Description: Call onContact() for each contact point with per-point data.
	-> provides detailed contact information (position, normal, impulse, etc.).
	*/

	// all initial and persisting reports for everything, with per-point data
	// pairFlags is set to all options bellow using bitwise OR operator '|'
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT     
		| physx::PxPairFlag::eDETECT_DISCRETE_CONTACT 
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND      // Call onContact when a contact first occurs
		| physx::PxPairFlag::eNOTIFY_TOUCH_LOST	      // Call onContact when a contact ends
		// | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS   // Call onContact for each simulation step while the contact persists // ! c:\code\gameworks\physx-3.4\physx_3.4\source\simulationcontroller\src\ScTriggerInteraction.h (99) : warning : Trigger pairs do not support PxPairFlag::eNOTIFY_TOUCH_PERSISTS events any longer.
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;  // Call onContact for each contact point with per-point data

	return physx::PxFilterFlag::eDEFAULT; // Use default filtering behavior applying the above pairFlags
}

// =========================================================================================
// ContactReportCallback Methods Implementation 
// =========================================================================================

void ContactReportCallback::onContact(
    const physx::PxContactPairHeader &pairHeader, // Actors involved in the contact. (hint: check ctrl+click to see struct definition)
    const physx::PxContactPair *pairs,            // Shapes that make the pair
    physx::PxU32 nbPairs)                         // How many pairs are there
{
	PX_UNUSED(pairs);
	PX_UNUSED(nbPairs);

	physx::PxActor* actor1 = pairHeader.actors[0]; // Extracts the first actor involved in the contact
	physx::PxActor* actor2 = pairHeader.actors[1]; // Extracts the second actor involved in the contact

	_engine->notifyContact(actor1, actor2); // Notify the physics engine about the contact event
}

void ContactReportCallback::onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count)
{
	// No implementation.
	// To avoid compiler warnings:
	PX_UNUSED(constraints);
	PX_UNUSED(count);
}

void ContactReportCallback::onWake(physx::PxActor **actors, physx::PxU32 count)
{
	// No implementation.
	// To avoid compiler warnings:
	PX_UNUSED(actors);
	PX_UNUSED(count);
}

void ContactReportCallback::onSleep(physx::PxActor **actors, physx::PxU32 count)
{
	// No implementation.
	// To avoid compiler warnings:
	PX_UNUSED(actors);
	PX_UNUSED(count);
}

void ContactReportCallback::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count)
{
	// No implementation.
	// To avoid compiler warnings:
	PX_UNUSED(pairs);
	PX_UNUSED(count);

	// std::cout << "Trigger event detected." << std::endl;
	for (physx::PxU32 i = 0; i < count; ++i) {
		physx::PxTriggerPair& pair = pairs[i];
		physx::PxRigidActor* triggerActor = pair.triggerActor;
		physx::PxRigidActor* otherActor = pair.otherActor;

		if (pair.status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			_engine->notifyTriggerEnter(pairs, count);
			// std::cout << "Trigger Enter Event." << std::endl;
		}
		else if (pair.status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {
			_engine->notifyTriggerExit(pairs, count);
			// std::cout << "Trigger Exit Event." << std::endl;
		}
		// std::cout << "Trigger Actor: " << triggerActor << ", Other Actor: " << otherActor << std::endl;
	}
}

void ContactReportCallback::onAdvance(const physx::PxRigidBody *const *, const physx::PxTransform *, const physx::PxU32)
{
	// No implementation.
}
