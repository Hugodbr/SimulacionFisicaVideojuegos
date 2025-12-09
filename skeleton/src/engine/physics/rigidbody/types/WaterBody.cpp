#include "WaterBody.h"


WaterBody::WaterBody(const physx::PxVec3& topCenter, const std::string& filePath)
{
    createRenderableEntity(filePath);

    glm::vec3 dimensions = _renderableEntity->getDimensions();

    _bounds = physx::PxBounds3(
        physx::PxVec3(
            topCenter.x - dimensions.x * 0.5f,
            topCenter.y - dimensions.y,
            topCenter.z - dimensions.z * 0.5f
        ),
        physx::PxVec3(
            topCenter.x + dimensions.x * 0.5f,
            topCenter.y,
            topCenter.z + dimensions.z * 0.5f
        )
    );

    physx::PxVec3 positionCenter = (_bounds.minimum + _bounds.maximum) * 0.5f;

    _body     = _physics->createRigidStatic(physx::PxTransform(positionCenter, physx::PxQuat(0.0f, 0.0f, 0.0f, 1.0f)));
    _material = _physics->createMaterial(0.5f, 0.5f, 0.5f);
    _shape    = _body->createShape(
        physx::PxBoxGeometry(
            (_bounds.maximum.x - _bounds.minimum.x) * 0.5f,
            (_bounds.maximum.y - _bounds.minimum.y) * 0.5f,
            (_bounds.maximum.z - _bounds.minimum.z) * 0.5f
        ),
        *_material
    );
    // _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    // _shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);

    // _body->attachShape(*_shape); // ! Create shape already attaches it to the actor
    // physx::PxRigidBodyExt::updateMassAndInertia(*_body, physx::PxReal(1000.0f)); // Water density approx 1000 kg/m^3 // ! Static bodies do not need mass and inertia
    _scene->addActor(*_body);

    // Pose the renderable entity
    _renderableEntity->setPose(
        glm::vec3(positionCenter.x, positionCenter.y, positionCenter.z),
        glm::quat(
            _body->getGlobalPose().q.w,
            _body->getGlobalPose().q.x,
            _body->getGlobalPose().q.y,
            _body->getGlobalPose().q.z
        )
    );
    std::cout << "WaterBody created at position: (" << positionCenter.x << ", " << positionCenter.y << ", " << positionCenter.z << ")" << std::endl;
}

void WaterBody::createRenderableEntity(const std::string &filePath)
{
    std::cout << "WaterBody::createRenderableEntity() called with filePath: " << filePath << std::endl;
    setRenderableEntity(std::make_unique<ModelSingleMeshPBR>(filePath));
}
