#pragma once

namespace Constants
{
	enum ParticleType {
		DEFAULT,
		CANNON_BALL,
		BULLET,
	};

	enum Integration_Method {
		EULER,
		EULER_SEMI_IMPLICIT,
		VERLET
	};

	namespace Particle {

		namespace Default {
			static const ParticleType type = DEFAULT;

		};

		namespace CannonBall{
			static const ParticleType type = CANNON_BALL;

		};

		namespace BulletBall{
			static const ParticleType type = BULLET;

		};
	};

};

