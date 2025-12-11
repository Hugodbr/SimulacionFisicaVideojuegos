#define GLM_ENABLE_EXPERIMENTAL

#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "InputManager.h"

#include <iostream>


Camera::Camera(
	Viewport* vp, 
	const physx::PxVec3& eye, 
	const physx::PxVec3& look
)
	: mInputMgr(InputManager::getInstance())
	, mViewMat(1.0)
	, mProjMat(1.0)
	, xRight(vp->width() / 2.0)
	, xLeft(-xRight)
	, yTop(vp->height() / 2.0)
	, yBot(-yTop)
	, mViewPort(vp)
	, mEye(glm::dvec3(eye.x, eye.y, eye.z))
	, mLook(glm::dvec3(look.x, look.y, look.z))
{
	setVM();
	setPM();
}

void Camera::setVM()
{
	mViewMat = glm::lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix

	setAxes(); // Update the Axes based on the new Matrix
}

void Camera::setAxes()
{
	mRight   =  glm::normalize(glm::dvec3(glm::row(mViewMat, 0)));
	mUpward  =  glm::normalize(glm::dvec3(glm::row(mViewMat, 1)));
	mForward = -glm::normalize(glm::dvec3(glm::row(mViewMat, 2))); 
}

void Camera::setPM()
{
	switch (mProjType)
	{
	case ORTHOGRAPHIC:
		mProjMat = glm::ortho( // glm::ortho defines the orthogonal projection matrix
			xLeft  * mScaleFact,
			xRight * mScaleFact,
			yBot   * mScaleFact,
			yTop   * mScaleFact,
			mNearVal,
			mFarVal
		);
		
		break;
	case PERSPECTIVE:
		double aspect = mViewPort->width() / (double)mViewPort->height();
		double fovy = glm::radians(FOV);

		mProjMat = glm::perspective(
			fovy, 
			aspect, 
			mNearVal, 
			mFarVal
		);

		// Also:
		/* mProjMat = glm::frustum(xLeft, xRight,
			yBot, yTop,
			mNearVal, mFarVal); 
		*/
	}
}

void Camera::changeProjection()
{
	mProjType = (mProjType == ORTHOGRAPHIC) ? PERSPECTIVE : ORTHOGRAPHIC;
	setPM();
}

void Camera::set2D()
{
	mEye  = mEye_2_3D;
	mLook = mLook_2_3D;
	mUp   = mUp_2_3D;

	mProjType = ORTHOGRAPHIC;
	setPM();
}

void Camera::set3D()
{
	mEye  = mEye_2_3D;
	mLook = mLook_2_3D;
	mUp   = mUp_2_3D;

	mProjType = PERSPECTIVE;
	setPM();
}

void Camera::setCenital()
{
	// Save current 2D/3D view parameters
	mEye_2_3D  = mEye;
	mLook_2_3D = mLook;
	mUp_2_3D   = mUp;

	mEye  = mEye_Cenital;
	mLook = mLook_Cenital;
	mUp   = mUp_Cenital;

	std::cout << "Reminder: Cenital position changes are not stored.\n";

	setVM();
}

//===============================================================================
// Upload methods
//===============================================================================
void Camera::uploadVM() const
{
	Shader::setUniform4All("viewMat", mViewMat);
}

void Camera::uploadPM() const
{
	Shader::setUniform4All("projMat", mProjMat);
}

void Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}

//===============================================================================
// Update method
//===============================================================================
void Camera::update(double deltaTime)
{
	// std::cout << "Updating camera...\n";

	if (mInputMgr.isKeyPressed(KeyCode::O)) {
		mOrbiting = !mOrbiting;
		// std::cout << "Orbiting toggled to " << (mOrbiting ? "ON" : "OFF") << std::endl;
	}
	if (mOrbiting) {
		orbitate(100000.0 * deltaTime); // Orbit with a period of 60 frames
	}
	
	// Is the left mouse button is already pressed when it`s time to update, the camera orbits according to mouse movement delta stored at InputManager
	if( /*!mInputMgr.isMousePressedThisFrame(MouseButton::Left) && */ mInputMgr.isMousePressed(MouseButton::Left) ) {
		glm::vec2 mouseDelta = mInputMgr.getMouseDelta();
		orbit(mouseDelta.x, mouseDelta.y);
		// std::cout << "Orbiting with mouse drag: (" << mouseDelta.x << ", " << mouseDelta.y << ")" << std::endl;
	}

	// if( mInputMgr.isMousePressed(MouseButton::Left) ) {
	// 	std::cout << "PRESSING" << std::endl;
	// }

	// if (mInputMgr.isMousePressed(MouseButton::Left)) {
	// 	std::cout << "Left mouse pressed" << std::endl;
	// 	glm::vec2 mouseDelta = mInputMgr.getMouseDelta();
	// 	orbit(mouseDelta.x, mouseDelta.y);
	// }
	// if (mInputMgr.isMousePressed(MouseButton::Right)) {
	// 	std::cout << "Right mouse pressed" << std::endl;
	// 	glm::vec2 mouseDelta = mInputMgr.getMouseDelta();
	// 	moveUD(-mouseDelta.y);
	// 	moveLR(mouseDelta.x);
	// }
	// if (mInputMgr.isKeyPressed(KeyCode::W)) {
	// 	moveFB(5.0);
	// }
	// if (mInputMgr.isKeyPressed(KeyCode::S)) {
	// 	moveFB(-5.0);
	// }
	// if (mInputMgr.isKeyPressed(KeyCode::A)) {
	// 	moveLR(-5.0);
	// }
	// if (mInputMgr.isKeyPressed(KeyCode::D)) {
	// 	moveLR(5.0);
	// }

}

//===============================================================================
// Movement methods
//===============================================================================
void Camera::moveLR(GLdouble cs)
{
	mEye  += mRight * cs;
	mLook += mRight * cs;
	setVM();
}

void Camera::moveFB(GLdouble cs)
{
	mEye  += mForward * cs;
	mLook += mForward * cs; // Or else the mEye point would surpass the mLook and it would be behind the camera
	setVM();
}

void Camera::moveUD(GLdouble cs)
{
	mEye  += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}

void Camera::pitch(GLdouble cs)
{
	// Rotate around mRight u(x)
	mLook = mEye + glm::rotate(mLook - mEye, glm::radians(cs), mRight);
	mUp = glm::rotate(mUp, glm::radians(cs), mRight);

	setVM(); // Update the view matrix and axes
}

void Camera::yaw(GLdouble cs)
{
	// Rotate around mUpward v(y)
	mLook = mEye + glm::rotate(mLook - mEye, glm::radians(cs), mUpward);
	mUp = glm::rotate(mUp, glm::radians(cs), mUpward);

	setVM(); // Update the view matrix and axes
}

void Camera::roll(GLdouble cs)
{
	mUp = glm::rotate(mUp, glm::radians(cs), mForward);

	setVM(); // Update the view matrix and axes
}

void Camera::orbitate(GLdouble period)
{
	GLdouble radius = glm::length(mEye - mLook);
	GLdouble teta = (glm::pi<GLdouble>() * 2.0 * radius) / period; // angle increment per time unit

	glm::dvec3 dist = mEye - mLook;
	dist = glm::rotate(dist, teta, mUp);

	mEye = mLook + dist;

	setVM();
}

void Camera::orbit(GLdouble dx, GLdouble dy)
{
	glm::dvec3 orbitDir = -(mUpward * dy + mRight * dx); // Invert direction for natural feel
	
	GLdouble radius = glm::length(mEye - mLook); // Distance from eye to look constant during orbit

	mEye = mLook + glm::normalize((mEye - mLook) + orbitDir) * radius; // New eye position after orbiting

	// New up vector calculation
	glm::dvec3 upPosPrev = (mEye - mLook) + mUp;

	mUp = glm::normalize(upPosPrev - glm::dot(upPosPrev, glm::normalize(mEye - mLook)) * glm::normalize(mEye - mLook));

	setVM();
}



// ===============================================================================

void Camera::onMouseScroll(double dx, double dy)
{
	mEye  += mForward * dy * 0.5; // Zoom in/out
	// mLook += mForward * dy * 0.5;
	setVM();
}

// TODO METHODS:
void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}