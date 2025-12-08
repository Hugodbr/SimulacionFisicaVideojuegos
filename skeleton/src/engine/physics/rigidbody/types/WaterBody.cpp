#include "WaterBody.h"


WaterBody::WaterBody(const physx::PxBounds3& bounds)
{
    physx::PxVec3 center = bounds.minimum + bounds.maximum * 0.5f;

    _body     = _physics->createRigidStatic(physx::PxTransform(center, physx::PxQuat(0, physx::PxVec3(0,0,0))));
    _material = _physics->createMaterial(0.5f, 0.5f, 0.1f);

    _shape    = _body->createShape(
        physx::PxBoxGeometry(
            (bounds.maximum.x - bounds.minimum.x) * 0.5f,
            (bounds.maximum.y - bounds.minimum.y) * 0.5f,
            (bounds.maximum.z - bounds.minimum.z) * 0.5f
        ),
        *_material
    );
    // _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    // _shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

    _body->attachShape(*_shape);
    _scene->addActor(*_body);
}