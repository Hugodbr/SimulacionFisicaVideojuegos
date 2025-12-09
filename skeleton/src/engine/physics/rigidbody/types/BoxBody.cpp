#include "BoxBody.h"



BoxBody::BoxBody(const physx::PxVec3 &center, const std::string &filePath)
{
    createRenderableEntity(filePath);

    glm::vec3 dimensions = _renderableEntity->getDimensions();

    _bounds = physx::PxBounds3(
        physx::PxVec3(
            center.x - dimensions.x * 0.5f,
            center.y - dimensions.y * 0.5f,
            center.z - dimensions.z * 0.5f
        ),
        physx::PxVec3(
            center.x + dimensions.x * 0.5f,
            center.y + dimensions.y * 0.5f,
            center.z + dimensions.z * 0.5f
        )
    );

    // ! TEST apply rotation
    physx::PxQuat rotationQuat = physx::PxQuat(0.0, 0.0, 0.0, 1.0);
    physx::PxQuat delta = physx::PxQuat(physx::PxPi / 5, physx::PxVec3(1, 0, 0)); // Rotate 45 degrees around X axis
    rotationQuat *= delta;
    physx::PxTransform currentPose = physx::PxTransform(center, rotationQuat);
    
    _body     = _physics->createRigidDynamic(currentPose);
    _material = _physics->createMaterial(0.5f, 0.5f, 0.9f);
    _shape    = _body->createShape(
        physx::PxBoxGeometry(
            (_bounds.maximum.x - _bounds.minimum.x) * 0.5f,
            (_bounds.maximum.y - _bounds.minimum.y) * 0.5f,
            (_bounds.maximum.z - _bounds.minimum.z) * 0.5f
        ),
        *_material
    );
    _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    // _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    // _shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    // _shape->setContactOffset(0.02f);
    // _shape->setRestOffset(0.0f);

    // _body->attachShape(*_shape); // ! Create shape already attaches it to the actor
    physx::PxRigidBodyExt::updateMassAndInertia(static_cast<physx::PxRigidDynamic&>(*_body), physx::PxReal(1000.0f)); // Approximate density for a wooden crate

    _scene->addActor(*_body);


    // Pose the renderable entity
    _renderableEntity->setPose(
        glm::vec3(center.x, center.y, center.z),
        glm::quat(
            _body->getGlobalPose().q.w,
            _body->getGlobalPose().q.x,
            _body->getGlobalPose().q.y,
            _body->getGlobalPose().q.z)
    );
    std::cout << "BoxBody created at position: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
}

void BoxBody::createRenderableEntity(const std::string &filePath)
{
    std::cout << "BoxBody::createRenderableEntity() called with filePath: " << filePath << std::endl;
    setRenderableEntity(std::make_unique<ModelSingleMeshPBR>(filePath));
}
