#pragma once

#include "Entity.h"


/* ---------------------SINGLE COLOR ENTITY--------------------- */

class RegularPolygon : public SingleColorEntity
{
public:
	explicit RegularPolygon(GLuint num, GLdouble r, const glm::vec4& color);
};

class Cube : public SingleColorEntity
{
public:
	explicit Cube(GLdouble length, const glm::vec4& color);
	void render(const glm::mat4& modelViewMat) const override;
};


/* ---------------------ENTITY WITH COLORS--------------------- */

class RGBAxes : public EntityWithColors
{
public:
	explicit RGBAxes(GLdouble l);
};

class RGBTriangle : public EntityWithColors
{
public:
	explicit RGBTriangle(GLdouble r);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
private:
	GLdouble orbit_angle = 0.0f;
};

class RGBRectangle : public EntityWithColors
{
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
};

class RGBCube : public EntityWithColors
{
public:
	explicit RGBCube(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};


/* ---------------------ENTITY WITH TEXTURE--------------------- */

class Ground : public EntityWithTexture
{
public:
	explicit Ground(Texture* texture, bool modulate, GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	void render(const glm::mat4& modelViewMat) const override;
};

class BoxOutline : public EntityWithTexture
{
public:

	explicit BoxOutline(Texture* texture, Texture* iteriorTexture, bool modulate, GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;

protected:

	Texture* mIteriorTexture;
};

class Star3D : public EntityWithTexture
{
public:
	explicit Star3D(Texture* texture, bool modulate, GLdouble re, GLuint np, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
};

class GlassParapet : public EntityWithTexture
{
public:
	explicit GlassParapet(Texture* texture, bool modulate, GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};

class BoxCover : public EntityWithTexture
{
public:
	explicit BoxCover(Texture* texture, Texture* iteriorTexture, bool modulate, GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;

protected:
	Texture* mIteriorTexture;
	GLdouble mLength;
	float mAngle;
	float mCurrAngle;
	float mRotSpeed;
};

class Grass : public EntityWithTexture
{
public:
	explicit Grass(Texture* texture, bool modulate, GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
};


/* ---------------------COLOR MATERIAL ENTITY--------------------- */

class Toros : public ColorMaterialEntity
{
public:
	explicit Toros(GLdouble R, GLdouble r, GLuint nPoints, GLuint nSamples, const glm::vec4& color = glm::vec4(1));
};

class IndexedBox : public ColorMaterialEntity
{
public:
	explicit IndexedBox(GLdouble length, const glm::vec4& color = glm::vec4(1));
	void render(const glm::mat4& modelViewMat) const override;
};

class IndexedMesh : public ColorMaterialEntity
{
public:
	explicit IndexedMesh(const std::string& filePath, const glm::vec4& color = glm::vec4(1));
};

class Sphere : public ColorMaterialEntity
{
public:
	explicit Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians, const glm::vec4& color = glm::vec4(1));
};

class Disk : public ColorMaterialEntity
{
public:
	explicit Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples, const glm::vec4& color = glm::vec4(1));
};

class Cone : public ColorMaterialEntity
{
public:
	// La R es la parte de arriba y la r la de abajo
	explicit Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples, const glm::vec4& color = glm::vec4(1));
};

class ModelSingleMeshMaterial : public ColorMaterialEntity
{
public:
	explicit ModelSingleMeshMaterial(const std::string& filePath, const glm::vec4& color = glm::vec4(1));
	virtual void render(const glm::mat4& modelViewMat) const override;
};

class ModelSingleMeshPBR : public Abs_Entity
{
public:
	explicit ModelSingleMeshPBR(const std::string& filePath, float scale = 1.0f);
	virtual ~ModelSingleMeshPBR();
	virtual void render(const glm::mat4& modelViewMat) const override;
protected:
	PBRMaterial* mPBRMaterial = nullptr;
};


/* ---------------------COMPOUNTD ENTITY--------------------- */

class WingAdvancedTIE : public EntityWithTexture
{
public:
	explicit WingAdvancedTIE();
	~WingAdvancedTIE();

	void render(const glm::mat4& modelViewMat) const override;
};

class TieFighter : public CompoundEntity
{
public:
	explicit TieFighter();
};

class ModelEntity : public CompoundEntity
{
public:
	explicit ModelEntity(const std::string& filePath, float scale = 1.0f);
	virtual void render(const glm::mat4& modelViewMat) const override;
	~ModelEntity();
protected:
	std::vector<IndexMesh*> mMeshes;
	std::vector<PBRMaterial*> mMaterials;
};
