#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <PxPhysicsAPI.h>
#include "Viewport.h"
#include "IInputEventListener.h"

class InputManager;

class Camera : public IInputEventListener
{
public:
	explicit Camera(Viewport* vp, const physx::PxVec3 &eye, const physx::PxVec3& dir);
	~Camera() = default;

	// ! socorro
	float getDir_X() const {
		glm::dvec3 dir = glm::normalize(mLook - mEye);
		return dir.x;
	}
	float getDir_Y() const {
		glm::dvec3 dir = glm::normalize(mLook - mEye);
		return dir.y;
	}
	float getDir_Z() const {
		glm::dvec3 dir = glm::normalize(mLook - mEye);
		return dir.z;
	}
	physx::PxVec3 getDir() const {
		return physx::PxVec3(getDir_X(), getDir_Y(), getDir_Z());
	}
	physx::PxTransform getTransformRelativeToCamera(float forwardDist, float sideOffset, float upOffset) const
	{
		physx::PxTransform cam = getTransform();
		// Local offset from camera (right, up, forward)
		physx::PxVec3 localOffset(sideOffset, upOffset, -forwardDist); // negative z is forward in PhysX
		physx::PxVec3 pos = cam.p + cam.q.rotate(localOffset);
		physx::PxQuat rot = cam.q;
		return physx::PxTransform(pos, rot);
	}
	physx::PxTransform getTransform() const
	{
		physx::PxVec3 viewY = mDir.cross(physx::PxVec3(0,1,0));

		if(viewY.normalize()<1e-6f) 
			return physx::PxTransform(physx::PxVec3(mEye.x, mEye.y, mEye.z));

		physx::PxMat33 m(mDir.cross(viewY), viewY, -mDir);
		return physx::PxTransform(physx::PxVec3(mEye.x, mEye.y, mEye.z), physx::PxQuat(m));
	}
	
	// !
	// viewPort
	Viewport const& viewPort() const { return *mViewPort; };

	// view matrix
	glm::dmat4 const& viewMat() const { return mViewMat; };

	void onMouseScroll(double dx, double dy) override;

	void set2D();
	void set3D();

	// void pitch(GLdouble a); // rotates a degrees on the X axis
	// void yaw(GLdouble a);   // rotates a degrees on the Y axis
	// void roll(GLdouble a);  // rotates a degrees on the Z axis

	// projection matrix
	glm::dmat4 const& projMat() const { return mProjMat; };

	// sets scene visible area size
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor
	void setScale(GLdouble s);

	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const;

	void moveLR(GLdouble cs); // A izquierda/A derecha
	void moveFB(GLdouble cs); // Adelante/Atrás
	void moveUD(GLdouble cs); // Arriba/Abajo

	void pitchReal(GLdouble cs); // Rotaci�n en el eje u (mitrar arriba / abajo)
	void yawReal(GLdouble cs); // Rotaci�n en el eje v (izqueda / derecha)
	void rollReal(GLdouble cs); // Rotaci�n en ele eje Z

	void orbit(GLdouble incAng, GLdouble incY);
	void setCenital();

	glm::dmat4 getProjectionMatrix() const { return mProjMat; }
	glm::dmat4 getViewMatrix() const { return mViewMat; }

	void changePrj();

	void update();

protected:
	glm::dvec3 mEye = {0.0, 0.0, 500.0}; // camera's position
	glm::dvec3 mLook = {0.0, 0.0, 0.0};  // target's position
	glm::dvec3 mUp = {0.0, 1.0, 0.0};    // the up vector

	glm::dvec3 mRight; // Vector u
	glm::dvec3 mUpward; // Vector v
	glm::dvec3 mFront; // Vector -n
	void setAxes();

	physx::PxVec3 mDir = physx::PxVec3(0, 0, -1); // forward vector // ! socorro


	glm::dmat4 mViewMat;   // view matrix = inverse of modeling matrix
	void uploadVM() const; // transfers viewMat to the GPU

	glm::dmat4 mProjMat;   // projection matrix
	void uploadPM() const; // transfers projMat to the GPU

	GLdouble xRight, xLeft, yTop, yBot;     // size of scene visible area
	GLdouble mNearVal = 1, mFarVal = 10000; // view volume
	GLdouble mScaleFact = 1;                // scale factor
	bool bOrto = true;                      // orthogonal or perspective projection

	Viewport* mViewPort; // the viewport

	void setVM();
	void setPM();

	GLdouble mAng;
	GLdouble mRadio;

	InputManager& mInputMgr;
};

#endif //_H_Camera_H_
