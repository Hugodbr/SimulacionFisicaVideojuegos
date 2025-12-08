// #pragma once

// #include "PxPhysicsAPI.h"
// #include "Geometry.h"

// class PhysicsEngine;

// class PhysxGeometry : public virtual Geometry
// {
// public:
//     PhysxGeometry();
//     virtual ~PhysxGeometry() = 0;

//     static std::unique_ptr<Geometry> createBoxFromPhysxGeometry(float length);
//     static std::unique_ptr<Geometry> createSphereFromPhysxGeometry(float radius);

// protected:
//     PhysicsEngine* _physicsEngine = nullptr;
//     physx::PxGeometryType::Enum _physxGeoType = physx::PxGeometryType::eINVALID;
// };

// class PhysxSphereGeometry : public virtual SphereGeometry, public virtual PhysxGeometry
// {
// public:
//     PhysxSphereGeometry(float radius);
//     virtual ~PhysxSphereGeometry() = default;

//     float getRadius() { return _radius; };

//     static const inline physx::PxGeometryType::Enum _physxGeoType = physx::PxGeometryType::eSPHERE;

// protected:
// };

// class PhysxBoxGeometry : public virtual BoxGeometry, public virtual PhysxGeometry
// {
// public:
//     PhysxBoxGeometry(float length);
//     virtual ~PhysxBoxGeometry() = default;

//     float getLength() { return _length; };

//     static const inline physx::PxGeometryType::Enum _physxGeoType = physx::PxGeometryType::eBOX;
    
// protected:
// };
