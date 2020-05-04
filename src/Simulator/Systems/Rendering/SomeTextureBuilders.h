#pragma once

namespace res
{ 
	class Texture2D_Builder;
}

namespace misc
{
	res::Texture2D_Builder checkedTextureBuilder(int width, int height);

	res::Texture2D_Builder colorTextureBuilder(int width, int height);

	res::Texture2D_Builder depthTextureBuilder(int width, int height);

	res::Texture2D_Builder rgbaf32TextureBuilder(int width, int height);

	res::Texture2D_Builder rgbaf16TextureBuilder(int width, int height);
}