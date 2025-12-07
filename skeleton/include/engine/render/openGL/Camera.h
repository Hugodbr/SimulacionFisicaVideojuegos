#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

#include <PxPhysicsAPI.h>
#include "Viewport.h"
#include "IInputEventListener.h"

class InputManager;

enum ProjectionType {
	ORTHOGRAPHIC,
	PERSPECTIVE
};

class Camera : public IInputEventListener
{
public:
	explicit Camera(
		Viewport* vp, 
		const physx::PxVec3& eye, // camera position
		const physx::PxVec3& look // camera look target
	);

	~Camera() = default;

	void update(double deltaTime);
	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const;

	Viewport const& viewPort() const { return *mViewPort; };
	// Projection matrix - converts view space to clip space
	glm::dmat4 const& projMat() const { return mProjMat; };
	// View matrix = camera transformation matrix
	glm::dmat4 const& viewMat() const { return mViewMat; };


	// Sets the camera to a 2D orthographic view
	void set2D();
	// Sets the camera to a 3D perspective view
	void set3D();
	// Toggles the projection type between orthographic and perspective
	void changeProjection();

	void setCenital(); // TODO possible conflict with changeProjection

	// Strafe movements
	void moveLR(GLdouble cs); // Left/Right
	void moveFB(GLdouble cs); // Forward/Backward
	void moveUD(GLdouble cs); // Up/Down

	void pitch(GLdouble a); // rotates a degrees on the X axis
	void yaw(GLdouble a);   // rotates a degrees on the Y axis
	void roll(GLdouble a);  // rotates a degrees on the Z axis

	void orbitate(GLdouble period); // period in frame units
	void orbit(GLdouble dx, GLdouble dy); // mouse movement deltas

	// ! socorro
	float getDir_X() const { return mForward.x; }
	float getDir_Y() const { return mForward.y; } 
	float getDir_Z() const { return mForward.z; }

	physx::PxVec3 getDir() const {
		return physx::PxVec3(getDir_X(), getDir_Y(), getDir_Z());
	}

	physx::PxTransform getTransformRelativeToCamera(float forwardDist, float sideOffset, float upOffset) const
	{
		std::cout << "Get transform not implemented yet in Camera class.\n";
		physx::PxTransform cam = getTransform();
		// Local offset from camera (right, up, forward)
		physx::PxVec3 localOffset(sideOffset, upOffset, -forwardDist); // negative z is forward in PhysX
		physx::PxVec3 pos = cam.p + cam.q.rotate(localOffset);
		physx::PxQuat rot = cam.q;
		return physx::PxTransform(pos, rot);
	}

	physx::PxTransform getTransform() const
	{
		// physx::PxVec3 viewY = mDir.cross(physx::PxVec3(0,1,0));

		// if(viewY.normalize()<1e-6f) 
		// 	return physx::PxTransform(physx::PxVec3(mEye.x, mEye.y, mEye.z));

		// physx::PxMat33 m(mDir.cross(viewY), viewY, -mDir);
		// return physx::PxTransform(physx::PxVec3(mEye.x, mEye.y, mEye.z), physx::PxQuat(m));
		return physx::PxTransform();
	}
	
	// // !

	// TODO METHODS:
	void onMouseScroll(double dx, double dy) override;
	// sets scene visible area size
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor
	void setScale(GLdouble s);



protected:
	InputManager& mInputMgr;

	Viewport* mViewPort; // the viewport

	glm::dvec3 mEye  = {0.0, 0.0, 5.0};  // camera's position
	glm::dvec3 mLook = {0.0, 0.0, 0.0};  // target's position
	glm::dvec3 mUp   = {0.0, 1.0, 0.0};  // the up vector (+Y)

	glm::dvec3 mEye_2_3D  = mEye;  // camera's position
	glm::dvec3 mLook_2_3D = mLook;  // target's position
	glm::dvec3 mUp_2_3D   = mUp;  // the up vector (+Y)

	glm::dvec3 mEye_Cenital  = {0.0, 500.0, 0.0};  // camera's position
	glm::dvec3 mLook_Cenital = {0.0, 0.0  , 0.0};  // target's position
	glm::dvec3 mUp_Cenital   = {1.0, 0.0  , 0.0};  // the up vector (+X)

	glm::dvec3 mRight;  // Vector  u
	glm::dvec3 mUpward; // Vector  v
	glm::dvec3 mForward;  // Vector -n

	glm::dmat4 mProjMat;   // projection matrix
	void setPM(); 		   // sets projection matrix according to parameters
	void uploadPM() const; // transfers projMat to the GPU

	glm::dmat4 mViewMat;   // view matrix = inverse of modeling matrix
	// Needs to be called after modifying mEye, mLook, or mUp. Calls setAxes().
	void setVM();		   // sets view matrix (LookAt matrix)
	void uploadVM() const; // transfers viewMat to the GPU
	
	// Sets the camera axes based on the view matrix. Called after modifying the view matrix, i.e., after rotations or movements that change the camera's orientation and position.
	void setAxes(); 		

	// Projection parameters
	GLdouble xRight, xLeft, yTop, yBot;          // size of scene visible area
	GLdouble mNearVal = 0.1, mFarVal = 1000.0;   // view volume
	GLdouble mScaleFact = 1.0;                   // scale factor
	ProjectionType mProjType = PERSPECTIVE;       // orthogonal or perspective projection. Default: perspective
	GLdouble FOV = 45.0;                     // field of view for perspective projection

	bool mOrbiting = false; // whether the camera is orbiting or not
};

#endif //_H_Camera_H_ 