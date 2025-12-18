#include "SurferBody.h"



SurferBody::SurferBody(const physx::PxVec3& center, const std::string& filePath, float scale)
{
    createRenderableEntity(filePath, scale);
    initiallize(center);
}

void SurferBody::createRenderableEntity(const std::string &filePath, float scale)
{
    std::cout << "SurferBody::createRenderableEntity() called with filePath: " << filePath << std::endl;
    setRenderableEntity(std::make_shared<ModelSingleMeshPBR>(filePath, scale));
}

physx::PxVec3 SurferBody::getDirectionForward() const
{
    physx::PxQuat orientation = _body->getGlobalPose().q;
    physx::PxVec3 forward(0.0f, 0.0f, 1.0f);
    physx::PxVec3 direction = orientation.rotate(forward);
    return direction.getNormalized();
}

physx::PxVec3 SurferBody::getDirectionRight() const
{
    physx::PxQuat orientation = _body->getGlobalPose().q;
    physx::PxVec3 right(1.0f, 0.0f, 0.0f);
    physx::PxVec3 direction = orientation.rotate(right);
    return direction.getNormalized();
}

physx::PxVec3 SurferBody::getDirectionUp() const
{
    physx::PxQuat orientation = _body->getGlobalPose().q;
    physx::PxVec3 up(0.0f, 1.0f, 0.0f);
    physx::PxVec3 direction = orientation.rotate(up);
    return direction.getNormalized();
}

void SurferBody::initiallize(const physx::PxVec3 &center)
{
    glm::vec3 dimensions = _renderableEntity->getDimensions();
    calculateVolume(dimensions);

    // Initially, the bounds can be set based on the center and dimensions
    // but after simulation starts, we should get the bounds from the physics body
    physx::PxBounds3 bounds = physx::PxBounds3(
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
    
    _body     = _physics->createRigidDynamic(physx::PxTransform(center, physx::PxQuat(0.0f, 0.0f, 0.0f, 1.0f)));
    std::cout << "SurferBody _body = " << _body << std::endl;
    _material = _physics->createMaterial(0.5f, 0.5f, 0.9f);
    _shape    = _body->createShape(
        physx::PxBoxGeometry(
            (bounds.maximum.x - bounds.minimum.x) * 0.5f,
            (bounds.maximum.y - bounds.minimum.y) * 0.5f,
            (bounds.maximum.z - bounds.minimum.z) * 0.5f
        ),
        *_material
    );
    // _body->attachShape(*_shape); // ! Create shape already attaches it to the actor
    _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    // static_cast<physx::PxRigidDynamic*>(_body)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
    // static_cast<physx::PxRigidDynamic*>(_body)->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);

    // static_cast<physx::PxRigidDynamic*>(_body)->setLinearDamping(0.4f);

    // _density = physx::PxReal(200.0f); // kg/m^3
    // physx::PxRigidBodyExt::updateMassAndInertia(static_cast<physx::PxRigidDynamic&>(*_body), _density);
    setDensity(100.0f);
    std::cout << "SurferBody density set to " << _density << " kg/m^3. Has mass = " << static_cast<physx::PxRigidDynamic&>(*_body).getMass() << " and volume: " << this->_volume << std::endl;

    // Apply initial rotation to the box
    // ! Must be done before adding to scene !!!
    // rotate(physx::PxQuat(physx::PxPi, physx::PxVec3(0,1,0)));

    _body->userData = this; // for collision callbacks
    _scene->addActor(*_body);

    // Print bounds for debugging
    // std::cout << "SurferBody bounds after initialization: " << std::endl;
    // std::cout << "  Min: (" << _body->getWorldBounds().minimum.x << ", " << _body->getWorldBounds().minimum.y << ", " << _body->getWorldBounds().minimum.z << ")" << std::endl;
    // std::cout << "  Max: (" << _body->getWorldBounds().maximum.x << ", " << _body->getWorldBounds().maximum.y << ", " << _body->getWorldBounds().maximum.z << ")" << std::endl;

    // Print dimensions based on bounds
    physx::PxBounds3 updatedBounds = _body->getWorldBounds();
    // std::cout << "SurferBody dimensions after initialization: " << std::endl;
    // std::cout << "  Width: " << (updatedBounds.maximum.x - updatedBounds.minimum.x) << std::endl;
    // std::cout << "  Height: " << (updatedBounds.maximum.y - updatedBounds.minimum.y) << std::endl;
    // std::cout << "  Depth: " << (updatedBounds.maximum.z - updatedBounds.minimum.z) << std::endl;
    
    updateRenderableEntityPose();
    // std::cout << "SurferBody created at position: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
}
