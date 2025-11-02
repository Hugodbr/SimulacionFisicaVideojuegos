#include "GunSystem.h"

#include "MathUtils.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "ConstantParticleGenerator.h"
#include "ForceGenerator.h"
#include "StaticParticle.h"
#include "Bullet.h"



GunSystem::GunSystem(const physx::PxVec3 &position, Camera* cam)
    : ParticleSystem()
    , _gunMeshGeneratorAndPool()
    , _gunBulletsGeneratorAndPool()
    , _camera(cam)
    , currentAmmunition(0) // index of the generator/pool to use
    , isShooting(false)
{
    _emitterOrigin = position;
}

void GunSystem::init()
{
    initParticleGeneratorAndPool();
}

void GunSystem::initParticleGeneratorAndPool()
{
    initGunMesh(); 
    createGun();

    initGunBullets();
}

void GunSystem::initGunMesh()
{
    // ONE MESH  GUN -----------------------------------------------------
    // Gun mesh particle size
    float size = 0.3f;
    // Gun color
    physx::PxVec4 color = Constants::Color::Green;

    _gunMeshGeneratorAndPool.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<StaticParticle>>(
            getReserveCountPerGenerator(),  // Pool size
            size, // size particle
            color  // color particle
        )
    });

    auto& generator = _gunMeshGeneratorAndPool.back().first;

    generator->init(_emitterOrigin);

    // Create generation policy
	ParticleGenerationPolicy genPolicy;
    _meshDataGun.emplace_back();
	_meshDataGun.back().loadMeshFromFile("../resources/flashLight.obj");
	genPolicy.setRegion(Region(_meshDataGun.back()));
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy;
	generator->setLifetimePolicy(lifePolicy);
    // ONE MESH END -----------------------------------------------------       

    // SECOND MESH -----------------------------------------------------
    // Gun muzzle
        // Gun mesh particle size
    float muzzleSize = 0.4f;
    // Gun color
    Vector4 muzzleColor = Constants::Color::Red;

    _gunMuzzleGeneratorAndPool.push_back({
        std::make_unique<GaussianParticleGenerator>(),
        std::make_unique<ParticlePool<Particle>>(
            2000,  // Pool size
            muzzleSize, // size particle
            muzzleColor,  // color particle
            1000.0f // initial speed
        )
    });

    auto& muzzleGenerator = _gunMuzzleGeneratorAndPool.back().first;

    generator->init(_emitterOrigin);

    // Create generation policy
	ParticleGenerationPolicy muzzleGenPolicy(SpawnMode::Count, ScalarStats(500, 100)); // 400 particles per shot
    _meshDataMuzzle.emplace_back();
	_meshDataMuzzle.back().loadMeshFromFile("../resources/muzzle.obj");
	muzzleGenPolicy.setRegion(Region(_meshDataMuzzle.back()));
    muzzleGenerator->setGenerationPolicy(muzzleGenPolicy);

    // Create lifetime policy
    // _meshDataMuzzle.emplace_back();
    // _meshDataMuzzle.back().loadMeshFromFile("../resources/muzzleConstraint.obj");
    ParticleLifetimePolicy muzzleLifePolicy(ScalarStats(0.03, 0.02)); // 0.03 seconds lifetime
    // muzzleLifePolicy.setVolumeRegion(Region(_meshDataMuzzle.back()));
	muzzleGenerator->setLifetimePolicy(muzzleLifePolicy);
}

void GunSystem::initGunBullets()
{
    // ONE BULLET TYPE -----------------------------------------------------
    
    // Bullet size
    float size = 0.1f;
    // Bullet color
    physx::PxVec4 color = Constants::Color::Red;

    _gunBulletsGeneratorAndPool.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<Bullet>>(
            Constants::System::Gun::ReserveCountPerBulletGenerator,  // Pool size
            _camera->getTransformRelativeToCamera(_emitterOrigin.x, _emitterOrigin.y, _emitterOrigin.z), // position particle
            _camera->getDir() // direction particle
        )
    });

    auto& generator = _gunBulletsGeneratorAndPool.back().first;

    generator->init(_emitterOrigin);

    // Create generation policy
    ParticleGenerationPolicy genPolicy;
    Region bulletRegion(RegionType::POINT_3D, Vector3Stats()); // Default region at origin
    genPolicy.setRegion(bulletRegion);
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy(ScalarStats(0.8, 0.0)); // 0.8 seconds lifetime
    generator->setLifetimePolicy(lifePolicy);
    // ONE BULLET TYPE END -----------------------------------------------------
}

void GunSystem::createGun()
{
    for (int i = 0; i < _gunMeshGeneratorAndPool.size(); ++i) 
    {
        auto& [gen, pool] = _gunMeshGeneratorAndPool[i];

        auto& vertices = _meshDataGun[i].getMeshVertices();
        // std::cout << "GunSystem::createGun -> vertices size: " << vertices.size() << std::endl;

        // Spawn "new" particles (getting from the pool)
        for (const auto& v : vertices)
        {
            auto* p = pool->activateParticle();
            if (p) {
                physx::PxTransform t = physx::PxTransform(v, physx::PxQuat(0));
                p->setTransformRelative(physx::PxTransform(_emitterOrigin - t.p, physx::PxQuat(0)));
                p->setTransform(t); 
                p->setAcceleration(physx::PxVec3(0.0f, 0.0f, 0.0f));
            }
            else break;
        }	
    }
}

void GunSystem::updateGunMesh(double deltaTime)
{
    _gunTransform = _camera->getTransformRelativeToCamera(
        _emitterOrigin.z, _emitterOrigin.x, _emitterOrigin.y
    );
    
    float stiffness = 10.0f; // how fast it catches up (try 5–15)
    float t = 1.0f - expf(-stiffness * (float)deltaTime); // framerate-independent smoothing

    for (auto& [gen, pool] : _gunMeshGeneratorAndPool) 
    {
        auto& particles = pool->accessParticlePool();
        for (int i = 0; i < pool->getActiveCount(); ++i) 
        {
            physx::PxTransform current = particles[i]->getTransform();

            // Target transform (camera-relative)
            physx::PxVec3 targetPos = _gunTransform.transform(particles[i]->getRelativeTransform().p);
            physx::PxQuat targetRot = _gunTransform.q;

            // Smoothly interpolate position and rotation
            physx::PxVec3 smoothedPos = lerp(current.p, targetPos, t);
            physx::PxQuat smoothedRot = slerp(current.q, targetRot, t);

            particles[i]->setTransform(physx::PxTransform(smoothedPos, smoothedRot));
        }
    }
}

void GunSystem::updateGunBullets(double deltaTime)
{
    for (auto& [gen, pool] : _gunBulletsGeneratorAndPool) 
    {
        for (int i = 0; i < pool->getActiveCount(); ++i)
        {
            auto* p = pool->accessParticlePool()[i];
            if (p) {
                p->update(deltaTime);

                if (gen->getLifetimePolicy().shouldDelete(gen->getDistribution(), *p)) {
                    pool->deactivate(i);
                    --i; // Adjust index after deactivation
                }
            }
        }
    }
}

void GunSystem::createMuzzleFlash(double deltaTime)
{
    // std::cout << "GunSystem::createMuzzleFlash -> Creating muzzle flash!" << std::endl;

    for (int i = 0; i < _gunMuzzleGeneratorAndPool.size(); ++i) 
    {
        auto& [gen, pool] = _gunMuzzleGeneratorAndPool[i];

        int numToSpawn = gen->getGenerationPolicy().spawnNumber(gen->getDistribution());

        // Spawn "new" particles (getting from the pool)
        for (int j = 0; j < numToSpawn; ++j)
        {
            auto* p = pool->activateParticle();
            if (p) {
                // Generates a position inside the mesh region
                physx::PxTransform t = physx::PxTransform(gen->getGeneratedPosition(), physx::PxQuat(0));
                p->setTransformRelative(physx::PxTransform(_emitterOrigin - t.p, physx::PxQuat(0)));

                // Target transform (camera-relative)
                physx::PxVec3 targetPos = _gunTransform.transform(p->getRelativeTransform().p);
                physx::PxQuat targetRot = _gunTransform.q;
                p->setTransform(physx::PxTransform(targetPos, targetRot));
                p->setVelocity(_camera->getDir() * 1000.0f); // Muzzle flash speed
            }
            else break;
        }	
    }
}

void GunSystem::updateMuzzleFlash(double deltaTime)
{
    for (auto& [gen, pool] : _gunMuzzleGeneratorAndPool) 
    {
        auto& particles = pool->accessParticlePool();
        
        for (int i = 0; i < pool->getActiveCount(); ++i)
        {
            particles[i]->update(deltaTime);

            if (gen->getLifetimePolicy().shouldDelete(gen->getDistribution(), *particles[i])) {
                pool->deactivate(i);
                --i; // Adjust index after deactivation
            }
        }
    }

}

void GunSystem::update(double deltaTime)
{
    updateGunMesh(deltaTime);
    updateGunBullets(deltaTime);
    if (isShooting) {
        createMuzzleFlash(deltaTime);
        isShooting = false;
    }
    updateMuzzleFlash(deltaTime);
}

void GunSystem::setTransform(const physx::PxTransform &t)
{
    _emitterOrigin = t.p;
}

void GunSystem::shoot()
{
    isShooting = true;

    physx::PxVec3 bulletDirection = _camera->getDir().getNormalized();

    auto& [gen, pool] = _gunBulletsGeneratorAndPool[currentAmmunition];

    // Spawn "new" particle (getting from the pool)
    auto* p = pool->activateParticle();

    if (p) {
        p->setTransform(_gunTransform);
        p->setVelocityDirection(bulletDirection);
    }
}
