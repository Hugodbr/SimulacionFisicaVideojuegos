#include "BoxBody.h"



BoxBody::BoxBody(const physx::PxVec3 &center, const std::string &filePath, float scale)
{
    createRenderableEntity(filePath, scale);
    initiallize(center);
}

BoxBody::BoxBody(const physx::PxVec3 &center, std::shared_ptr<Abs_Entity> renderableEntity)
{
    setRenderableEntity(renderableEntity);
    std::cout << "BoxBody::BoxBody() called with shared_ptr renderableEntity." << std::endl;
    initiallize(center);
}

void BoxBody::createRenderableEntity(const std::string &filePath, float scale)
{
    std::cout << "BoxBody::createRenderableEntity() called with filePath: " << filePath << std::endl;
    setRenderableEntity(std::make_shared<ModelSingleMeshPBR>(filePath, scale));
}

void BoxBody::initiallize(const physx::PxVec3 &center)
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
    std::cout << "BoxBody _body = " << _body << std::endl;
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

    // _density = physx::PxReal(200.0f); // kg/m^3
    // physx::PxRigidBodyExt::updateMassAndInertia(static_cast<physx::PxRigidDynamic&>(*_body), _density);
    setDensity(200.0f);
    std::cout << "BoxBody density set to " << _density << " kg/m^3. Has mass = " << static_cast<physx::PxRigidDynamic&>(*_body).getMass() << " and volume: " << this->_volume << std::endl;

    // Apply initial rotation to the box
    // ! Must be done before adding to scene !!!
    // rotate(physx::PxQuat(physx::PxPiDivTwo, physx::PxVec3(-1,0,0)));

    _body->userData = this; // for collision callbacks
    _scene->addActor(*_body);

    std::cout << "BoxBody created at position: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
}
