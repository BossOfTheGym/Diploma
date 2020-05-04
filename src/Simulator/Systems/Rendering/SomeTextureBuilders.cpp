#include "SomeTextureBuilders.h"

#include <GraphicsEngine/ResourceLoaders/TextureBuilders/Texture2D_Builder.h>

namespace
{
	res::Texture2D_Builder::PixelData checkedData(int width, int height)
	{
		const int CHANNELS = 4;

		res::Texture2D_Builder::PixelData result(static_cast<size_t>(width) * height * CHANNELS);

		auto iter = result.begin();
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int checked = (i / 32 % 2 == (j / 32 % 2));

				*iter = static_cast<float>(checked); ++iter;
				*iter = static_cast<float>(checked); ++iter;
				*iter =	static_cast<float>(checked); ++iter;
				*iter = 1.00f; ++iter;
			}
		}

		return result;
	}
}

namespace misc
{
	res::Texture2D_Builder checkedTextureBuilder(int width, int height)
	{
		return res::Texture2D_Builder(
			  checkedData(width, height)

			, res::Texture2D_Builder::ImageData
			{
				  gl::TextureTarget::Texture2D
				, 0
				, gl::InternalFormat::RGBA
				, width
				, height
				, gl::PixelDataFormat::RGBA
				, gl::DataType::Float
				, nullptr
			}

			, res::Texture2D_Builder::SamplingParameters
			{
				  gl::TextureParameterValue::Linear
				, gl::TextureParameterValue::Linear
				, gl::TextureParameterValue::Repeat
				, gl::TextureParameterValue::Repeat
			}
		);
	}

	res::Texture2D_Builder colorTextureBuilder(int width, int height)
	{
		return res::Texture2D_Builder(
			res::Texture2D_Builder::PixelData{}

			, res::Texture2D_Builder::ImageData
			{
				  gl::TextureTarget::Texture2D
				, 0
			    , gl::InternalFormat::RGBA
			    , width
			    , height
			    , gl::PixelDataFormat::RGBA
			    , gl::DataType::Float
			    , nullptr
			}

			, res::Texture2D_Builder::SamplingParameters
			{
				  gl::TextureParameterValue::Linear
				, gl::TextureParameterValue::Linear
			    , gl::TextureParameterValue::ClampToEdge
			    , gl::TextureParameterValue::ClampToEdge
			}
		);
	}

	res::Texture2D_Builder depthTextureBuilder(int width, int height)
	{
		return res::Texture2D_Builder(
			res::Texture2D_Builder::PixelData{}

			, res::Texture2D_Builder::ImageData
			{
				gl::TextureTarget::Texture2D
				, 0
			    , gl::InternalFormat::DepthComponent
			    , width
			    , height
			    , gl::PixelDataFormat::DepthComponent
			    , gl::DataType::Float
			    , nullptr
			}

			, res::Texture2D_Builder::SamplingParameters
			{
				  gl::TextureParameterValue::Nearest
				, gl::TextureParameterValue::Nearest
				, gl::TextureParameterValue::ClampToEdge
				, gl::TextureParameterValue::ClampToEdge
			}
		);
	}

	res::Texture2D_Builder rgbaf32TextureBuilder(int width, int height)
	{
		return res::Texture2D_Builder(
			res::Texture2D_Builder::PixelData{}

			, res::Texture2D_Builder::ImageData
			{
				  gl::TextureTarget::Texture2D
				, 0
				, gl::InternalFormat::RGBA32F
				, width
				, height
				, gl::PixelDataFormat::RGBA
				, gl::DataType::Float
				, nullptr
			}
		);
	}

	res::Texture2D_Builder rgbaf16TextureBuilder(int width, int height)
	{
		return res::Texture2D_Builder(
			res::Texture2D_Builder::PixelData{}

			, res::Texture2D_Builder::ImageData
			{
				gl::TextureTarget::Texture2D
				, 0
			, gl::InternalFormat::RGBA16F
			, width
			, height
			, gl::PixelDataFormat::RGBA
			, gl::DataType::Float
			, nullptr
			}
		);
	}
}