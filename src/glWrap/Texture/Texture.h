#pragma once

#include <glWrap/OpenGL.h>
#include <glWrap/ID/Id.h>

namespace gl
{
	class Texture : public Id
	{
	public:
		struct Image2D_Data
		{
			TextureTarget   target         = TextureTarget::None;
			GLint           level          = static_cast<GLint>(0);
			InternalFormat  internalFormat = InternalFormat::None;
			GLsizei         width          = static_cast<GLsizei>(0);
			GLsizei         height         = static_cast<GLsizei>(0);
			// border = 0 - do not delete this comment
			PixelDataFormat format         = PixelDataFormat::None;
			DataType        type           = DataType::None;
			const GLvoid* pixels         = nullptr;
		};


	public:
		Texture();

		Texture(Texture&& tex);

		~Texture();

		Texture& operator = (Texture&& tex);


	public:
		void genTexture();

		void createTexture(TextureTarget target);


		void bind(TextureTarget target) const;

		void unbind(TextureTarget target) const;

		void active(TextureActive texture) const;

		void bindToUnit(TextureUnit unit) const;


		void texParameteri(TextureTarget target, TextureParameter name, GLint parameter);

		void textureParameteri(TextureParameter name, GLint parameter);

		void textureParameterfv(TextureParameter name, const GLfloat* params);

		void texImage2D(const Image2D_Data& data);

		void deleteTexture();


	private:
		void resetTexture();
	};
}