#pragma once

#include "RigidBody.h"

class SurferBody : public RigidBody
{
public:
    SurferBody(const physx::PxVec3& center, const std::string& filePath, float scale = 1.0f);
    virtual ~SurferBody() = default;
    virtual void createRenderableEntity(const std::string& filePath, float scale) override;

    virtual void onBeginUpdate(double deltaTime) override {};
    virtual void onEndUpdate(double deltaTime) override {};

    virtual physx::PxVec3 getDirectionForward() const;
    virtual physx::PxVec3 getDirectionRight() const;
    virtual physx::PxVec3 getDirectionUp() const;



protected:
    virtual void initiallize(const physx::PxVec3 &center);

};