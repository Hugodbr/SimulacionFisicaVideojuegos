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
