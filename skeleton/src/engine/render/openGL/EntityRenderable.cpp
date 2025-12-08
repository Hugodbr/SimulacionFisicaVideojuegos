#include "EntityRenderable.h"
#include "app/GameApp.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "app/GameApp.h"
#include "Camera.h"
#include <iostream>

using namespace glm;


RGBAxes::RGBAxes(GLdouble l)
{
	mShader = Shader::get("vcolors");
	mMesh = Mesh::createRGBAxes(l);
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r, const glm::vec4& color)
	: SingleColorEntity(color)
{
	mMesh = Mesh::generateRegularPolygon(num, r);
}


/* RGB TRIANGLE -----------------------------------------------------------------*/

RGBTriangle::RGBTriangle(GLdouble r)
	: EntityWithColors()
{
	constexpr int num = 3;

	mMesh = Mesh::createRGBTriangle(r);
}

void
RGBTriangle::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE); // Activa el renderizado solo para las caras visibles para la c?mara

		glCullFace(GL_BACK); // Tambi?n se puede hacer: glFrontFace(GL_CCW);  
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		glCullFace(GL_FRONT); // Tambi?n se puede hacer: glFrontFace(GL_CW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		glDisable(GL_CULL_FACE);

	}
}

void
RGBTriangle::update()
{
	setWPos({ 0,0,0 });
	setModelMat(glm::rotate(modelMat(), radians(-5.0f), vec3(0, 0, 1)));

	setWPos({ glm::cos(orbit_angle) * 200, glm::sin(orbit_angle) * 200, 0 });
	orbit_angle += 0.2;

}


/* RGB RECTANGLE -----------------------------------------------------------------*/

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
}

void
RGBRectangle::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE); // Activa el renderizado solo para las caras visibles para la c?mara

		glCullFace(GL_BACK); // Tambi?n se puede hacer: glFrontFace(GL_CCW); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		glCullFace(GL_FRONT); // Tambi?n se puede hacer: glFrontFace(GL_CW); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		glDisable(GL_CULL_FACE);

	}
}


/* SINGLE COLOR CUBE -----------------------------------------------------------------*/

Cube::Cube(GLdouble length, const glm::vec4& color)
	: SingleColorEntity(color)
{
	mMesh = Mesh::generateCube(length);
}

void
Cube::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		// mMesh->render();
		// glEnable(GL_CULL_FACE); // Activa el renderizado solo para las caras visibles para la c?mara

		//Error anterior con los vértices en sentido horario:
		// glCullFace(GL_FRONT); // Tambi?n se puede hacer: glFrontFace(GL_CCW); 
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		//Error anterior con los vértices en sentido horario:
		//glCullFace(GL_BACK); // Tambi?n se puede hacer: glFrontFace(GL_CW); 
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // FILL to restore default
		glDisable(GL_CULL_FACE);

	}
}


/* RBG COLOR CUBE -----------------------------------------------------------------*/

RGBCube::RGBCube(GLdouble length)
{
	mMesh = Mesh::generateRGBCubeTriangles(length);
}

void
RGBCube::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE); // Activa el renderizado solo para las caras visibles para la c?mara

		//Error anterior con los vértices en sentido horario:
		// glCullFace(GL_FRONT); // Tambi?n se puede hacer: glFrontFace(GL_CCW); 
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		//Error anterior con los vértices en sentido horario:
		//glCullFace(GL_BACK); // Tambi?n se puede hacer: glFrontFace(GL_CW); 
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to default orelse leaks to other scenes
		glDisable(GL_CULL_FACE);
	}
}


/* GROUND -----------------------------------------------------------------*/

Ground::Ground(Texture* texture, bool modulate, GLdouble w, GLdouble h, GLuint rw, GLuint rh) : EntityWithTexture(texture, modulate)
{
	mMesh = Mesh::generateRectangleTexCor(w, h, rw, rh);
	setModelMat(glm::rotate(modelMat(), radians(90.0f), vec3(1, 0, 0)));
}

void Ground::render(const glm::mat4& modelViewMat) const
{
	EntityWithTexture::render(modelViewMat);
}


/* BOX OUT LINE -----------------------------------------------------------------*/

BoxOutline::BoxOutline(Texture* texture, Texture* iteriorTexture, bool modulate, GLdouble length)
	: EntityWithTexture(texture, modulate), mIteriorTexture(iteriorTexture)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	//setModelMat(glm::translate(glm::mat4(1), glm::vec3(0, length / 2, 100)));
}

void BoxOutline::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();

		glEnable(GL_CULL_FACE); // Activa el renderizado solo para las caras visibles para la c?mara

		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		if (mTexture != nullptr) mTexture->bind();

		//Error anterior con los vértices en sentido horario:
		// glCullFace(GL_FRONT); // Tambi?n se puede hacer: glFrontFace(GL_CCW); 
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind();

		if (mIteriorTexture != nullptr) mIteriorTexture->bind();

		//Error anterior con los vértices en sentido horario:
		//glCullFace(GL_BACK); // Tambi?n se puede hacer: glFrontFace(GL_CW); 
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		if (mIteriorTexture != nullptr) mIteriorTexture->unbind();

		glDisable(GL_CULL_FACE);

	}
}


/* STAR 3D -----------------------------------------------------------------*/

Star3D::Star3D(Texture* texture, bool modulate, GLdouble re, GLuint np, GLdouble h)
	: EntityWithTexture(texture, modulate)
{
	mMesh = Mesh::generateStar3DTexCor(re, np, h);
}

void
Star3D::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("modulate", mModulate);

		upload(aMat);

		if (mTexture != nullptr) mTexture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();
		if (mTexture != nullptr) mTexture->unbind();

		aMat = glm::scale(aMat, vec3(1, 1, -1));
		upload(aMat);

		if (mTexture != nullptr) mTexture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();
		if (mTexture != nullptr) mTexture->unbind();
	}
}

void Star3D::update()
{
	setModelMat(glm::rotate(modelMat(), 0.1f, vec3(0, 0, 1)));
}


/* GLASS PARAPET -----------------------------------------------------------------*/

GlassParapet::GlassParapet(Texture* texture, bool modulate, GLdouble length)
	: EntityWithTexture(texture, modulate)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
}

void GlassParapet::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication

		mShader->use();

		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (mTexture != nullptr) mTexture->bind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

/* BOX COVER -----------------------------------------------------------------*/

BoxCover::BoxCover(Texture* texture, Texture* iteriorTexture, bool modulate, GLdouble length)
	: EntityWithTexture(texture, modulate), mIteriorTexture(iteriorTexture), mLength(length)
{
	mMesh = Mesh::generateRectangleTexCor(length, length, 1, 1);

	mAngle = glm::radians(90.0f);
	mCurrAngle = mAngle;
	mRotSpeed = -0.01f;
}

void BoxCover::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();

		glEnable(GL_CULL_FACE); // Activa el renderizado solo para las caras visibles para la c?mara

		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		if (mTexture != nullptr) mTexture->bind();

		//Error anterior con los vértices en sentido horario:
		// glCullFace(GL_FRONT); // Tambi?n se puede hacer: glFrontFace(GL_CCW); 
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind();

		if (mIteriorTexture != nullptr) mIteriorTexture->bind();

		//Error anterior con los vértices en sentido horario:
		//glCullFace(GL_BACK); // Tambi?n se puede hacer: glFrontFace(GL_CW); 
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		if (mIteriorTexture != nullptr) mIteriorTexture->unbind();

		glDisable(GL_CULL_FACE);
	}
}

void BoxCover::update()
{
	if (mCurrAngle < -glm::pi<float>() / 4 || mCurrAngle > mAngle) {
		mRotSpeed = -mRotSpeed;
	}
	mCurrAngle += mRotSpeed;

	glm::mat4 trans1 = glm::translate(glm::mat4(1), glm::vec3(0, mLength / 2, 0)); // ajuste del pivot
	glm::mat4 rot = glm::rotate(glm::mat4(1), mCurrAngle, vec3(1, 0, 0)); // rota alrededor del pivot

	glm::mat4 trans2 = glm::translate(glm::mat4(1), mWorldPosition); // translada a la posicion final
	glm::mat4 trans3 = glm::translate(glm::mat4(1), glm::vec3(0, -mLength / 2, 0)); // vuelve ajuste del pivot

	setModelMat(trans3 * trans2 * rot * trans1);
}


/* GRASS -----------------------------------------------------------------*/

Grass::Grass(Texture* texture, bool modulate, GLdouble w, GLdouble h)
	: EntityWithTexture(texture, modulate)
{
	mShader = Shader::get("texture:texture_alpha");
	mMesh = Mesh::generateRectangleTexCor(w, h, 1, 1);
}

void Grass::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("modulate", mModulate);

		float alpha = 0;

		if (mTexture != nullptr) mTexture->bind();

		for (int i = 0; i < 3; ++i)
		{
			aMat = glm::rotate(aMat, alpha, glm::vec3(0, 1, 0));

			upload(aMat);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mMesh->render();

			alpha += (glm::pi<float>() * 2) / 8;
		}

		if (mTexture != nullptr) mTexture->unbind();
	}
}


/* INDEX MESHES -----------------------------------------------------------------*/

Toros::Toros(GLdouble R, GLdouble r, GLuint nPoints, GLuint nSamples, const glm::vec4& color)
	: ColorMaterialEntity(color)
{
	std::vector<glm::vec2> profile(nPoints + 1);

	double delta = 2 * glm::pi<double>() / static_cast<double>(nPoints);
	double teta = glm::pi<double>() / 2.0;

	// i <= nPoints to emplace the first vertex again
	for (int i = 0; i <= nPoints; ++i) {
		profile[i] = { r * cos(teta) + R, r * sin(teta) };
		teta -= delta;
	}

	mMesh = IndexMesh::generateByRevolution(profile, nSamples, 2 * glm::pi<double>());
}

IndexedBox::IndexedBox(GLdouble length, const glm::vec4& color)
	: ColorMaterialEntity(color)
{
	mMesh = IndexMesh::generateIndexedBox(length);
}

void IndexedBox::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication

		assert(mShader == Shader::get("light"));
		mShader->use();
		// mShader->setUniform("time",static_cast<float>(GameApp::getInstance().getAbsTime()));
		upload(aMat); // = mShader->setUniform("modelView", aMat);
		mMaterial.upload(*mShader);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mMesh->render();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

IndexedMesh::IndexedMesh(const std::string& filePath, const glm::vec4& color)
	: ColorMaterialEntity(color)
{
	mMesh = IndexMesh::loadMeshWithAssimp(filePath, 30.0f);
}

Sphere::Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians, const glm::vec4& color)
	: ColorMaterialEntity(color)
{
	std::vector<glm::vec2> profile(nParallels + 1); // So the circunference is complete we need to add an extra vertex

	double delta = glm::pi<double>() / static_cast<double>(nParallels);
	double teta = glm::pi<double>() / 2.0;

	for (int i = 0; i <= nParallels; ++i) {
		profile[i] = { radius * cos(teta), radius * sin(teta) };
		teta -= delta;
	}

	mMesh = IndexMesh::generateByRevolution(profile, nMeridians, 2 * glm::pi<double>());
}

Disk::Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples, const glm::vec4& color)
	: ColorMaterialEntity(color)
{
	std::vector<glm::vec2> profile(nRings);

	double dist = (R - r) / nRings;

	for (int i = 0; i < nRings; ++i) {
		profile[i] = { r + i * dist, 0 };
	}

	mMesh = IndexMesh::generateByRevolution(profile, nSamples, 2 * glm::pi<double>());
}

Cone::Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples, const glm::vec4& color)
	: ColorMaterialEntity(color)
{
	int nVetexes = nRings + 2;
	// La R es la parte de arriba y la r la de abajo
	std::vector<glm::vec2> profile(nVetexes);

	profile[0] = { 0, 0 };

	double distX = (R - r) / nRings; // Orden de la resta importante
	double distY = h / (nRings - 1);

	for (int i = 0; i < nRings; ++i) {
		profile[i + 1] = { r + i * distX, i * distY };
	}

	profile[nVetexes - 1] = { 0, h };

	std::reverse(profile.begin(), profile.end());

	mMesh = IndexMesh::generateByRevolution(profile, nSamples, 2 * glm::pi<double>());
}


/* COMPOUND ENTITIES -----------------------------------------------------------------*/

WingAdvancedTIE::WingAdvancedTIE()
	: EntityWithTexture(new Texture(), false)
{
	mMesh = Mesh::generateWingAdvancedTIE(80, 50);

	mTexture->load("../assets/images/noche.jpg", 122);
}

WingAdvancedTIE::~WingAdvancedTIE()
{
	delete mTexture;
	mTexture = nullptr;
}

void WingAdvancedTIE::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication

		mShader->use();

		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (mTexture != nullptr) mTexture->bind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

TieFighter::TieFighter()
{
	addEntity(new Sphere(30, 30, 40, glm::dvec4(0, 0.25, 0.41, 1)));

	Cone* wingConnector = new Cone(100, 8, 8, 30, 30, glm::dvec4(0, 0.25, 0.41, 1));

	glm::mat4 rot = glm::rotate(glm::mat4(1), glm::radians(90.0f), vec3(1, 0, 0));
	glm::mat4 trans = glm::translate(glm::mat4(1), glm::vec3(0, 0, -50));

	wingConnector->setModelMat(trans * rot);

	addEntity(wingConnector);

	// Right Wing
	WingAdvancedTIE* rWing = new WingAdvancedTIE();
	rot = glm::rotate(modelMat(), glm::radians(180.0f), vec3(0, 1, 0));
	trans = glm::translate(glm::mat4(1), glm::vec3(0, 0, 15));

	rWing->setModelMat(trans * rot);

	addEntity(rWing);

	// Left Wing
	WingAdvancedTIE* lWing = new WingAdvancedTIE();
	trans = glm::translate(glm::mat4(1), glm::vec3(0, 0, -15));

	lWing->setModelMat(trans);

	addEntity(lWing);

	// Nose
	Cone* shipNose = new Cone(52, 10, 10, 30, 30, glm::dvec4(0, 0.25, 0.41, 1));

	rot = glm::rotate(glm::mat4(1), glm::radians(-90.0f), vec3(0, 0, 1));

	shipNose->setModelMat(rot);

	addEntity(shipNose);

	// Nose Disk
	Disk* shipNoseDisk = new Disk(15, 0, 5, 40, glm::dvec4(0, 0.25, 0.41, 1));

	rot = glm::rotate(glm::mat4(1), glm::radians(90.0f), vec3(0, 0, 1));
	trans = glm::translate(glm::mat4(1), glm::vec3(53, 0, 0));

	shipNoseDisk->setModelMat(trans * rot);

	addEntity(shipNoseDisk);
}

ModelEntity::ModelEntity(const std::string &filePath, float scale)
{
	mMeshes = IndexMesh::loadAllMeshesWithAssimp(filePath, scale);

	for (auto* mesh : mMeshes) {
		mMaterials.push_back(new PBRMaterial());

		// mMaterials.back()->albedo    = mesh->diffuseTex;
		// mMaterials.back()->normal    = mesh->normalTex;
		// mMaterials.back()->roughness = mesh->roughnessTex;
		// mMaterials.back()->metallic  = mesh->metallicTex;
		// mMaterials.back()->ao        = mesh->aoTex;
		// mMaterials.back()->emissive  = mesh->emissiveTex;

		EntityWithMultiTexture* entity = new EntityWithMultiTexture();
		entity->setMesh(mesh);
		entity->setMaterial(mMaterials.back());
		addEntity(entity);
		std::cout << "Added entity with mesh" << std::endl;
	}
}

void ModelEntity::render(const glm::mat4 &modelViewMat) const
{
	CompoundEntity::render(modelViewMat);

	std::cout << "Number of bones: " << mMeshes[0]->boneLines.size() << std::endl;

	// glDisable(GL_DEPTH_TEST);
	// glUseProgram(0);  // fixed pipeline

	// glBegin(GL_LINES);
	// glColor3f(1.0f, 0.0f, 0.0f); // red skeleton

	// for (auto& line : mMesh->boneLines)
	// {
	// 	glVertex3fv(glm::value_ptr(line.start));
	// 	glVertex3fv(glm::value_ptr(line.end));
	// }

	// glEnd();
	// glEnable(GL_DEPTH_TEST);
}

ModelEntity::~ModelEntity() 
{
	for (auto* material : mMaterials) {
		delete material;
	}
	mMaterials.clear();
}

ModelSingleMeshMaterial::ModelSingleMeshMaterial(const std::string &filePath, const glm::vec4 &color)
{
	mShader = Shader::get("light");
	mMesh = IndexMesh::loadMeshWithAssimp(filePath, 5.0f);
}

void ModelSingleMeshMaterial::render(const glm::mat4 &modelViewMat) const
{
	if (mMesh == nullptr) {
		std::cerr << "ModelSingleMeshMaterial::render: No mesh assigned!" << std::endl;
	}

	assert(mShader == Shader::get("light"));

	mat4 model = mModelMat;
	mat4 view  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().viewMat();
	mat4 proj  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().projMat();
	
	mShader->use();
	mShader->setUniform("modelMat", model);
	mShader->setUniform("viewMat", view);
	mShader->setUniform("projMat", proj);
	mMaterial.upload(*mShader);

	glEnable(GL_DEPTH_TEST);
	mMesh->render();
	glDisable(GL_DEPTH_TEST);
}

ModelSingleMeshPBR::ModelSingleMeshPBR(const std::string &filePath)
{
	mShader = Shader::get("pbr");
	mMesh = IndexMesh::loadMeshWithAssimp(filePath, 1.0f);

	mPBRMaterial = new PBRMaterial();

	mPBRMaterial->albedoTex            = mMesh->albedoTex;
	mPBRMaterial->normalTex            = mMesh->normalTex;
	mPBRMaterial->metallicRoughnessTex = mMesh->metallicRoughnessTex;
	mPBRMaterial->roughnessTex         = mMesh->roughnessTex;
	mPBRMaterial->metallicTex          = mMesh->metallicTex;
	mPBRMaterial->aoTex                = mMesh->aoTex;
	mPBRMaterial->emissiveTex          = mMesh->emissiveTex;
}

ModelSingleMeshPBR::~ModelSingleMeshPBR()
{
	delete mPBRMaterial;
	mPBRMaterial = nullptr;

	// Note: mMesh is deleted in the base class Abs_Entity
}

void ModelSingleMeshPBR::render(const glm::mat4 &modelViewMat) const
{
	if (mMesh == nullptr) {
		std::cerr << "ModelSingleMeshPBR::render: No mesh assigned!" << std::endl;
	}

	assert(mShader == Shader::get("pbr"));

	mat4 model = mModelMat;
	mat4 view  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().viewMat();
	mat4 proj  = static_cast<GameApp&>(GameApp::getInstance()).getCamera().projMat();
	
	mShader->use();
	mShader->setUniform("modelMat", model);
	mShader->setUniform("viewMat", view);
	mShader->setUniform("projMat", proj);

	glm::vec3 lightDirWorld = glm::normalize(glm::vec3(-1, -1, -1));
	glm::vec3 lightDirView  = glm::mat3(view) * lightDirWorld;
	mShader->setUniform("lightDirView", lightDirView);

	if (mPBRMaterial) {
		mPBRMaterial->bindAll(mShader->getProgram());
		glEnable(GL_DEPTH_TEST);
		mMesh->render();
		glDisable(GL_DEPTH_TEST);
		mPBRMaterial->unbindAll(); 
	} 
	else {
		std::cout << "Warning: PBRMaterial is null in ModelSingleMeshPBR::render()" << std::endl;
	}


}
