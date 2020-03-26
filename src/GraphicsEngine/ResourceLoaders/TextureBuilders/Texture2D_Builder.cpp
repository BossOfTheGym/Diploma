#include "Texture2D_Builder.h"

#include <algorithm>

namespace res
{
	using PixelData          = Texture2D_Builder::PixelData;
	using ImageData          = Texture2D_Builder::ImageData;
	using SamplingParameters = Texture2D_Builder::SamplingParameters;

	namespace
	{
		__forceinline const void* getPointer(const PixelData& data)
		{
			return (!data.empty() ? data.data() : nullptr);
		}
	}

	Texture2D_Builder::Texture2D_Builder(
		const PixelData& pixelDataInit
		, const ImageData& imageDataInit
		, const SamplingParameters& samplingParamsInit
	)
		: m_pixelData(pixelDataInit)
		, m_imageData(imageDataInit)
		, m_samplingParams(samplingParamsInit)
	{
		m_imageData.pixels = getPointer(m_pixelData);
	}

	Texture2D_Builder::Texture2D_Builder(
		PixelData&& pixelDataInit
		, const ImageData& imageDataInit
		, const SamplingParameters& samplingParamsInit
	)
		: m_pixelData(std::move(pixelDataInit))
		, m_imageData(std::move(imageDataInit))
		, m_samplingParams(std::move(samplingParamsInit))
	{
		m_imageData.pixels = getPointer(m_pixelData);
	}


	Texture2D_Builder::Texture2D_Builder(const Texture2D_Builder& builder)
		: m_pixelData(builder.m_pixelData)
		, m_imageData(builder.m_imageData)
		, m_samplingParams(builder.m_samplingParams)
	{
		m_imageData.pixels = getPointer(m_pixelData);
	}

	Texture2D_Builder::Texture2D_Builder(Texture2D_Builder&& builder) noexcept
		: m_pixelData(std::move(builder.m_pixelData))
		, m_imageData(std::move(builder.m_imageData))
		, m_samplingParams(std::move(builder.m_samplingParams))
	{
		m_imageData.pixels = getPointer(m_pixelData);
	}



	Texture2D_Builder& Texture2D_Builder::operator = (const Texture2D_Builder& builder)
	{
		m_pixelData      = builder.m_pixelData;
		m_imageData      = builder.m_imageData;
		m_samplingParams = builder.m_samplingParams;

		m_imageData.pixels = getPointer(m_pixelData);

		return *this;
	}

	Texture2D_Builder& Texture2D_Builder::operator = (Texture2D_Builder&& builder) noexcept
	{
		std::swap(m_pixelData, builder.m_pixelData);
		std::swap(m_imageData, builder.m_imageData);
		std::swap(m_samplingParams, builder.m_samplingParams);

		//a bit redundant
		m_imageData.pixels = getPointer(m_pixelData);

		return *this;
	}


	//build method
	gl::Texture Texture2D_Builder::buildTexture()
	{
		gl::Texture tex;

		auto& [minFilter, magFilter, wrapModeS, wrapModeT, borderColor] = m_samplingParams;

		tex.genTexture();

		tex.bind(gl::TextureTarget::Texture2D);

		tex.texImage2D(m_imageData);

		tex.textureParameteri(gl::TextureParameter::TextureMinFilter, static_cast<GLint>(minFilter));
		tex.textureParameteri(gl::TextureParameter::TextureMagFilter, static_cast<GLint>(magFilter));
		tex.textureParameteri(gl::TextureParameter::TextureWrapS, static_cast<GLint>(wrapModeS));
		tex.textureParameteri(gl::TextureParameter::TextureWrapT, static_cast<GLint>(wrapModeT));

		if (wrapModeS == gl::TextureParameterValue::ClampToBorder || wrapModeT == gl::TextureParameterValue::ClampToBorder)
		{
			tex.textureParameterfv(gl::TextureParameter::TextureBorderColor, glm::value_ptr(borderColor));
		}

		tex.unbind(gl::TextureTarget::Texture2D);

		return std::move(tex);
	}
}