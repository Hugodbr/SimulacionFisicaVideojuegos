#pragma once

#include "RigidBody.h"

class BoxBody : public RigidBody
{
public:
    BoxBody(const physx::PxVec3& center, const std::string& filePath, float scale = 1.0f);
    BoxBody(const physx::PxVec3& center, const std::string& filePath, double mass, float scale = 1.0f);
    BoxBody(const physx::PxVec3& center, const std::string& filePath, float density, float scale = 1.0f);
    BoxBody(const physx::PxVec3& center, std::shared_ptr<Abs_Entity> renderableEntity);
    BoxBody(const physx::PxVec3& center, std::shared_ptr<Abs_Entity> renderableEntity, float density);
    virtual ~BoxBody() = default;

    virtual void createRenderableEntity(const std::string& filePath, float scale) override;

    virtual void onBeginUpdate(double deltaTime) override {};
    virtual void onEndUpdate(double deltaTime) override {};

protected:
    virtual void initiallize(const physx::PxVec3 &center);
};