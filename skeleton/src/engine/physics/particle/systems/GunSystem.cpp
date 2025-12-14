#include "GunSystem.h"

#include "MathUtils.h"
#include "ForceManager.h"
#include "ForceGenerator.h"
#include "WindRegionForce.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "ConstantParticleGenerator.h"
#include "StaticParticle.h"
#include "Bullet.h"
#include "Camera.h"


GunSystem::GunSystem(const physx::PxVec3 &position, Camera* cam, const std::string& gunMeshFilename)
    : ParticleSystem()
    , _gunMeshGeneratorAndPool()
    , _gunBulletsGeneratorAndPool()
    , _camera(cam)
    , _gunMeshFilename(gunMeshFilename)
    , currentAmmunition(0) // index of the generator/pool to use
    , isShooting(false)
{
    _emitterOrigin = position;
}

void GunSystem::init()
{
    initParticleGeneratorAndPool();
}

void GunSystem::applyForces()
{
    // Get all forces available from ForceManager
	std::vector<ForceGenerator*> forceGenerators = _forceManager.getForceGenerators();

    // ONLY FOR BULLETS
	for (auto& [generator, pool] : _gunBulletsGeneratorAndPool) // for each pool
	{
		for (auto& forceGen : forceGenerators) // for each force generator
		{
			if (forceGen->isActive() && doForceAffectsSystem(*forceGen)) {
				for (int i = 0; i < pool->getActiveCount(); ++i) 
				{
					auto& particle = pool->accessParticlePool()[i];
					particle->clearForces();
					particle->applyForce(*forceGen);
				}
			}
		}
	}
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
    float size = 0.4f;
    // Gun color
    physx::PxVec4 color = Constants::Color::Green;

    _meshDataGun.emplace_back();
	_meshDataGun.back().loadMeshFromFile(_gunMeshFilename);

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
	genPolicy.setRegion(Region(_meshDataGun.back()));
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy;
	generator->setLifetimePolicy(lifePolicy);
    // ONE MESH END -----------------------------------------------------       

    // SECOND MESH -----------------------------------------------------
    // Gun muzzle1
    // Gun mesh particle size
    float muzzleSize = 0.6f;
    // Gun color
    physx::PxVec4 color1 = Constants::Color::Red;
    physx::PxVec4 colorDeviation = physx::PxVec4(0.4f, 0.3f, 0.1f, 0.0f);
    ColorStats muzzleColor(color1, colorDeviation);

    _gunMuzzleGeneratorAndPool.push_back({
        std::make_unique<GaussianParticleGenerator>(),
        std::make_unique<ParticlePool<Particle>>(
            Constants::System::Gun::ReserveCountPerMuzzleGenerator,  // Pool size
            muzzleSize, // size particle
            Constants::Color::Red,  // color particle
            Constants::Particle::WithMass::Bullet::Speed // initial speed
        )
    });

    auto& muzzleGenerator = _gunMuzzleGeneratorAndPool.back().first;

    generator->init(_emitterOrigin);

    // Create generation policy
	ParticleGenerationPolicy muzzleGenPolicy(SpawnMode::Count, ScalarStats(1500, 200)); // 1500 particles per shot
    _meshDataMuzzle.emplace_back();
	_meshDataMuzzle.back().loadMeshFromFile("../resources/muzzle.obj");
	muzzleGenPolicy.setRegion(Region(_meshDataMuzzle.back()));
    muzzleGenPolicy.setColor(muzzleColor);
    muzzleGenerator->setGenerationPolicy(muzzleGenPolicy);

    // Create lifetime policy
    // _meshDataMuzzle.emplace_back();
    // _meshDataMuzzle.back().loadMeshFromFile("../resources/muzzleConstraint.obj");
    ParticleLifetimePolicy muzzleLifePolicy(ScalarStats(0.1, 0.02)); // 0.1 seconds lifetime
    // muzzleLifePolicy.setVolumeRegion(Region(_meshDataMuzzle.back()));
	muzzleGenerator->setLifetimePolicy(muzzleLifePolicy);
}

void GunSystem::initGunBullets()
{
    // ONE BULLET TYPE -----------------------------------------------------
    
    // Bullet size
    float size =  Constants::Particle::WithMass::Bullet::Size;
    // Bullet color
    physx::PxVec4 color = Constants::Color::Green;

    _gunBulletsGeneratorAndPool.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<Bullet>>(
            Constants::System::Gun::ReserveCountPerBulletGenerator,  // Pool size
            _camera->getTransformRelativeToCamera(_emitterOrigin.x, _emitterOrigin.y, _emitterOrigin.z), // position particle
            _camera->getDir() * Constants::Particle::WithMass::Bullet::Speed
        )
    });

    auto& generator = _gunBulletsGeneratorAndPool.back().first;

    generator->init(_emitterOrigin);

    // Create generation policy
    ParticleGenerationPolicy genPolicy;
    Region bulletRegion(RegionType::POINT_3D, Vector3Stats()); // Default region at origin
    genPolicy.setRegion(bulletRegion);
    genPolicy.setColor(ColorStats(color, physx::PxVec4(0.0f, 0.0f, 0.0f, 0.0f)));
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
    
    float stiffness = 10.0f; // how fast it catches up
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
                // std::cout << "Bullet Position: " << p->getPosition().x << ", " << p->getPosition().y << ", " << p->getPosition().z << std::endl;
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

                p->setColor(gen->getGeneratedColor());
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
    // ! IMPORTANT ! Must call base update to handle sub-systems and inside forces
	ParticleSystem::update(deltaTime);
	if (!isActive()) {
		return;
	}

    _timeSinceLastShot += deltaTime;

    updateGunMesh(deltaTime);
    updateGunBullets(deltaTime);
    if (isShooting) {
        createMuzzleFlash(deltaTime);
        isShooting = false;
    }
    updateMuzzleFlash(deltaTime);
}

void GunSystem::setRenderable(bool renderable)
{
    ParticleSystem::setRenderable(renderable);

    std::cout << "GunSystem setRenderable: " << renderable << std::endl;

    // Update visibility of gun mesh particles
    for (auto& [gen, pool] : _gunMeshGeneratorAndPool) 
    {
        auto& particles = pool->accessParticlePool();
        for (int i = 0; i < pool->getActiveCount(); ++i) 
        {
            particles[i]->setVisibility(renderable);
        }
    }
}

void GunSystem::setTransform(const physx::PxTransform &t)
{
    _emitterOrigin = t.p;
}

void GunSystem::shoot()
{
    if (_timeSinceLastShot < _cooldownTimeSinceLastShot) {
        return; // Still in cooldown
    }
    _timeSinceLastShot = 0.0;
    isShooting = true;

    physx::PxVec3 bulletDirection = _camera->getDir().getNormalized();

    auto& [gen, pool] = _gunBulletsGeneratorAndPool[currentAmmunition];

    // Spawn "new" particle (getting from the pool)
    auto* p = pool->activateParticle();

    if (p) {
        p->setTransform(_gunTransform);
        p->setVelocityDirection(bulletDirection);
        p->setColor(gen->getGeneratedColor());

        createBulletTraceForce(*p);
    }

}

void GunSystem::createBulletTraceForce(ParticleWithMass &bulletParticle)
{
    Region windRegion(Cylinder(
            bulletParticle.getPosition(),
            bulletParticle.getPosition(),
            10.0f
        )   
    ); // Cylinder region around the bullet that grows with it

    std::unique_ptr<ForceGenerator> windGen = std::make_unique<WindRegionForce>(
        this,
        windRegion,
        bulletParticle.getVelocity() // wind velocity
    );

    dynamic_cast<RegionalForce*>(windGen.get())->setFollowParticle(
        true, dynamic_cast<ParticleWithMass&>(bulletParticle)
    );

    windGen->setGroup(Constants::Group::DynamicGroup::ENEMY);
    registerForceGenAtForceManager(std::move(windGen));
}
