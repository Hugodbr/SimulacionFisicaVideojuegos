#include "WaterBody.h"

#include "Constants.h"
#include "RopeNodeBody.h"
#include "SurfBoardBody.h"
#include "SurferBody.h"


WaterBody::WaterBody(const physx::PxVec3& topCenter, const std::string& filePath, float scale)
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
    _shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    _shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    
    _density = 1000.0f; // Density of water in kg/m^3
    
    // _body->attachShape(*_shape); // ! Create shape already attaches it to the actor
    // physx::PxRigidBodyExt::updateMassAndInertia(*_body, physx::PxReal(1000.0f)); // Water density approx 1000 kg/m^3 // ! Static bodies do not need mass and inertia
    _body->userData = this; // for collision callbacks
    _scene->addActor(*_body);

    // std::cout << "WaterBody created at position: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
}

void WaterBody::createRenderableEntity(const std::string &filePath, float scale)
{
    // std::cout << "WaterBody::createRenderableEntity() called with filePath: " << filePath << std::endl;
    setRenderableEntity(std::make_unique<ModelSingleMeshMaterial>(filePath));
}

void WaterBody::onEndUpdate(double deltaTime)
{
    // std::cout << "WaterBody::onEndUpdate() called." << std::endl;
    resolveInteractions(); // Handle interactions with affected actors
    // std::cout << "Applying forces to " << _affectedRigidActors.size() << " actors" << std::endl;
}

void WaterBody::onTriggerEnter(physx::PxActor *other)
{
    // std::cout << "WaterBody triggered by another actor entering." << std::endl;
    if (std::find(_affectedRigidActors.begin(), _affectedRigidActors.end(), static_cast<physx::PxRigidActor*>(other)) == _affectedRigidActors.end())
    {
        RigidBody* rb = static_cast<RigidBody*>(other->userData);
        if (dynamic_cast<SurfBoardBody*>(rb)) {
            SurfBoardBody* surfboard = static_cast<SurfBoardBody*>(rb);
            surfboard->setIsJumping(false);
            // std::cout << "SurfBoardBody entered water, setting isJumping to false: " << surfboard->isJumping() << std::endl;
        }
        else if (dynamic_cast<SurferBody*>(rb))
        {
            SurferBody* surfer = static_cast<SurferBody*>(rb);
            surfer->undoJoint();
            return; // Ignore surfboards for buoyancy
        }
        else if (dynamic_cast<RopeNodeBody*>(rb))
        {
            // std::cout << "RopeNodeBody entered water, ignoring buoyancy." << std::endl;
            return; // Ignore rope nodes for buoyancy
        }

        _affectedRigidActors.push_back(static_cast<physx::PxRigidActor*>(other));
    }
    // std::cout << "onTriggerEnter: Total affected actors: " << _affectedRigidActors.size() << std::endl;
}

void WaterBody::onTriggerExit(physx::PxActor *other)
{
    // std::cout << "WaterBody triggered by another actor exiting." << std::endl;
    auto it = std::find(
        _affectedRigidActors.begin(),
        _affectedRigidActors.end(),
        static_cast<physx::PxRigidActor*>(other)
    );
    if (it != _affectedRigidActors.end()) {
        RigidBody* rb = static_cast<RigidBody*>(other->userData);
        if (dynamic_cast<SurfBoardBody*>(rb)) {
            SurfBoardBody* surfboard = static_cast<SurfBoardBody*>(rb);
            surfboard->setIsJumping(true);
        }
        _affectedRigidActors.erase(it);
    }
    // std::cout << "onTriggerExit: Total affected actors: " << _affectedRigidActors.size() << std::endl;
}

void WaterBody::resolveInteractions()
{
    for (physx::PxRigidActor* other : _affectedRigidActors) {
        // std::cout << "Applying buoyant force to affected actor." << std::endl;
        physx::PxVec3 buoyancyForce = calculateBuoyancy(*static_cast<RigidBody*>(other->userData));

        static_cast<physx::PxRigidBody*>(other)->addForce(
            physx::PxVec3(0.0f, buoyancyForce.y, 0.0f), // ! Only vertical
            physx::PxForceMode::eFORCE
        );
    }
}

physx::PxVec3 WaterBody::calculateBuoyancy(RigidBody& affectedBody)
{
    // std::cout << "FROM PHYSICS : -------------------------------------------------" << std::endl;
    // physx::PxBounds3 affectedBodyBounds = static_cast<physx::PxRigidBody*>(affectedBody.getBody())->getWorldBounds();
    // std::cout << "Affected body bounds: minY=" << affectedBodyBounds.minimum.y 
    //       << " maxY=" << affectedBodyBounds.maximum.y << std::endl;
    // physx::PxBounds3 b = static_cast<physx::PxRigidBody*>(this->getBody())->getWorldBounds();
    // std::cout << "Water Trigger bounds: minY=" << b.minimum.y 
    //       << " maxY=" << b.maximum.y << std::endl;
    // std::cout << "FROM GAME LOGIC : ----------------------------------------------" << std::endl;
    // std::cout << "Affected body bounds: minY=" << affectedBody.getBounds().minimum.y 
    //       << " maxY=" << affectedBody.getBounds().maximum.y << std::endl;
    // std::cout << "Water Trigger bounds: minY=" << this->getBounds().minimum.y 
    //       << " maxY=" << this->getBounds().maximum.y << std::endl;
    // std::cout << "Affected body centers: botcenterY=" << affectedBody.getBottomCenter().y 
    //       << " topcenterY=" << affectedBody.getTopCenter().y << std::endl;
    // std::cout << "Water Trigger centers: botcenterY=" << this->getBottomCenter().y 
    //       << " topcenterY=" << this->getTopCenter().y << std::endl;

    // std::cout << "Water level Y: " << _bounds.maximum.y << std::endl;
    // std::cout << "Water bottom Y: " << _bounds.minimum.y << std::endl;
    physx::PxVec3 buoyantForce(0.0f, 0.0f, 0.0f);
    physx::PxVec3 gravity = Constants::Physics::Gravity;
    physx::PxVec3 affectedBodyVelocity = static_cast<physx::PxRigidBody*>(affectedBody.getBody())->getLinearVelocity();
    float bodyVolume = affectedBody.getVolume();
    // std::cout << "Affected body volume: " << bodyVolume << " m^3" << std::endl;
    float bodySubmergedVolume = bodyVolume;

    if (affectedBodyVelocity.magnitude() < 0.01f) {
        // std::cout << "Affected body is nearly stationary. No drag forces applied." << std::endl;
        affectedBodyVelocity = physx::PxVec3(0.0f, 0.0f, 0.0f);
    }
    // Calculate drag forces
    float affectedBodyArea = bodyVolume / (affectedBody.getTopCenter().y - affectedBody.getBottomCenter().y); // approximate cross-sectional area
    // std::cout << "Affected body approximate cross-sectional area: " << affectedBodyArea << " m^2" << std::endl;
    physx::PxVec3 dragForce = - 0.5f * (_Cd * this->_density * affectedBodyArea * affectedBodyVelocity.getNormalized() * affectedBodyVelocity.magnitudeSquared()); // the - sign indicates that the drag force opposes the velocity

    // physx::PxVec3 linearDragForce = -_k1 * affectedBodyVelocity; // the - sign indicates that the drag force opposes the velocity
    // physx::PxVec3 quadraticDragForce = -_k2 * affectedBodyVelocity * affectedBodyVelocity.magnitude(); // quadratic drag

    // Calculate buoyant force based on submerged bounding box volume
    if (affectedBody.getBottomCenter().y > this->getBounds().maximum.y) {
        std::cout << "Body is completely above water. No buoyant force applied. Shouldn't have been triggered. Something is wrong." << std::endl;
        return physx::PxVec3(0.0f, 0.0f, 0.0f); // Body is completely above water
    }
    // Body is completely submerged
    else if (affectedBody.getTopCenter().y < this->getBounds().maximum.y) { 
        // std::cout << "Body is completely submerged. Maximum buoyant force applied." << std::endl;
    }
    // Body is partially submerged
    else {
        // std::cout << "Body is partially submerged. Calculating partial buoyant force." << std::endl;
        // std::cout << "Affected body top center Y: " << affectedBody.getTopCenter().y << ", bottom center Y: " << affectedBody.getBottomCenter().y << std::endl;
        float submergedHeight = this->getBounds().maximum.y - affectedBody.getBottomCenter().y;
        float totalHeight = affectedBody.getTopCenter().y - affectedBody.getBottomCenter().y;
        float submergedFraction = submergedHeight / totalHeight;
        // std::cout << "Submerged height: " << submergedHeight << " m, Total height: " << totalHeight << " m, Submerged fraction: " << submergedFraction << std::endl;
        submergedFraction = physx::PxClamp(submergedFraction, 0.0f, 1.0f);
        // linearDragForce *= submergedFraction;
        // quadraticDragForce *= submergedFraction;
        bodySubmergedVolume *= submergedFraction;
    }
    // std::cout << "TopCenter - WaterLevel = " << (affectedBody.getTopCenter().y - _bounds.maximum.y) << std::endl;
    // std::cout << "BottomCenter - WaterLevel = " << (affectedBody.getBottomCenter().y - _bounds.maximum.y) << std::endl;

    // std::cout << "Affected body submerged volume: " << bodySubmergedVolume << " m^3" << std::endl;
    // std::cout << "drag greater than buoyant force: " << ((((-1) * _density * gravity * bodySubmergedVolume).abs().y) < ((linearDragForce + quadraticDragForce).abs()).y) << std::endl;
    // // Calculate total buoyant force
    buoyantForce = ((-1) * _density * gravity * bodySubmergedVolume) + dragForce;
    // buoyantForce = ((-1) * _density * gravity * bodySubmergedVolume) + linearDragForce + quadraticDragForce;
    // std::cout << "Calculated buoyant force: (" << buoyantForce.x << ", " << buoyantForce.y << ", " << buoyantForce.z << ")" << std::endl;
    // // ! teste Calculate total buoyant force
    // std::cout << "Density: " << _density << " kg/m^3, Gravity: (" << gravity.x << ", " << gravity.y << ", " << gravity.z << "), Submerged Volume: " << bodySubmergedVolume << " m^3" << std::endl;
    // buoyantForce = ((-1) * _density * gravity * bodySubmergedVolume);
    // std::cout << "Calculated buoyant force: (" << buoyantForce.x << ", " << buoyantForce.y << ", " << buoyantForce.z << ")" << std::endl;
    return buoyantForce;
}