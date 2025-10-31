#include <PxPhysicsAPI.h>

// Linear interpolation between two PxVec3
inline physx::PxVec3 lerp(const physx::PxVec3& a, const physx::PxVec3& b, float t)
{
    return a + (b - a) * t;
}

// Spherical linear interpolation between two PxQuat
inline physx::PxQuat slerp(const physx::PxQuat& a, const physx::PxQuat& b, float t)
{
    // Clamp t to [0, 1]
    t = physx::PxClamp(t, 0.0f, 1.0f);

    float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

    // If dot < 0, negate one quaternion to take the shortest path
    physx::PxQuat end = (dot < 0.0f) ? physx::PxQuat(-b.x, -b.y, -b.z, -b.w) : b;
    dot = fabsf(dot);

    // If quaternions are close, use linear interpolation
    if (dot > 0.9995f)
    {
        physx::PxQuat result(
            a.x + t * (end.x - a.x),
            a.y + t * (end.y - a.y),
            a.z + t * (end.z - a.z),
            a.w + t * (end.w - a.w)
        );
        result.normalize();
        return result;
    }

    // Use spherical interpolation
    float theta_0 = acosf(dot);
    float theta = theta_0 * t;
    float sin_theta = sinf(theta);
    float sin_theta_0 = sinf(theta_0);

    float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;

    physx::PxQuat result(
        (a.x * s0) + (end.x * s1),
        (a.y * s0) + (end.y * s1),
        (a.z * s0) + (end.z * s1),
        (a.w * s0) + (end.w * s1)
    );
    return result;
}