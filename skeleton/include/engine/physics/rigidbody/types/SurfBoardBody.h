#pragma once

#include "RigidBody.h"

class SurfBoardBody : public RigidBody
{
public:
    SurfBoardBody(const physx::PxVec3& center, const std::string& filePath, float scale = 1.0f);
    virtual ~SurfBoardBody() = default;

    virtual void createRenderableEntity(const std::string& filePath, float scale) override;

    virtual void onBeginUpdate(double deltaTime) override {};
    virtual void onEndUpdate(double deltaTime) override {};

    virtual bool isJumping() const { return _isJumping; }
    virtual void setIsJumping(bool isJumping) { _isJumping = isJumping; }

protected:
    virtual void initiallize(const physx::PxVec3 &center);

    bool _isJumping = true;
};