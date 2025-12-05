#define GLM_ENABLE_EXPERIMENTAL

#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "InputManager.h"

#include <iostream>


Camera::Camera(Viewport* vp)
	: mInputMgr(InputManager::getInstance())
	, mViewMat(1.0)
	, mProjMat(1.0)
	, xRight(vp->width() / 2.0)
	, xLeft(-xRight)
	, yTop(vp->height() / 2.0)
	, yBot(-yTop)
	, mViewPort(vp)
{
	setPM();
}

void
Camera::uploadVM() const
{
}

void
Camera::setVM()
{
	mViewMat = glm::lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix

	setAxes(); // Update the Axes based on the new Matrix
}

void Camera::onMouseScroll(double dx, double dy)
{
	setScale(dy * 0.1);
}

void Camera::set2D()
{
	mEye = { 0, 0, 500 };
	mLook = { 0, 0, 0 };
	mUp = { 0, 1, 0 };
	mRadio = glm::sqrt(glm::pow(mEye.z - mLook.z, 2) + glm::pow(mEye.x - mLook.x, 2));
	mAng = glm::radians(.0);
	setVM();
}

void
Camera::set3D()
{
	mEye  = {0.0f, 2.0f, 5.0f};
	mLook = {0.0f, 1.0f, 0.0f};
	mUp   = {0.0f, 1.0f, 0.0f};
	mRadio = glm::sqrt(glm::pow(mEye.z - mLook.z, 2) + glm::pow(mEye.x - mLook.x, 2));
	mAng = glm::radians(.0);
	setVM();
}

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

void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = glm::ortho(xLeft * mScaleFact,
			xRight * mScaleFact,
			yBot * mScaleFact,
			yTop * mScaleFact,
			mNearVal,
			mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else {
		// Me gusta más esto: (también está en la diapositivas)
		double aspect = mViewPort->width() / (double)mViewPort->height();
		constexpr double fovy = glm::radians(60.0);
		mProjMat = glm::perspective(fovy, aspect, mNearVal, mFarVal);

		// Que esto: 
		//mProjMat = glm::frustum(xLeft, xRight,
		//	yBot, yTop,
		//	mNearVal, mFarVal);
	}
}

void
Camera::uploadPM() const
{
	Shader::setUniform4All("projection", mProjMat);
}

void
Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}


void
Camera::setAxes()
{
	mRight = glm::row(mViewMat, 0);
	mUpward = glm::row(mViewMat, 1);
	mFront = -glm::row(mViewMat, 2);
}

void
Camera::moveLR(GLdouble cs)
{
	mEye += mRight * cs;
	mLook += mRight * cs;
	setVM();
}

void
Camera::moveFB(GLdouble cs)
{
	mEye += mFront * cs;
	mLook += mFront * cs; // Or else the mEye point would surpass the mLook and it would be behind the camera
	setVM();
}

void
Camera::moveUD(GLdouble cs)
{
	mEye += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}
void
Camera::changePrj()
{
	bOrto = !bOrto;
	setPM(); // Vuelve a calcular mProjMat según el tipo de vista
}

void Camera::update()
{
	if (mInputMgr.isMousePressed(MouseButton::Left)) {
		std::cout << "Left mouse pressed\n";
		glm::vec2 mouseDelta = mInputMgr.getMouseDelta();
		moveUD(-mouseDelta.y);
		moveLR(mouseDelta.x);
	}
	if (mInputMgr.isMousePressed(MouseButton::Right)) {
		std::cout << "Right mouse pressed\n";
		glm::vec2 mouseDelta = mInputMgr.getMouseDelta();
		orbit(-mouseDelta.x, 0.0);
	}
	if (mInputMgr.isKeyPressed(KeyCode::W)) {
		moveFB(5.0);
	}
	if (mInputMgr.isKeyPressed(KeyCode::S)) {
		moveFB(-5.0);
	}
	if (mInputMgr.isKeyPressed(KeyCode::A)) {
		moveLR(-5.0);
	}
	if (mInputMgr.isKeyPressed(KeyCode::D)) {
		moveLR(5.0);
	}
}

void 
Camera::pitchReal(GLdouble cs)
{
	GLdouble a = cs;

	// Rotate around mRight u(x)
	mLook = mEye + glm::rotate(mLook - mEye, glm::radians(a), mRight);
	mUp = glm::rotate(mUp, glm::radians(a), mRight);

	setVM(); // Update the view matrix and axes
}

void 
Camera::yawReal(GLdouble cs)
{
	GLdouble a = cs;

	// Rotate around mUpward v(y)
	mLook = mEye + glm::rotate(mLook - mEye, glm::radians(a), mUpward);
	mUp = glm::rotate(mUp, glm::radians(a), mUpward);

	setVM(); // Update the view matrix and axes
}

void 
Camera::rollReal(GLdouble cs)
{
	GLdouble a = cs;

	mUp = glm::rotate(mUp, glm::radians(a), mFront);

	setVM();
}

void Camera::orbit(GLdouble incAng, GLdouble incY)
{
	mRadio = glm::sqrt(glm::pow(mEye.z - mLook.z, 2) + glm::pow(mEye.x - mLook.x, 2));

	mAng += incAng;
	mEye.x = mLook.x + cos(glm::radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(glm::radians(mAng)) * mRadio;
	mEye.y += incY;

	setVM();
}

void Camera::setCenital()
{
	mEye = { 0, 700, 0 };
	mLook = { 0, 0, 0 };
	mUp = { 1, 0, 0 };

	setVM();
}
