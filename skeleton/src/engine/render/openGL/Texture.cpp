#include "Texture.h"

#include "Image.h"
#include <iostream>
#include <vector>

using rgba_color = glm::u8vec4;

Texture::~Texture()
{
	if (mId != 0)
		glDeleteTextures(1, &mId);
}

void
Texture::init()
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);

	// Filters and clamping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // GL_CLAMP
}

void
Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, mId);
}

void
Texture::load(const std::string& name, GLubyte alpha)
{
	if (mId == 0)
		init();

	Image image;
	image.load(name);

	if (alpha != 255)
		image.setAlpha(alpha);

	mWidth = image.width();
	mHeight = image.height();

	GLint level = 0;  // Base image level
	GLint border = 0; // No border

	glBindTexture(GL_TEXTURE_2D, mId);
	glTexImage2D(GL_TEXTURE_2D,
	             level,
	             GL_RGBA,
	             mWidth,
	             mHeight,
	             border,
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             image.data());

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadColorBuffer(GLsizei width, GLsizei height, GLuint buffer)
{
	bind();

	init();
	mWidth = width;
	mHeight = height;

	glReadBuffer(buffer);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);

	unbind();
}

void Texture::save(const std::string& name)
{
	bind();

	if (mWidth == 0 || mHeight == 0) {
		unbind();
		throw std::logic_error("Texture::save: ERROR: Texture dimensions are invalid.");
	}

	GLsizei size = mWidth * mHeight * 4;
	std::vector<GLubyte> pixels(size);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

	Image image;
	GLuint a_width = mWidth;
	GLuint a_height = mHeight;
	image.load(reinterpret_cast<rgba_color*>(pixels.data()), a_width, a_height);

	image.save(name);

	unbind();

	std::cout << "image saved" << std::endl;
}

void
Texture::setWrap(GLuint wp) // GL_REPEAT, GL_CLAMP_TO_EDGE, ...
{
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);
	glBindTexture(GL_TEXTURE_2D, 0);
}
