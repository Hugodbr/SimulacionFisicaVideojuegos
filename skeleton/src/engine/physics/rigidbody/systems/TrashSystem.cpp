#include "TrashSystem.h"

#include "RBGenerationPolicy.h"
#include "RBLifetimePolicy.h"


TrashSystem::TrashSystem(const physx::PxVec3 &origin, const Region& region, const std::string& filePath, float scale)
    : RigidBodySystem()
    , _region(region)
{
    _origin = origin;
    initRigidBodyGeneratorAndPool(filePath, scale);
}

void TrashSystem::init()
{

}

void TrashSystem::initRigidBodyGeneratorAndPool(const std::string &filePath, float scale)
{
    setRenderableEntity(std::make_shared<ModelSingleMeshPBR>(filePath, scale));

    _generatorAndPool = {
        std::make_unique<GaussianRigidBodyGenerator>(),
        std::make_unique<RigidBodyPool<BoxBody>>(
			Constants::System::Trash::ReserveCountPerGenerator,  // Pool size
                physx::PxVec3(0),
                _renderableEntity
            )
        };

    auto& generator = _generatorAndPool.first;

    generator->init(
        _origin,
        Vector3Stats(physx::PxVec3(0), physx::PxVec3(0)) // velocity = 0
    );

    // Create generation policy
	RBGenerationPolicy genPolicy = RBGenerationPolicy(
        true, ScalarStats(1.0, 0.0),
        false, ScalarStats(0.0, 0.0)
    );

	physx::PxBounds3 box = physx::PxBounds3(
		physx::PxVec3(_region.shape.box.minimum.x, _region.shape.box.minimum.y, _region.shape.box.minimum.z),
		physx::PxVec3(_region.shape.box.maximum.x, _region.shape.box.maximum.y, _region.shape.box.maximum.z)
	);
	Region generationRegion(box);

	genPolicy.setRegion(generationRegion);

    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    RBLifetimePolicy lifePolicy = RBLifetimePolicy(ScalarStats(5.0, 1.0)); // mean 5s, dev 2s
    
	generator->setLifetimePolicy(lifePolicy);

    for (auto* rb : _generatorAndPool.second->accessRigidBodyPool()) {
        rb->deactivate();
    }

}

void TrashSystem::update(double deltaTime)
{
    // ! IMPORTANT ! Must call base update to handle sub-systems and inside forces
	RigidBodySystem::update(deltaTime);
    if (!isActive()) {
        return;
    }

    if (_followTarget) {
        physx::PxVec3 targetPos = _followTarget->getPosition();
        _generatorAndPool.first->setEmitterOrigin(targetPos);
    }

	// Apply forces to all particles
	applyForces();

    auto& gen = _generatorAndPool.first;
    auto& pool = _generatorAndPool.second;

    int numToSpawn = 0;

    // Check if should spawn RBs this frame
    if (mustSpawnRigidBody(deltaTime, *gen)) {
        numToSpawn = gen->getGenerationPolicy().spawnNumber(gen->getDistribution());
    }

    // Spawn "new" RBs (getting from the pool)
    for (int i = 0; i < numToSpawn; ++i) 
    {
        auto* rb = pool->activateRigidBody();
        if (rb) {
            setRenderableForRigidBody(*rb);
            physx::PxTransform t = physx::PxTransform(0, 0, 0, physx::PxQuat(0, 0, 0, 1));
            t.p = gen->getGeneratedPosition();
            // std::cout << "Spawning Trash at: (" << t.p.x << ", " << t.p.y << ", " << t.p.z << ")\n";
            rb->setPose(t);
        }
        else break;
    }

    // Update active particles and deactivate if needed
    auto& rbs = pool->accessRigidBodyPool();
    for (int i = 0; i < pool->getActiveCount(); ++i) 
    {
        // std::cout << "Active Rain Particles: " << pool->getActiveCount() << std::endl;
        // Update particle
        rbs[i]->update(deltaTime);

        if (mustKillRigidBody(*rbs[i], *gen)) {
            pool->deactivateRigidBody(i);
            --i; // Adjust index after deactivation
        }
    }

    // std::cout << "target pos: (" << _followTarget->getPosition().x << ", " << _followTarget->getPosition().y << ", " << _followTarget->getPosition().z << ")\n";
}

void TrashSystem::render(const glm::mat4 &modelViewMat)
{
    for (int i = 0; i < _generatorAndPool.second->getActiveCount(); ++i) {
        auto& rb = _generatorAndPool.second->accessRigidBodyPool()[i];
        if (rb->isActive()) {
            rb->updateRenderableEntityPose(); // because the renderable is shared among all rigid bodies in the system
            _renderableEntity->render(modelViewMat);
        }
    }
}

void TrashSystem::setFollowTarget(PhysicalObject &target)
{
    _followTarget = &target;
}

void TrashSystem::applyForces()
{
}
