#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "PBRMaterial.h"


class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0)  // 4x4 identity matrix
	  , mShader(nullptr) 
	  , mWorldPosition({0, 0, 0}) {};
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::mat4& modelViewMat) const = 0; // abstract method
	virtual void update() {};

	// modeling matrix
	glm::mat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::mat4 const& aMat) { mModelMat = aMat; };

	// position modification
	glm::vec3 getWPos();
	void setWPos(glm::vec3 position);
	void setWPos(float x, float y, float z);

	void setTexture(Texture* tex) { mTexture = tex; }

	void setVisibility(bool visibility) { _isVisible = visibility; };
	// Called by the scene when rendering
	bool isVisible() const { return _isVisible; };

	// load or unload entity data into the GPU
	virtual void load();
	virtual void unload();

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::mat4 mModelMat;  // modeling matrix
	Shader* mShader; // shader
	Texture* mTexture = nullptr;

	glm::vec3 mWorldPosition; // position with axis as origin

	bool _isVisible = true;

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::mat4& mModelViewMat) const;
};



class EntityWithColors : public Abs_Entity
{
public:
	explicit EntityWithColors();
	void render(const glm::mat4& modelViewMat) const override;
};



class SingleColorEntity : public Abs_Entity
{
public:
	explicit SingleColorEntity(const glm::vec4& color = glm::vec4(0, 0, 1, 0));
	void render(const glm::mat4& modelViewMat) const override;
	glm::vec4 getColor() const;
	void setColor(const glm::vec4& color);

protected:
	glm::vec4 mColor;
};



class EntityWithTexture : public Abs_Entity
{
public:
	explicit EntityWithTexture(Texture* texture, bool modulate);
	void render(const glm::mat4& modelViewMat) const override;

protected:
	bool mModulate = false;
};

class EntityWithMultiTexture : public Abs_Entity
{
public:
	explicit EntityWithMultiTexture();
	void render(const glm::mat4& modelViewMat) const override;
	void setMesh(IndexMesh* mesh) { mMesh = mesh; }
	void setMaterial(PBRMaterial* pbrMaterial) { mPBRMaterial = pbrMaterial; }
	void setModulate(bool modulate) { mModulate = modulate; }
protected:
	bool mModulate = false;
	PBRMaterial* mPBRMaterial = nullptr;
};



class CompoundEntity : public Abs_Entity
{
public:
	CompoundEntity() : Abs_Entity() {}
	CompoundEntity(const CompoundEntity& e) = delete;            // no copy constructor
	CompoundEntity& operator=(const CompoundEntity& e) = delete; // no copy assignment

	virtual ~CompoundEntity();

	void render(const glm::mat4& modelViewMat) const override; // abstract method
	void update() override {};

	void load() override;
	void unload() override;

	void addEntity(Abs_Entity* ae);

protected:
	std::vector<Abs_Entity*> gObjects;
};



class EntityWithMaterial : public SingleColorEntity
{
public:
	explicit EntityWithMaterial(const glm::vec4& color = glm::vec4(1));
	void render(const glm::mat4& modelViewMat) const override;
	void setMaterial(const Material& m) { mMaterial = m; }

	void toggleShowNormals();
	void renderNormals(const glm::mat4& modelViewMat) const;

protected:
	bool mShowNormals = false;
	Material mMaterial;
};



class ColorMaterialEntity : public EntityWithMaterial
{
public:
	explicit ColorMaterialEntity(const glm::vec4& color = glm::vec4(1));
};

#endif //_H_Entities_H_
