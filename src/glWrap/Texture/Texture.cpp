#include "Texture.h"

#include <utility>

namespace gl
{
//constructors & destructor
	Texture::Texture() : Id()
	{}

	Texture::Texture(Texture&& texture) : Id()
	{
		*this = std::move(texture);
	}


	Texture::~Texture()
	{
		deleteTexture();
	}


	//operators
	Texture& Texture::operator = (Texture&& texture)
	{
		static_cast<Id&>(*this) = static_cast<Id&&>(texture);

		return *this;
	}


	//core functions
	void Texture::genTexture()
	{
		GLuint textureId;
		glGenTextures(1, &textureId);

		static_cast<Id&>(*this) = textureId;
	}

	void Texture::createTexture(TextureTarget target)
	{
		GLuint textureId;

		glCreateTextures(static_cast<GLenum>(target), 1, &textureId);

		static_cast<Id&>(*this) = textureId;
	}


	void Texture::bind(TextureTarget target) const
	{
		glBindTexture(static_cast<GLenum>(target), id());
	}

	void Texture::unbind(TextureTarget target) const
	{
		glBindTexture(static_cast<GLenum>(target), Id::Empty);
	}

	void Texture::active(TextureActive texture) const
	{
		glActiveTexture(texture);
	}

	void Texture::bindToUnit(TextureUnit unit) const
	{
		glBindTextureUnit(unit, id());
	}


	void Texture::texParameteri(TextureTarget target, TextureParameter name, GLint parameter)
	{
		glTexParameteri(
			static_cast<GLenum>(target)
			, static_cast<GLenum>(name)
			, parameter
		);
	}

	void Texture::textureParameteri(TextureParameter name, GLint parameter)
	{
		glTextureParameteri(id(), static_cast<GLenum>(name), parameter);
	}

	void Texture::textureParameterfv(TextureParameter name, const GLfloat* params)
	{
		glTextureParameterfv(id(), static_cast<GLenum>(name), params);
	}

	void Texture::texImage2D(const Image2D_Data& data)
	{
		glTexImage2D(
			static_cast<GLenum>(data.target)
			, data.level
			, static_cast<GLint>(data.internalFormat)
			, data.width
			, data.height
			, 0 // border = 0
			, static_cast<GLenum>(data.format)
			, static_cast<GLenum>(data.type)
			, data.pixels
		);
	}

	//delete
	void Texture::deleteTexture()
	{
		GLuint textureId = id();
		glDeleteTextures(1, &textureId);

		resetTexture();
	}

	void Texture::resetTexture()
	{
		resetId();
	}
}
