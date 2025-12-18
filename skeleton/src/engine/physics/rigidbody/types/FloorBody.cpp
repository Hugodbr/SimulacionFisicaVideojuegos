#include "FloorBody.h"

#include "Constants.h"


FloorBody::FloorBody(const physx::PxVec3& topCenter, const std::string& filePath, physx::PxQuat rotation, float scale)
{
    createRenderableEntity(filePath, scale);
    glm::vec3 dimensions = _renderableEntity->getDimensions();
    calculateVolume(dimensions);

    physx::PxBounds3 bounds = physx::PxBounds3(
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

    physx::PxVec3 center = physx::PxVec3(
        (bounds.minimum.x + bounds.maximum.x) * 0.5f,
        (bounds.minimum.y + bounds.maximum.y) * 0.5f,
        (bounds.minimum.z + bounds.maximum.z) * 0.5f
    );
    
    _body     = _physics->createRigidStatic(physx::PxTransform(center, physx::PxQuat(0.0f, 0.0f, 0.0f, 1.0f)));
    _material = _physics->createMaterial(0.5f, 0.5f, 0.5f);
    _shape    = _body->createShape(
        physx::PxBoxGeometry(
            (bounds.maximum.x - bounds.minimum.x) * 0.5f,
            (bounds.maximum.y - bounds.minimum.y) * 0.5f,
            (bounds.maximum.z - bounds.minimum.z) * 0.5f
        ),
        *_material
    );
    _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    
    _density = 1000.0f; // Density of water in kg/m^3

    rotate(physx::PxQuat(physx::PxPiDivFour, physx::PxVec3(1,0,0)));
    
    // _body->attachShape(*_shape); // ! Create shape already attaches it to the actor
    // physx::PxRigidBodyExt::updateMassAndInertia(*_body, physx::PxReal(1000.0f)); // Water density approx 1000 kg/m^3 // ! Static bodies do not need mass and inertia
    _body->userData = this; // for collision callbacks
    _scene->addActor(*_body);

    // std::cout << "FloorBody created at position: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
}

void FloorBody::createRenderableEntity(const std::string &filePath, float scale)
{
    // std::cout << "FloorBody::createRenderableEntity() called with filePath: " << filePath << std::endl;
    setRenderableEntity(std::make_unique<ModelSingleMeshMaterial>(filePath));
}

void FloorBody::initiallize(const physx::PxVec3 &center)
{
}
