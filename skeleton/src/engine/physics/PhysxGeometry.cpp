// #include "PhysxGeometry.h"

// #include "engine/physics/PhysicsEngine.h"


// PhysxGeometry::PhysxGeometry()
// {
//     _physicsEngine = &PhysicsEngine::getInstance();
// }

// PhysxGeometry::~PhysxGeometry()
// {
// }

// std::unique_ptr<Geometry> PhysxGeometry::createBoxFromPhysxGeometry(const physx::PxGeometry &physxGeo)
// {
//     std::unique_ptr<Geometry> geom = std::make_unique<PhysxBoxGeometry>(static_cast<const physx::PxBoxGeometry&>(physxGeo).halfExtents.x);



//     return geom;
// }

// std::unique_ptr<Geometry> PhysxGeometry::createSphereFromPhysxGeometry(const physx::PxGeometry &physxGeo)
// {
//     return nullptr;
// }

// PhysxSphereGeometry::PhysxSphereGeometry(float radius)
//     : SphereGeometry(radius)
// {
//     physx::PxPhysics* physics = _physicsEngine->getPhysics();
// }

// PhysxBoxGeometry::PhysxBoxGeometry(float length)
//     : BoxGeometry(length)
// {
// }
