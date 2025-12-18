#include <iostream>
#include "Entity.h"
#include "app/GameApp.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void Abs_Entity::upload(const mat4& modelViewMat) const
{
	// mShader->setUniform("modelView", modelViewMat);
}

Abs_Entity::~Abs_Entity()
{
	delete mMesh;
	mMesh = nullptr;
}

void Abs_Entity::load()
{
	mMesh->load();
}

void Abs_Entity::unload()
{
	mMesh->unload();
}

glm::vec3 Abs_Entity::getDimensions() const
{
	if (mMesh) {
		return mMesh->getDimensions();
	} else {
		std::cout << "Warning: Mesh is null in Abs_Entity::getDimensions()" << std::endl;
		return glm::vec3(0.0f);
	}
}

glm::vec3 Abs_Entity::getWPos()
{
	return mWorldPosition;
}

void Abs_Entity::setWPos(const glm::vec3& position)
{
	setPose(position, mWorldRotation);
}

void Abs_Entity::setWPos(float x, float y, float z)
{
	setWPos(glm::vec3(x, y, z));
}

void Abs_Entity::setWQuat(float w, float x, float y, float z)
{
	setPose(mWorldPosition, glm::quat(w, x, y, z));
}

void Abs_Entity::setPose(const glm::vec3 &position, const glm::quat &rotation)
{
	if (position != mWorldPosition) {
    	mWorldPosition = position;
    	mWorldPosMat = glm::translate(glm::mat4(1.0f), mWorldPosition);
		// std::cout << "Entity position set to: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
	}
	if (rotation != mWorldRotation) {
		mWorldRotation = glm::normalize(rotation);
    	mWorldRotMat = glm::mat4_cast(mWorldRotation);
	}
	setModelMat(mWorldPosMat * mWorldRotMat);
}

/* ---------------------ENTITY WITH COLORS--------------------- */

EntityWithColors::EntityWithColors()
{
	mShader = Shader::get("vcolors");
}

void
EntityWithColors::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication

		mShader->use();
		upload(aMat);

		mMesh->render();
	}
}


/* ---------------------SINGLE COLOR ENTITY--------------------- */

SingleColorEntity::SingleColorEntity(const glm::vec4& color)
	: mColor(color)
{
	mShader = Shader::get("simple_light");
}

void
SingleColorEntity::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication

		mShader->use();
		mShader->setUniform("color", (glm::vec4)mColor);
		mShader->setUniform("lightDir", glm::vec4(modelViewMat * vec4(-1, -1, -1, 0)));
		upload(aMat);

		mMesh->render();
	}
}

glm::vec4 SingleColorEntity::getColor() const {
	return mColor;
}

void SingleColorEntity::setColor(const glm::vec4& color) {
	mColor = color;
}


/* ---------------------ENTITY WITH TEXTURE--------------------- */

EntityWithTexture::EntityWithTexture(Texture* texture, bool modulate) : mModulate(modulate)
{
	mShader = Shader::get("texture");
	setTexture(texture);
}

void EntityWithTexture::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();

		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		if (mTexture != nullptr) mTexture->bind();

		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind();
	}
}


/* ---------------------COMPOUND ENTITY--------------------- */

CompoundEntity::~CompoundEntity()
{
	for (Abs_Entity* el : gObjects)
		delete el;
}

void CompoundEntity::render(const glm::mat4& modelViewMat) const
{
	/*
	* Se multiplica la matriz modelViewMat recibida por la matriz de modelado que la entidad compuesta
	* tiene por ser una entidad
	*/
	mat4 aMat = modelViewMat * mModelMat;

	for (Abs_Entity* e : gObjects) {
		e->render(aMat);
	}
}

void CompoundEntity::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();
}

void CompoundEntity::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();
}

void CompoundEntity::addEntity(Abs_Entity* ae)
{
	gObjects.push_back(ae);
}


/* ---------------------ENTITY WITH MATERIAL--------------------- */

EntityWithMaterial::EntityWithMaterial(const glm::vec4& color)
	: SingleColorEntity(color)
	, mMaterial(color)
{
	mShader = Shader::get("light");
	mMaterial = Material(glm::vec3(color.r, color.g, color.b), 32.0f);
	mMaterial.setSeaBlue();
}

void EntityWithMaterial::render(const glm::mat4& modelViewMat) const
{
	if (!mMesh) {
		std::cout << "Warning: Mesh is null in EntityWithMaterial::render()" << std::endl;
		return;
	}

	mat4 model = mModelMat;
	mat4 view  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().viewMat();
	mat4 proj  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().projMat();
	
	assert(mShader == Shader::get("light"));

	mShader->use();
	mShader->setUniform("modelMat", model);
	mShader->setUniform("viewMat", view);
	mShader->setUniform("projMat", proj);	
	mMaterial.upload(*mShader);

	glEnable(GL_DEPTH_TEST);
	mMesh->render();
}

void EntityWithMaterial::toggleShowNormals()
{
	mShowNormals = !mShowNormals;
}

void EntityWithMaterial::renderNormals(const glm::mat4& modelViewMat) const
{
	mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication

	Shader* nShader = Shader::get("normals");
	nShader->use();
	nShader->setUniform("modelView", aMat);

	mMesh->render();
}


/* ---------------------COLOR MATERIAL ENTITY--------------------- */

// Subclase trivial = sin nada
ColorMaterialEntity::ColorMaterialEntity(const glm::vec4& color)
	: EntityWithMaterial(color)
{
}

/* ---------------------MULTI TEXTURE ENTITY--------------------- */
EntityWithMultiTexture::EntityWithMultiTexture()
{
	mShader = Shader::get("pbr");
}

void EntityWithMultiTexture::render(const glm::mat4& modelViewMat) const 
{
	if (!mMesh) {
		std::cout << "Warning: Mesh is null in EntityWithMultiTexture::render()" << std::endl;
		return;
	}

	mat4 model = mModelMat;
	mat4 view  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().viewMat();
	mat4 proj  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().projMat();
	
	mShader->use();
	mShader->setUniform("model", model);
	mShader->setUniform("view", view);
	mShader->setUniform("projection", proj);

	// send light direction IN VIEW SPACE
	glm::vec3 lightDirWorld = glm::normalize(glm::vec3(-1, -1, -1));
	glm::vec3 lightDirView  = glm::mat3(view) * lightDirWorld;
	mShader->setUniform("lightDirView", lightDirView);

	if (mPBRMaterial) {
		mPBRMaterial->bindAll(mShader->getProgram());
		mMesh->render();
		mPBRMaterial->unbindAll(); 
	} 
	else {
		std::cout << "Warning: PBRMaterial is null in EntityWithMultiTexture::render()" << std::endl;
	}
}