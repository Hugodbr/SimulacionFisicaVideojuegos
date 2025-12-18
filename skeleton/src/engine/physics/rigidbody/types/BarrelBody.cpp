#include "BarrelBody.h"



BarrelBody::BarrelBody(const physx::PxVec3 &center, const std::string &filePath, float scale)
{
    createRenderableEntity(filePath, scale);
    initiallize(center);
}

BarrelBody::BarrelBody(const physx::PxVec3 &center, const std::string &filePath, double mass, float scale)
{
    createRenderableEntity(filePath, scale);
    initiallize(center);
    // Set mass after initialization
    setDensity(static_cast<float>(mass) / this->_volume); 
}

BarrelBody::BarrelBody(const physx::PxVec3 &center, const std::string &filePath, float density, float scale)
{
    createRenderableEntity(filePath, scale);
    initiallize(center);
    static_cast<physx::PxRigidDynamic&>(*_body).setMass(density * this->_volume);
}

BarrelBody::BarrelBody(const physx::PxVec3 &center, std::shared_ptr<Abs_Entity> renderableEntity)
{
    setRenderableEntity(renderableEntity);
    std::cout << "BarrelBody::BarrelBody() called with shared_ptr renderableEntity." << std::endl;
    initiallize(center);
}

BarrelBody::BarrelBody(const physx::PxVec3 &center, std::shared_ptr<Abs_Entity> renderableEntity, float density)
{
    setRenderableEntity(renderableEntity);
    std::cout << "BarrelBody::BarrelBody() called with shared_ptr renderableEntity and density." << std::endl;
    initiallize(center);
    // Set mass after initialization
    static_cast<physx::PxRigidDynamic&>(*_body).setMass(density * this->_volume);
}

void BarrelBody::createRenderableEntity(const std::string &filePath, float scale)
{
    std::cout << "BarrelBody::createRenderableEntity() called with filePath: " << filePath << std::endl;
    setRenderableEntity(std::make_shared<ModelSingleMeshPBR>(filePath, scale));
}

void BarrelBody::initiallize(const physx::PxVec3 &center)
{
    glm::vec3 dimensions = _renderableEntity->getDimensions();
    calculateVolume(dimensions);

    std::cout << "BarrelBody dimensions: (" << dimensions.x << ", " << dimensions.y << ", " << dimensions.z << ")" << std::endl;

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
    // std::cout << "BarrelBody _body = " << _body << std::endl;
    _material = _physics->createMaterial(0.5f, 0.5f, 0.9f);

    float radius = dimensions.y * 0.5f;
    float halfHeight = (dimensions.z * 0.5f) - radius;
    _shape    = _body->createShape(
        physx::PxCapsuleGeometry(physx::PxReal(radius), physx::PxReal(halfHeight)),
        *_material
    );

    // _body->attachShape(*_shape); // ! Create shape already attaches it to the actor
    _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);

    // _density = physx::PxReal(200.0f); // kg/m^3
    // physx::PxRigidBodyExt::updateMassAndInertia(static_cast<physx::PxRigidDynamic&>(*_body), _density);
    setDensity(300.0f);
    

    // ! Cylinder inertia
    float Ixx = (1.0f / 12.0f) * static_cast<physx::PxRigidDynamic&>(*_body).getMass() * (3 * (dimensions.y * 0.5f) * (dimensions.y * 0.5f) + (dimensions.z) * (dimensions.z));
    float Iyy = Ixx;
    float Izz = (0.5f) * static_cast<physx::PxRigidDynamic&>(*_body).getMass() * (dimensions.y * 0.5f) * (dimensions.y * 0.5f);
    this->setMassSpaceInertiaTensor(physx::PxVec3(Ixx, Iyy, Izz));

    // std::cout << "BarrelBody density set to " << _density << " kg/m^3. Has mass = " << static_cast<physx::PxRigidDynamic&>(*_body).getMass() << " and volume: " << this->_volume << std::endl;

    // Apply initial rotation to the box
    // ! Must be done before adding to scene !!!
    // rotate(physx::PxQuat(physx::PxPiDivTwo, physx::PxVec3(-1,0,0)));

    _body->userData = this; // for collision callbacks
    _scene->addActor(*_body);

    std::cout << "BarrelBody created at position: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
}
