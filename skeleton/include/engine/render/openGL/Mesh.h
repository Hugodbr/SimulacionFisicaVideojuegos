#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

class Texture;

class Mesh
{
public:
	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generateRegularPolygon(GLuint num, GLdouble r);
	static Mesh* createRGBTriangle(GLdouble r);
	static Mesh* generateRectangle(GLdouble w, GLdouble h);
	static Mesh* generateRGBRectangle(GLdouble w, GLdouble h);
	static Mesh* generateCube(GLdouble length);
	static Mesh* generateRGBCubeTriangles(GLdouble length);
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	static Mesh* generateBoxOutline(GLdouble length);
	static Mesh* generateBoxOutlineTexCor(GLdouble length);
	static Mesh* generateStar3D(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generateWingAdvancedTIE(GLdouble panelW, GLdouble panelH); // Gets panel size

	Mesh();
	virtual ~Mesh();

	Mesh(const Mesh& m) = delete;            // no copy constructor
	Mesh& operator=(const Mesh& m) = delete; // no copy assignment

	virtual void render() const;

	GLuint size() const { return mNumVertices; }; // number of elements
	std::vector<glm::vec3> const& vertices() const { return vVertices; };
	std::vector<glm::vec4> const& colors() const { return vColors; };

	virtual void load();
	virtual void unload();

	Texture* normalTex = nullptr;
	Texture* roughnessTex = nullptr;
	Texture* metallicTex = nullptr;
	Texture* aoTex = nullptr;
	Texture* emissiveTex = nullptr;
	Texture* metallicRoughnessTex = nullptr;
	Texture* albedoTex = nullptr;

	// skeletal animation support /
	struct BoneInfo {
		std::string name;
		int parentIndex;
		glm::mat4 offset;        // bone → mesh space
		glm::mat4 transform;     // bind pose transform (from Assimp)
		glm::mat4 globalTransform;
	};

	struct BoneDebugLine {
		glm::vec3 start;
		glm::vec3 end;
	};

	std::vector<BoneInfo> bones;
	std::vector<BoneDebugLine> boneLines;
	// skeletal animation support /

protected:
	GLuint mPrimitive = GL_TRIANGLES;          // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0; // number of elements ( = vVertices.size())
	std::vector<glm::vec3> vVertices; // vertex array
	std::vector<glm::vec4> vColors;   // color array
	std::vector<glm::vec2> vTexCoords;   // texture vertex array
	std::vector<glm::vec3> vTangents; // tangent array
	virtual void draw() const;

	std::vector<glm::vec3> vNormals; // en la CPU
	GLuint mNBO; // en la GPU

	GLuint mVAO;  // vertex array object

private:
	GLuint mVBO;  // vertex buffer object
	GLuint mCBO;  // color buffer object
	GLuint mTCO;  // texture buffer object
	GLuint mTangentsBO; // tangent buffer object
};

class IndexMesh : public Mesh
{
public:
	IndexMesh();
	~IndexMesh();

	IndexMesh(const IndexMesh& im) = delete;            // no copy constructor
	IndexMesh& operator=(const IndexMesh& im) = delete; // no copy assignment

	static IndexMesh* generateByRevolution(
		const std::vector<glm::vec2>& profile, GLuint nSamples,
		GLfloat angleMax = glm::two_pi<GLfloat>()  // or glm::pi<GLfloat>()
	);

	static IndexMesh* generateIndexedBox(GLdouble l);
	static IndexMesh* loadMeshWithAssimp(const std::string& filePath, float scale = 1.0f);
	static std::vector<IndexMesh*> loadAllMeshesWithAssimp(const std::string& filePath, float scale = 1.0f);

	void load() override;
	void unload() override;
	void draw() const override;

	void buildNormalVectors();
	void loadBones(const aiMesh* aiMesh);


protected:
	std::vector<GLuint> vIndexes;

private:
	GLuint mIBO; // index buffer object
};

#endif //_H_Scene_H_