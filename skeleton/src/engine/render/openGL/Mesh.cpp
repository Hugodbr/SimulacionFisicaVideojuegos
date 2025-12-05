#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>

#include "Texture.h"

#include <filesystem>
#include <cassert>
#include <limits>
#include <algorithm>
#include <functional>

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
	: mVAO(NONE)
	, mVBO(NONE)
	, mCBO(NONE)
	, mNBO(NONE)
	, mTCO(NONE)
	, mTangentsBO(NONE)
{
}

Mesh::~Mesh()
{
	unload();
}

void
Mesh::draw() const
{
	glDrawArrays(
		mPrimitive,
		0,
		size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::load()
{
	assert(mVBO == NONE); // not already loaded

	if (vVertices.size() > 0) { // transfer data
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vVertices.size() * sizeof(vec3), vVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);

		if (vColors.size() > 0) {             // upload colors
			glGenBuffers(1, &mCBO);

			glBindBuffer(GL_ARRAY_BUFFER, mCBO);
			glBufferData(GL_ARRAY_BUFFER, vColors.size() * sizeof(vec4), vColors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), nullptr);
			glEnableVertexAttribArray(1);
		}

		if (vTexCoords.size() > 0) {
			glGenBuffers(1, &mTCO);
			glBindBuffer(GL_ARRAY_BUFFER, mTCO);
			glBufferData(GL_ARRAY_BUFFER,
				vTexCoords.size() * sizeof(vec2),
				vTexCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(vec2), nullptr);
			glEnableVertexAttribArray(2);
		}

		if (vNormals.size() > 0) // upload normals
		{
			glGenBuffers(1, &mNBO);
			glBindBuffer(GL_ARRAY_BUFFER, mNBO);
			glBufferData(GL_ARRAY_BUFFER,
				vNormals.size() * sizeof(vec3),
				vNormals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), nullptr);
			glEnableVertexAttribArray(3);
		}

		if (vTangents.size() > 0) // upload tangents
		{
			glGenBuffers(1, &mTangentsBO);
			glBindBuffer(GL_ARRAY_BUFFER, mTangentsBO);
			glBufferData(GL_ARRAY_BUFFER,
				vTangents.size() * sizeof(vec3),
				vTangents.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), nullptr);
			glEnableVertexAttribArray(4);
		}
	}
}

void Mesh::unload()
{
	if (mVAO != NONE) {
		glDeleteVertexArrays(1, &mVAO);
		mVAO = NONE;
	}

	if (mVBO != NONE) {
		glDeleteBuffers(1, &mVBO);
		mVBO = NONE;
	}

	if (mCBO != NONE) {
		glDeleteBuffers(1, &mCBO);
		mCBO = NONE;
	}

	if (mTCO != NONE) {
		glDeleteBuffers(1, &mTCO);
		mTCO = NONE;
	}

	if (mNBO != NONE) {
		glDeleteBuffers(1, &mNBO);
		mNBO = NONE;
	}

	if (mTangentsBO != NONE) {
		glDeleteBuffers(1, &mTangentsBO);
		mTangentsBO = NONE;
	}
}

void
Mesh::render() const
{
	assert(mVAO != NONE);

	glBindVertexArray(mVAO);
	draw();
}

Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh*
Mesh::generateRegularPolygon(GLuint num, GLdouble r)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP;

	mesh->mNumVertices = num;

	mesh->vVertices.reserve(mesh->mNumVertices);

	constexpr double alpha = glm::radians(90.0);
	const double delta = glm::radians(360.0 / num);

	for (int i = 0; i < num; ++i)
	{
		mesh->vVertices.emplace_back(
			r * glm::cos(alpha + delta * i),
			r * glm::sin(alpha + delta * i),
			0.0);
	}

	return mesh;
}

Mesh*
Mesh::createRGBTriangle(GLdouble r)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLES;

	constexpr int num = 3;
	mesh->mNumVertices = num;

	mesh->vVertices.reserve(mesh->mNumVertices);

	constexpr double alpha = glm::radians(90.0);
	constexpr double delta = glm::radians(360.0 / num);

	for (int i = 0; i < num; ++i)
	{
		mesh->vVertices.emplace_back(
			r * glm::cos(alpha + delta * i),
			r * glm::sin(alpha + delta * i),
			0.0);
	}

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh*
Mesh::generateRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 4;

	mesh->vVertices.reserve(mesh->mNumVertices);

	// Orden importante para añadir el color luego 
	// (en este caso los vértices hacen un zig-zag, es decir, una "Z")
	mesh->vVertices.emplace_back(-w / 2, h / 2, 0.0);
	mesh->vVertices.emplace_back(-w / 2, -h / 2, 0.0);
	mesh->vVertices.emplace_back(w / 2, h / 2, 0.0);
	mesh->vVertices.emplace_back(w / 2, -h / 2, 0.0);

	return mesh;
}

Mesh*
Mesh::generateRGBRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = generateRectangle(w, h);

	mesh->vColors.reserve(mesh->mNumVertices);

	// X axis color: red
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh*
Mesh::generateCube(GLdouble length)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->mNumVertices = 36;

	mesh->vVertices.reserve(mesh->mNumVertices);

	// face1
	mesh->vVertices.emplace_back(length / 2, -length / 2, -length / 2); // 2
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2); // 1
	mesh->vVertices.emplace_back(length / 2, -length / 2, length / 2); // 0

	mesh->vVertices.emplace_back(length / 2, -length / 2, -length / 2); // 2
	mesh->vVertices.emplace_back(length / 2, length / 2, -length / 2); // 3
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2); // 1

	// face2
	mesh->vVertices.emplace_back(length / 2, length / 2, -length / 2); // 3
	mesh->vVertices.emplace_back(-length / 2, length / 2, -length / 2); // 4
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2); // 1

	mesh->vVertices.emplace_back(-length / 2, length / 2, -length / 2); // 4
	mesh->vVertices.emplace_back(-length / 2, length / 2, length / 2); // 6
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2); // 1

	// face3
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2); // 1
	mesh->vVertices.emplace_back(-length / 2, -length / 2, length / 2); // 7
	mesh->vVertices.emplace_back(length / 2, -length / 2, length / 2); // 0

	mesh->vVertices.emplace_back(-length / 2, length / 2, length / 2); // 6
	mesh->vVertices.emplace_back(-length / 2, -length / 2, length / 2); // 7
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2); // 1

	//face4
	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2); // 5
	mesh->vVertices.emplace_back(-length / 2, -length / 2, length / 2); // 7
	mesh->vVertices.emplace_back(-length / 2, length / 2, length / 2); // 6

	mesh->vVertices.emplace_back(-length / 2, length / 2, -length / 2); // 4
	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2); // 5
	mesh->vVertices.emplace_back(-length / 2, length / 2, length / 2); // 6

	//face5
	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2); // 5
	mesh->vVertices.emplace_back(length / 2, -length / 2, length / 2); // 0
	mesh->vVertices.emplace_back(-length / 2, -length / 2, length / 2); // 7

	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2); // 5
	mesh->vVertices.emplace_back(length / 2, -length / 2, -length / 2); // 2
	mesh->vVertices.emplace_back(length / 2, -length / 2, length / 2); // 0

	//face6
	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2); // 5
	mesh->vVertices.emplace_back(-length / 2, length / 2, -length / 2); // 4
	mesh->vVertices.emplace_back(length / 2, length / 2, -length / 2); // 3

	mesh->vVertices.emplace_back(length / 2, length / 2, -length / 2); // 3
	mesh->vVertices.emplace_back(length / 2, -length / 2, -length / 2); // 2
	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2); // 5

	return mesh;
}

Mesh*
Mesh::generateRGBCubeTriangles(GLdouble length)
{
	Mesh* mesh = generateCube(length);

	mesh->vColors.reserve(mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; ++i) {
		if (i / 6 == 0 || i / 6 == 3) {
			mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
		}
		else if (i / 6 == 1 || i / 6 == 4) {
			mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
		}
		else mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	}

	return mesh;
}

Mesh*
Mesh::generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh)
{
	Mesh* mesh = generateRectangle(w * rw, h * rh);

	mesh->vTexCoords.reserve(4);

	mesh->vTexCoords.emplace_back(rw, 0.0);
	mesh->vTexCoords.emplace_back(rw, rh);
	mesh->vTexCoords.emplace_back(0.0, 0.0);
	mesh->vTexCoords.emplace_back(0.0, rh);

	return mesh;
}

Mesh*
Mesh::generateBoxOutline(GLdouble length)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 10;

	mesh->vVertices.reserve(mesh->mNumVertices);

	// Se asignan en zig-zag
	mesh->vVertices.emplace_back(length / 2, length / 2, -length / 2);  // 1
	mesh->vVertices.emplace_back(length / 2, -length / 2, -length / 2); // 0
	mesh->vVertices.emplace_back(-length / 2, length / 2, -length / 2); // 3
	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2);// 2

	mesh->vVertices.emplace_back(-length / 2, length / 2, length / 2);  // 6
	mesh->vVertices.emplace_back(-length / 2, -length / 2, length / 2); // 5
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2);   // 8
	mesh->vVertices.emplace_back(length / 2, -length / 2, length / 2);  // 7

	mesh->vVertices.emplace_back(length / 2, length / 2, -length / 2);  // 1
	mesh->vVertices.emplace_back(length / 2, -length / 2, -length / 2); // 0

	return mesh;
}

Mesh*
Mesh::generateBoxOutlineTexCor(GLdouble length)
{
	Mesh* mesh = generateBoxOutline(length);

	mesh->vTexCoords.reserve(10);

	mesh->vTexCoords.emplace_back(1.0, 0.0); // 0
	mesh->vTexCoords.emplace_back(1.0, 1.0);  // 1
	mesh->vTexCoords.emplace_back(0.0, 0.0); // 2
	mesh->vTexCoords.emplace_back(0.0, 1.0); // 3

	mesh->vTexCoords.emplace_back(1.0, 0.0); // 0
	mesh->vTexCoords.emplace_back(1.0, 1.0);  // 1
	mesh->vTexCoords.emplace_back(0.0, 0.0); // 2
	mesh->vTexCoords.emplace_back(0.0, 1.0); // 3

	mesh->vTexCoords.emplace_back(1.0, 0.0); // 0
	mesh->vTexCoords.emplace_back(1.0, 1.0);  // 1

	return mesh;
}

Mesh*
Mesh::generateStar3D(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;

	mesh->mNumVertices = 2 * np + 2;

	mesh->vVertices.reserve(mesh->mNumVertices);

	double alpha = glm::radians(90.0);
	const double delta = glm::radians(360.0 / (2 * np));
	double ri = re / 2;

	mesh->vVertices.emplace_back(0, 0, 0);

	for (int i = 0; i < 2 * np + 1; ++i)
	{
		if (i % 2 == 0)
			mesh->vVertices.emplace_back(
				re * glm::cos(alpha),
				re * glm::sin(alpha),
				h);
		else {
			mesh->vVertices.emplace_back(
				ri * glm::cos(alpha),
				ri * glm::sin(alpha),
				h);
		}
		alpha += delta;
	}

	return mesh;
}

Mesh* Mesh::generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = generateStar3D(re, np, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);

	double alpha = glm::radians(90.0);
	const double delta = glm::radians(360.0 / (2 * np));
	double ri = re / 2;

	mesh->vTexCoords.emplace_back(0.5f, 0.5f);

	for (int i = 0; i < np; ++i)
	{
		if (i % 2 == 0) {
			mesh->vTexCoords.emplace_back(0.0, 1.0);
			mesh->vTexCoords.emplace_back(0.5, 1.0);
		}
		else {
			mesh->vTexCoords.emplace_back(0.0, 1.0);
			mesh->vTexCoords.emplace_back(0.0, 0.5);
		}
		alpha += delta;
	}

	mesh->vTexCoords.emplace_back(0.0, 1.0);

	return mesh;
}

Mesh* Mesh::generateWingAdvancedTIE(GLdouble panelW, GLdouble panelH)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);

	constexpr double edgeWingAng = glm::radians<double>(45);

	mesh->vVertices.emplace_back(panelW / 2, -panelH * (0.5 + glm::sin(edgeWingAng)), 0);
	mesh->vVertices.emplace_back(-panelW / 2, -panelH * (0.5 + glm::sin(edgeWingAng)), 0);

	mesh->vVertices.emplace_back(panelW / 2, -panelH / 2, -panelH * glm::cos(edgeWingAng));
	mesh->vVertices.emplace_back(-panelW / 2, -panelH / 2, -panelH * glm::cos(edgeWingAng));

	mesh->vVertices.emplace_back(panelW / 2, panelH / 2, -panelH * glm::cos(edgeWingAng));
	mesh->vVertices.emplace_back(-panelW / 2, panelH / 2, -panelH * glm::cos(edgeWingAng));

	mesh->vVertices.emplace_back(panelW / 2, panelH * (0.5 + glm::sin(edgeWingAng)), 0);
	mesh->vVertices.emplace_back(-panelW / 2, panelH * (0.5 + glm::sin(edgeWingAng)), 0);

	// Texture vec
	mesh->vTexCoords.reserve(8);

	mesh->vTexCoords.emplace_back(1.0, 0.0);
	mesh->vTexCoords.emplace_back(0.0, 0.0);

	mesh->vTexCoords.emplace_back(1.0, 0.33);
	mesh->vTexCoords.emplace_back(0.0, 0.33);

	mesh->vTexCoords.emplace_back(1.0, 0.66);
	mesh->vTexCoords.emplace_back(0.0, 0.66);

	mesh->vTexCoords.emplace_back(1.0, 1.0);
	mesh->vTexCoords.emplace_back(0.0, 1.0);

	return mesh;
}


// INDEX MESH =======================================================================================================================================

IndexMesh::IndexMesh()
	: mIBO(NONE)
{
}

void IndexMesh::load()
{
	Mesh::load();

	if (vIndexes.size() > 0)
	{
		glBindVertexArray(mVAO);
		glGenBuffers(1, &mIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			vIndexes.size() * sizeof(GLuint),
			vIndexes.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
}

void IndexMesh::unload()
{
	Mesh::unload();

	if (mIBO != NONE) {
		glDeleteBuffers(1, &mIBO);
		mIBO = NONE;
	}
}

void IndexMesh::draw() const
{
	glDrawElements(
		mPrimitive, // primitiva ( GL_TRIANGLES , etc.)
		vIndexes.size(), // número de índices
		GL_UNSIGNED_INT, // tipo de los índices
		0 // offset en el VBO de índices
	);
}

void IndexMesh::buildNormalVectors()
{
	vNormals.resize(mNumVertices, glm::vec3(0.0f)); // Initialize normals to zero

	std::vector<std::vector<glm::vec3>> normalsPerVertices;
	normalsPerVertices.resize(vVertices.size());


	for (int i = 0; i < vIndexes.size(); i += 3) {
		int inx0 = vIndexes[i];
		int inx1 = vIndexes[i + 1];
		int inx2 = vIndexes[i + 2];
		glm::vec3 v0 = vVertices[inx0];
		glm::vec3 v1 = vVertices[inx1];
		glm::vec3 v2 = vVertices[inx2];

		glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

		//normalsPerVertices[inx0].push_back(normal);
		//normalsPerVertices[inx1].push_back(normal);
		//normalsPerVertices[inx2].push_back(normal);

		vNormals[vIndexes[i]] += normal;
		vNormals[vIndexes[i + 1]] += normal;
		vNormals[vIndexes[i + 2]] += normal;
	}

	//for (int vertexIndex = 0; vertexIndex < normalsPerVertices.size(); ++vertexIndex) {
	//	const auto& normals = normalsPerVertices[vertexIndex];
	//	for (int i = 0; i < normals.size(); ++i) {
	//		int j = 0;
	//		bool duplicated = false;
	//		while (j < i && !duplicated) {
	//			if (normals[i] == normals[j]) duplicated = true;
	//			++j;
	//		}
	//		if (!duplicated) {
	//			vNormals[vertexIndex] += normals[i];
	//		}
	//	}
	//}

	for (auto& n : vNormals) {
		n = glm::normalize(n);
	}

	//for (auto& n : vNormals) {
	//	std::cout << "Normalized: " << n.x << ", " << n.y << ", " << n.z << std::endl;
	//}
}

IndexMesh* IndexMesh::generateByRevolution(const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax)
{
	IndexMesh* mesh = new IndexMesh;
	mesh->mPrimitive = GL_TRIANGLES;
	int tamPerfil = profile.size();
	mesh->vVertices.reserve(nSamples * tamPerfil);


	// Genera los vértices de las muestras
	GLdouble theta1 = 2 * 3.14159265358979323846 / static_cast<double>(nSamples);

	for (int i = 0; i < nSamples; ++i) {  // muestra i-ésima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) {  // rota el perfil
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
		}
	}

	// Genera los índices para formar los triángulos
	for (int i = 0; i < nSamples; ++i) { // caras entre i e i+1
		/*int max = tamPerfil - 1*/;
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara vertical
			if (profile[j].x != 0.0) { // triángulo inferior
				for (auto [s, t] : { std::pair{i % nSamples, j}, {i % nSamples, (j + 1)}, {(i + 1) % nSamples, j} }) {
					mesh->vIndexes.push_back(s * tamPerfil + t/*%max*/);
				}
			}

			if (profile[(j + 1)/*%max*/].x != 0.0) { // triángulo superior
				for (auto [s, t] : { std::pair{i % nSamples, (j + 1)}, {(i + 1) % nSamples, (j + 1)}, {(i + 1) % nSamples, j} }) {
					mesh->vIndexes.push_back(s * tamPerfil + t/*%max*/);
				}
			}
		}
	}

	mesh->mNumVertices = mesh->vVertices.size();
	mesh->buildNormalVectors();

	return mesh;
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble length)
{
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// Vertices
	mesh->vVertices.emplace_back(length / 2, length / 2, -length / 2);  // 0
	mesh->vVertices.emplace_back(length / 2, -length / 2, -length / 2); // 1
	mesh->vVertices.emplace_back(-length / 2, length / 2, -length / 2); // 2
	mesh->vVertices.emplace_back(-length / 2, -length / 2, -length / 2); // 3
	mesh->vVertices.emplace_back(-length / 2, length / 2, length / 2);  // 4
	mesh->vVertices.emplace_back(-length / 2, -length / 2, length / 2); // 5
	mesh->vVertices.emplace_back(length / 2, length / 2, length / 2);   // 6
	mesh->vVertices.emplace_back(length / 2, -length / 2, length / 2);  // 7

	// Indexes
	mesh->vIndexes = {
		0, 1, 2,  2, 1, 3,  // Back
		4, 5, 6,  6, 5, 7,  // Front
		2, 3, 4,  4, 3, 5,  // Left
		6, 7, 0,  0, 7, 1,  // Right
		2, 4, 0,  0, 4, 6,  // Top
		5, 3, 1,  1, 7, 5   // Bottom
	};

	mesh->buildNormalVectors();

	//mesh->vNormals.push_back(glm::normalize(glm::vec3(1, 1, -1))); // 0
	//mesh->vNormals.push_back(glm::normalize(glm::vec3(1, -1, -1))); // 1
	//mesh->vNormals.push_back(glm::normalize(glm::vec3(-1, 1, -1))); // 2
	//mesh->vNormals.push_back(glm::normalize(glm::vec3(-1, -1, -1))); // 3
	//mesh->vNormals.push_back(glm::normalize(glm::vec3(-1, 1, 1))); // 4
	//mesh->vNormals.push_back(glm::normalize(glm::vec3(-1, -1, 1))); // 5
	//mesh->vNormals.push_back(glm::normalize(glm::vec3(1, 1, 1))); // 6
	//mesh->vNormals.push_back(glm::normalize(glm::vec3(1, -1, 1))); // 6

	return mesh;
}

IndexMesh *IndexMesh::loadMeshWithAssimp(const std::string &filePath, float scale)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_CalcTangentSpace |
        aiProcess_OptimizeMeshes
    );


    if (!scene || !scene->HasMeshes())
    {
        std::cerr << "Error loading mesh with Assimp: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    aiMesh* aiMesh = scene->mMeshes[0];
	std::cout << "Tangents: " << aiMesh->HasTangentsAndBitangents() << std::endl;

    IndexMesh* mesh = new IndexMesh();
    mesh->mPrimitive = GL_TRIANGLES;

    // --- Vertices ---
    mesh->vVertices.reserve(aiMesh->mNumVertices);
    for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
    {
        aiVector3D pos = aiMesh->mVertices[i] * scale;
        mesh->vVertices.emplace_back(pos.x, pos.y, pos.z);
    }

    // --- Normals ---
    if (aiMesh->HasNormals())
    {
		std::cout << "Has normals" << std::endl;
        mesh->vNormals.reserve(aiMesh->mNumVertices);
        for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
        {
            aiVector3D n = aiMesh->mNormals[i];
            mesh->vNormals.emplace_back(n.x, n.y, n.z);
        }
    }
	else {
		std::cout << "No normals" << std::endl;
	}

	for (int uv = 0; uv < AI_MAX_NUMBER_OF_TEXTURECOORDS; uv++)
	{
		if (aiMesh->HasTextureCoords(uv))
			std::cout << "UV SET " << uv << " exists" << std::endl;
	}
    // --- Texture Coordinates (only channel 0) ---
    if (aiMesh->HasTextureCoords(0))
    {
        mesh->vTexCoords.reserve(aiMesh->mNumVertices);
        for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
        {
            aiVector3D uv = aiMesh->mTextureCoords[0][i];
            mesh->vTexCoords.emplace_back(uv.x, uv.y);
        }
    }

	if (aiMesh->HasTangentsAndBitangents()) {
		mesh->vTangents.reserve(aiMesh->mNumVertices);
		for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
		{
			aiVector3D t = aiMesh->mTangents[i];
			mesh->vTangents.emplace_back(t.x, t.y, t.z);
		}
	}

    // --- Indexes ---
    for (unsigned f = 0; f < aiMesh->mNumFaces; f++)
    {
        const aiFace& face = aiMesh->mFaces[f];
        if (face.mNumIndices != 3) continue;

        mesh->vIndexes.push_back(face.mIndices[0]);
        mesh->vIndexes.push_back(face.mIndices[1]);
        mesh->vIndexes.push_back(face.mIndices[2]);
    }

    mesh->mNumVertices = mesh->vVertices.size();



    // ======================================================================
    //  --- MATERIAL LOADING (Diffuse, Normal, Roughness, Metalness, AO) ---
    // ======================================================================

    aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
    aiString texPath;

	auto loadTex = [&](aiTextureType type) -> Texture*
	{
		aiString texPath;
		if (aiMat->GetTextureCount(type) == 0)
			return nullptr;

		if (aiMat->GetTexture(type, 0, &texPath) != AI_SUCCESS)
			return nullptr;

		std::string raw = texPath.C_Str();
		std::cout << "[Assimp] Material texture type " << type << ": " << raw << std::endl;

		std::filesystem::path baseDir = std::filesystem::path(filePath).parent_path();

		// --- Step 1: Extract only the filename ---
		std::string filename = std::filesystem::path(raw).filename().string();
		std::cout << "[Assimp DEBUG] Extracted filename: " << filename << std::endl;

		// --- Step 2: Search in textures directory ---
		std::filesystem::path texturesDir = baseDir.parent_path() / "textures";
		std::filesystem::path absPath = texturesDir / filename;

		// --- Step 3: If not found, try substring match ---
		if (!std::filesystem::exists(absPath))
		{
			for (auto& f : std::filesystem::directory_iterator(texturesDir))
			{
				std::string existing = f.path().filename().string();

				// Match by "Normal", "Base_Color", etc.
				if (existing.find("Normal") != std::string::npos &&
					filename.find("Normal") != std::string::npos)
				{
					absPath = f.path();
					std::cout << "[MATCH] Found matching texture: " << absPath << std::endl;
					break;
				}
			}
		}

		// --- Step 4: Final check ---
		if (!std::filesystem::exists(absPath))
		{
			std::cout << "[ERROR] Texture NOT FOUND even after fallback: " << absPath << std::endl;
			return nullptr;
		}

		// --- Load texture ---
		Texture* tex = new Texture();
		tex->load(absPath.string());
		std::cout << "[LOAD] Loaded texture: " << absPath << std::endl;
		return tex;
	};

	mesh->diffuseTex    = loadTex(aiTextureType_DIFFUSE);
	mesh->roughnessTex  = loadTex(aiTextureType_DIFFUSE_ROUGHNESS);
	mesh->metallicTex   = loadTex(aiTextureType_METALNESS);
	mesh->aoTex         = loadTex(aiTextureType_AMBIENT_OCCLUSION);
	mesh->emissiveTex   = loadTex(aiTextureType_EMISSIVE);

	// NORMALS MUST BE LAST
	mesh->normalTex = loadTex(aiTextureType_NORMALS);
	if (!mesh->normalTex)
		mesh->normalTex = loadTex(aiTextureType_NORMAL_CAMERA);
	if (!mesh->normalTex)
		mesh->normalTex = loadTex(aiTextureType_HEIGHT);

	std::cout << "FINAL normalTex: " << mesh->normalTex << std::endl;

    return mesh;
}

std::vector<IndexMesh*> IndexMesh::loadAllMeshesWithAssimp(const std::string& filePath, float scale)
{
    std::vector<IndexMesh*> meshes;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_CalcTangentSpace |
        aiProcess_OptimizeMeshes
    );

    if (!scene || !scene->HasMeshes())
    {
        std::cerr << "Error loading mesh with Assimp: " << importer.GetErrorString() << std::endl;
        return meshes;
    }
	std::cout << "Total meshes in file " << filePath << ": " << scene->mNumMeshes << std::endl;

    // ------------------------------------------------------------------
    // SHARED TEXTURE LOADER FOR ANY MATERIAL
    // ------------------------------------------------------------------
    auto loadTex = [&](aiMaterial* aiMat, aiTextureType type) -> Texture*
    {
        aiString texPath;
        if (aiMat->GetTextureCount(type) == 0)
            return nullptr;

        if (aiMat->GetTexture(type, 0, &texPath) != AI_SUCCESS)
            return nullptr;

        // Extract file name only
        std::string raw = texPath.C_Str();
        std::string filename = std::filesystem::path(raw).filename().string();

        // Folder structure
        std::filesystem::path baseDir = std::filesystem::path(filePath).parent_path();
        std::filesystem::path texturesDir = baseDir.parent_path() / "textures";

        // First try exact match
        std::filesystem::path absPath = texturesDir / filename;

        // Try intelligent substring match
        if (!std::filesystem::exists(absPath))
        {
            for (auto& f : std::filesystem::directory_iterator(texturesDir))
            {
                std::string file = f.path().filename().string();

                // Match "Normal" files, BaseColor, etc.
                if ((file.find("Normal")     != std::string::npos && filename.find("Normal")     != std::string::npos) ||
                    (file.find("Base")       != std::string::npos && filename.find("Base")       != std::string::npos) ||
                    (file.find("Color")      != std::string::npos && filename.find("Color")      != std::string::npos) ||
                    (file.find("Rough")      != std::string::npos && filename.find("Rough")      != std::string::npos) ||
                    (file.find("Metal")      != std::string::npos && filename.find("Metal")      != std::string::npos) ||
                    (file.find("Ambient")    != std::string::npos && filename.find("Ambient")    != std::string::npos))
                {
                    absPath = f.path();
                    std::cout << "[MATCH] " << file << " -> " << absPath << std::endl;
                    break;
                }
            }
        }

        if (!std::filesystem::exists(absPath))
        {
            std::cout << "[ERROR] Texture NOT FOUND: " << filename << std::endl;
            return nullptr;
        }

        Texture* tex = new Texture();
        tex->load(absPath.string());
        return tex;
    };

    // ------------------------------------------------------------------
    // MAIN MESH LOOP — LOAD ALL MESHES
    // ------------------------------------------------------------------
    for (unsigned m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh* aiMesh = scene->mMeshes[m];
        IndexMesh* mesh = new IndexMesh();

        mesh->mPrimitive = GL_TRIANGLES;

        // ---- Load vertices ----
        mesh->vVertices.reserve(aiMesh->mNumVertices);
        for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
        {
            aiVector3D pos = aiMesh->mVertices[i] * scale;
            mesh->vVertices.emplace_back(pos.x, pos.y, pos.z);
        }

        // ---- Load normals ----
        if (aiMesh->HasNormals())
        {
            mesh->vNormals.reserve(aiMesh->mNumVertices);
            for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
            {
                aiVector3D n = aiMesh->mNormals[i];
                mesh->vNormals.emplace_back(n.x, n.y, n.z);
            }
        }

		for (int uv = 0; uv < AI_MAX_NUMBER_OF_TEXTURECOORDS; uv++)
		{
			if (aiMesh->HasTextureCoords(uv))
				std::cout << "UV SET " << uv << " exists" << std::endl;
		}
		std::cout << "Mesh has bones: " << aiMesh->mNumBones << std::endl;

		mesh->bones.reserve(aiMesh->mNumBones);

		for (unsigned b = 0; b < aiMesh->mNumBones; b++)
		{
			aiBone* aiBone = aiMesh->mBones[b];

			BoneInfo bone;
			bone.name = aiBone->mName.C_Str();
			bone.offset = glm::transpose(glm::make_mat4(&aiBone->mOffsetMatrix.a1));

			mesh->bones.push_back(bone);
		}

        // ---- Load UVs (channel 0 only) ----
        if (aiMesh->HasTextureCoords(0))
        {
            mesh->vTexCoords.reserve(aiMesh->mNumVertices);
            for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
            {
                aiVector3D uv = aiMesh->mTextureCoords[0][i];
                mesh->vTexCoords.emplace_back(uv.x, uv.y);
            }
        }

        // ---- Load tangents ----
        if (aiMesh->HasTangentsAndBitangents())
        {
            mesh->vTangents.reserve(aiMesh->mNumVertices);
            for (unsigned i = 0; i < aiMesh->mNumVertices; i++)
            {
                aiVector3D t = aiMesh->mTangents[i];
                mesh->vTangents.emplace_back(t.x, t.y, t.z);
            }
        }

        // ---- Load indices ----
        for (unsigned f = 0; f < aiMesh->mNumFaces; f++)
        {
            const aiFace& face = aiMesh->mFaces[f];
            if (face.mNumIndices == 3)
            {
                mesh->vIndexes.push_back(face.mIndices[0]);
                mesh->vIndexes.push_back(face.mIndices[1]);
                mesh->vIndexes.push_back(face.mIndices[2]);
            }
        }

        mesh->mNumVertices = mesh->vVertices.size();

        // ------------------------------------------------------------------
        // LOAD MATERIAL FOR THIS MESH
        // ------------------------------------------------------------------
        aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];

        mesh->diffuseTex    = loadTex(aiMat, aiTextureType_DIFFUSE);
        mesh->roughnessTex  = loadTex(aiMat, aiTextureType_DIFFUSE_ROUGHNESS);
        mesh->metallicTex   = loadTex(aiMat, aiTextureType_METALNESS);
        mesh->aoTex         = loadTex(aiMat, aiTextureType_AMBIENT_OCCLUSION);
        mesh->emissiveTex   = loadTex(aiMat, aiTextureType_EMISSIVE);

        // ---- Normal maps last (with fallback) ----
        mesh->normalTex = loadTex(aiMat, aiTextureType_NORMALS);
        if (!mesh->normalTex) 
            mesh->normalTex = loadTex(aiMat, aiTextureType_NORMAL_CAMERA);
        if (!mesh->normalTex)
            mesh->normalTex = loadTex(aiMat, aiTextureType_HEIGHT);

        meshes.push_back(mesh);
	}

	//--------------------------------------------------------
	// Build skeleton ONCE (outside loop!)
	//--------------------------------------------------------
	std::function<void(aiNode*, int, IndexMesh*)> traverse =
		[&](aiNode* node, int parentIndex, IndexMesh* mesh)
	{
		int currentIndex = -1;

		// Match node to bone
		for (int i = 0; i < mesh->bones.size(); i++)
		{
			if (mesh->bones[i].name == node->mName.C_Str())
			{
				mesh->bones[i].parentIndex = parentIndex;

				glm::mat4 local = glm::transpose(glm::make_mat4(&node->mTransformation.a1));

				if (parentIndex >= 0)
					mesh->bones[i].globalTransform = mesh->bones[parentIndex].globalTransform * local;
				else
					mesh->bones[i].globalTransform = local;

				currentIndex = i;
				break;
			}
		}

		// Recurse children
		for (unsigned c = 0; c < node->mNumChildren; c++)
			traverse(node->mChildren[c], currentIndex, mesh);
	};


	//--------------------------------------------------------
	// Build bone lines for each mesh (static pose)
	//--------------------------------------------------------
	for (IndexMesh* mesh : meshes)
	{
		traverse(scene->mRootNode, -1, mesh);

		mesh->boneLines.clear();

		for (int i = 0; i < mesh->bones.size(); i++)
		{
			int parent = mesh->bones[i].parentIndex;
			if (parent < 0) continue;

			glm::vec3 childPos  = glm::vec3(mesh->bones[i].globalTransform[3]);
			glm::vec3 parentPos = glm::vec3(mesh->bones[parent].globalTransform[3]);

			mesh->boneLines.push_back({ parentPos, childPos });
		}
	}

	return meshes;
}
