#include "engine/physics/rigidbody/RigidBody.h"

#include "PhysicsEngine.h"
#include "RigidBody.h"


RigidBody::RigidBody()
    : PhysicalObject(
        physx::PxTransform(physx::PxVec3(0,0,0), physx::PxQuat(0, physx::PxVec3(0,0,0))), // default transform
        physx::PxVec3(0,0,0), // default velocity
        Constants::Particle::Default::Size, // default size
        Constants::Physics::Damping, // default damping
        Constants::Color::White, // default color
        Constants::Integration_Method::VERLET
    )
{
    _physicsEngine = &PhysicsEngine::getInstance();

    _physics  = _physicsEngine->getPhysics();
    _scene    = _physicsEngine->getScene();
}

RigidBody::~RigidBody()
{
    // Seams like PhysX releases the shapes and materials when the actor is released
    // So no need to release them here again (double free issue)
    // _shape->release();
    // _material->release();
}

void RigidBody::update(double deltaTime)
{
    onBeginUpdate(deltaTime);
    // Rigid bodies are updated by the PhysX simulation step
    updateAge(deltaTime);

    updateRenderableEntityPose();

    onEndUpdate(deltaTime);
}

void RigidBody::setRenderableEntity(std::unique_ptr<Abs_Entity> renderable)
{
    std::cout << "RigidBody::setRenderableEntity() called." << std::endl;
    if (_renderableEntity) {
        std::cout << "Warning: Overwriting existing renderable entity!" << std::endl;
    }
    _renderableEntity = std::move(renderable);
}

void RigidBody::load()
{
    if (_renderableEntity) {
        _renderableEntity->load();
    } else {
        std::cerr << "RigidBody::load() - No renderable entity set!" << std::endl;
    }
}

void RigidBody::unload()
{
    if (_renderableEntity) {
        _renderableEntity->unload();
    }
}

void RigidBody::render(const glm::mat4 &modelViewMat)
{
    if (_renderableEntity) {
        _renderableEntity->render(modelViewMat);
    } else {
        std::cerr << "RigidBody::render() - No renderable entity set!" << std::endl;
    }
}

physx::PxVec3 RigidBody::getTopCenter() const
{
    physx::PxBounds3 bounds = this->getBounds();
    
    physx::PxVec3 topCenter = physx::PxVec3(
        (bounds.minimum.x + bounds.maximum.x) * 0.5f,
        bounds.maximum.y,
        (bounds.minimum.z + bounds.maximum.z) * 0.5f
    );
    return topCenter;
}

physx::PxVec3 RigidBody::getBottomCenter() const
{
    physx::PxBounds3 bounds = this->getBounds();
    
    physx::PxVec3 bottomCenter = physx::PxVec3(
        (bounds.minimum.x + bounds.maximum.x) * 0.5f,
        bounds.minimum.y,
        (bounds.minimum.z + bounds.maximum.z) * 0.5f
    );
    return bottomCenter;
}

void RigidBody::updateRenderableEntityPose()
{
    if (_renderableEntity) {
        _renderableEntity->setPose(
            glm::vec3(
                _body->getGlobalPose().p.x,
                _body->getGlobalPose().p.y,
                _body->getGlobalPose().p.z
            ),
            glm::quat(
                _body->getGlobalPose().q.w,
                _body->getGlobalPose().q.x,
                _body->getGlobalPose().q.y,
                _body->getGlobalPose().q.z
            )
        );
    } else {
        std::cerr << "RigidBody::updateRenderableEntityPose() - No renderable entity set!" << std::endl;
    }
}

void RigidBody::rotate(const physx::PxQuat &deltaRotation)
{
    std::cout << "RigidBody::rotate _body = " << _body << std::endl;

    physx::PxTransform pose = _body->getGlobalPose();
    pose.q = deltaRotation * pose.q;
    _body->setGlobalPose(pose);

    updateRenderableEntityPose();
}

void RigidBody::calculateVolume(const glm::vec3& dimensions) {
    _volume = dimensions.x * dimensions.y * dimensions.z;
}

// void RigidBody::calculateParameters()
// {
//     physx::PxBounds3 bounds = this->_bounds;
//     // physx::PxBounds3 bounds = this->_body->getWorldBounds();
//     // std::cout << "RigidBody::calculateParameters() - Calculated bounds: min(" 
//     //           << bounds.minimum.x << ", " << bounds.minimum.y << ", " << bounds.minimum.z << ") "
//     //           << "max(" << bounds.maximum.x << ", " << bounds.maximum.y << ", " << bounds.maximum.z << ")"
//     //           << std::endl;

//     _topCenter = physx::PxVec3(
//         (bounds.minimum.x + bounds.maximum.x) * 0.5f,
//         bounds.maximum.y,
//         (bounds.minimum.z + bounds.maximum.z) * 0.5f
//     );

//     _bottomCenter = physx::PxVec3(
//         (bounds.minimum.x + bounds.maximum.x) * 0.5f,
//         bounds.minimum.y,
//         (bounds.minimum.z + bounds.maximum.z) * 0.5f
//     );

//     _center = physx::PxVec3(
//         (bounds.minimum.x + bounds.maximum.x) * 0.5f,
//         (bounds.minimum.y + bounds.maximum.y) * 0.5f,
//         (bounds.minimum.z + bounds.maximum.z) * 0.5f
//     );

//     _volume = (bounds.maximum.x - bounds.minimum.x) *
//               (bounds.maximum.y - bounds.minimum.y) *
//               (bounds.maximum.z - bounds.minimum.z);
// }

// void RigidBody::refreshParameters()
// {
//     physx::PxTransform pose = _body->getGlobalPose();
//     physx::PxBounds3 bounds = this->_body->getWorldBounds();

//     _bounds = physx::PxBounds3(
//         physx::PxVec3(
//             pose.p.x - (_bounds.maximum.x - _bounds.minimum.x) * 0.5f,
//             pose.p.y - (_bounds.maximum.y - _bounds.minimum.y) * 0.5f,
//             pose.p.z - (_bounds.maximum.z - _bounds.minimum.z) * 0.5f
//         ),
//         physx::PxVec3(
//             pose.p.x + (_bounds.maximum.x - _bounds.minimum.x) * 0.5f,
//             pose.p.y + (_bounds.maximum.y - _bounds.minimum.y) * 0.5f,
//             pose.p.z + (_bounds.maximum.z - _bounds.minimum.z) * 0.5f
//         )
//     );

//     calculateParameters();
// }
