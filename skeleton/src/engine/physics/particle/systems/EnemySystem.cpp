#include "EnemySystem.h"

// EnemySystem::EnemySystem(const std::string& filename, int numberOfEnemies)
//     : MeshSystem(filename)
//     , _numberOfEnemies(numberOfEnemies)
// {
// }

// EnemySystem::EnemySystem(const std::string& filename, int numberOfEnemies, float pointSize, double scale, const Vector4& color)
//     : MeshSystem(filename, pointSize, scale, color)
//     , _numberOfEnemies(numberOfEnemies)
// {
// }

// void EnemySystem::init()
// {
//     if (_numberOfEnemies <= 0) {
//         std::cerr << "EnemySystem initialized with non-positive number of enemies: " << _numberOfEnemies << std::endl;
//     } else if (_numberOfEnemies > _maxSubSystems) {
//         std::cerr << "EnemySystem initialized with number of enemies exceeding max sub-systems (" << _maxSubSystems << "): " << _numberOfEnemies << std::endl;
//         _numberOfEnemies = _maxSubSystems;
//     }

//     for (int i = 0; i < _numberOfEnemies; ++i) {
//         SpawnEnemy();
//     }
// }

// void EnemySystem::update(double deltaTime)
// {
// }

// void EnemySystem::SpawnEnemy()
// {
//     std::cout << "Spawning an enemy." << std::endl;

//     std::unique_ptr<ParticleSystem> enemy = std::make_unique<MeshSystem>(
//         _meshData,
//         _pointSize,
//         _scale,
//         _color
//     );

//     enemy->setWorldRegion(_worldRegion);
// }
