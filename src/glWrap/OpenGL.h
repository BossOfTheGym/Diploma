// TODO : move this to a separate project and use as lib
// TODO : remove dependency from Common.h
// TODO : unite glWrap and Math
// TODO : add config file(after unification it will be contained in Math) 
// TODO : template whole library because many classes have logic depending on float type
#pragma once

#include <Config/Config.h>
#include <GL/glew.h>
#include <string>

namespace gl
{
	using String = std::string;	

	using config::Float;

	using BufferIndex = GLuint;

	enum class BlendMode : GLenum
	{
		Add             = GL_FUNC_ADD
		, Subtract        = GL_FUNC_SUBTRACT
		, ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT
		, Max             = GL_MAX
		, Min             = GL_MIN
	};


	enum class BlendFactor : GLenum
	{
		Zero = GL_ZERO
		, One  = GL_ONE

		, SrcColor         = GL_SRC_COLOR
		, OneMinusSrc      = GL_ONE_MINUS_SRC_COLOR
		, SrcAlpha         = GL_SRC_ALPHA
		, OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA

		, DstColor         = GL_DST_COLOR
		, OneMinusDstColor = GL_ONE_MINUS_DST_COLOR
		, DstAlpha         = GL_DST_ALPHA
		, OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA

		, ConstantColor         = GL_CONSTANT_COLOR
		, OneMinusConstant      = GL_ONE_MINUS_CONSTANT_COLOR
		, ConstatntAlpha        = GL_CONSTANT_ALPHA
		, OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA

		, SrcAlphaSaturate = GL_SRC_ALPHA_SATURATE

		, Src1Color         = GL_SRC1_COLOR
		, OneMinusSrc1Color = GL_ONE_MINUS_SRC1_COLOR
		, Src1Alpha         = GL_SRC1_ALPHA
		, OneMinusSrc1Alpha = GL_ONE_MINUS_SRC1_ALPHA
	};


	enum class Face : GLenum
	{
		Front        = GL_FRONT
		, Back         = GL_BACK
		, FrontAndBack = GL_FRONT_AND_BACK
	};


	enum class DepthFunc : GLenum
	{
		Never        = GL_NEVER
		, Always       = GL_ALWAYS
		, Less         = GL_LESS
		, Equal        = GL_EQUAL
		, LessEqual    = GL_LEQUAL
		, Greater      = GL_GREATER
		, GreaterEqual = GL_GEQUAL
		, NotEqual     = GL_NOTEQUAL
	};


	using CapabalityIndex = GLuint;

	enum class Capability : GLenum
	{
		Blend     = GL_BLEND
		, CullFace  = GL_CULL_FACE
		, DepthTest = GL_DEPTH_TEST
		, Scissor   = GL_SCISSOR_TEST
		, Stencil   = GL_STENCIL_TEST
	};


	enum class FrontFace : GLenum
	{
		Clockwise        = GL_CW
		, CounterClockwise = GL_CCW
	};


	enum class PolygonMode : GLenum
	{
		Point = GL_POINT
		, Line  = GL_LINE
		, Fill  = GL_FILL
	};


	enum class GLBool : GLboolean
	{
		  True  = GL_TRUE
		, False = GL_FALSE
	};



	enum class AttributeSize : GLint
	{
		One   = 1
		, Two   = 2
		, Three = 3
		, Four  = 4

		// TODO : check this out
		// in fact specifies some special DirectX data layout
		, D3D_BGRA = GL_BGRA
	};



	enum class ShaderType : GLenum
	{
		Vertex         = GL_VERTEX_SHADER
		, TessControl    = GL_TESS_CONTROL_SHADER
		, TessEvaluation = GL_TESS_EVALUATION_SHADER
		, Geometry       = GL_GEOMETRY_SHADER
		, Fragment       = GL_FRAGMENT_SHADER
		, Compute        = GL_COMPUTE_SHADER

		, None           = static_cast<GLenum>(0)
	};



	enum class DrawMode : GLenum
	{
		Points                 = GL_POINTS

		, LineStrip              = GL_LINE_STRIP
		, LineLoop               = GL_LINE_LOOP
		, Lines                  = GL_LINES
		, LineStripAdjacency     = GL_LINE_STRIP_ADJACENCY
		, LinesAdjacency         = GL_LINES_ADJACENCY

		, TriangleStrip          = GL_TRIANGLE_STRIP
		, TriangleFan            = GL_TRIANGLE_FAN
		, Triangles              = GL_TRIANGLES
		, TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY
		, TrianglesAdjacency     = GL_TRIANGLES_ADJACENCY

		, Patches                = GL_PATCHES

		, None = static_cast<GLenum>(0)
	};

	enum class IndicesType : GLenum
	{
		UnsignedByte  = GL_UNSIGNED_BYTE
		, UnsignedShort = GL_UNSIGNED_SHORT
		, UnsignedInt   = GL_UNSIGNED_INT

		, None = static_cast<GLenum>(0)
	};



	enum class DataType : GLenum
	{
		  Byte         = GL_BYTE
		, UnsignedByte = GL_UNSIGNED_BYTE

		, Short         = GL_SHORT
		, UnsignedShort = GL_UNSIGNED_SHORT

		, Int         = GL_INT
		, UnsignedInt = GL_UNSIGNED_INT

		, HalfFloat = GL_HALF_FLOAT
		, Float     = GL_FLOAT
		, Double    = GL_DOUBLE
		, Fixed     = GL_FIXED

		, UnsignedInt_2_10_10_10_rev  = GL_UNSIGNED_INT_2_10_10_10_REV
		, UnsignedInt_10F_11F_11F_rev = GL_UNSIGNED_INT_10F_11F_11F_REV

		, None = static_cast<GLenum>(0)
	};



	enum class BufferTarget : GLenum
	{
		ArrayBuffer             = GL_ARRAY_BUFFER
		, AtomicCounterBuffer     = GL_ATOMIC_COUNTER_BUFFER
		, CopyReadBuffer          = GL_COPY_READ_BUFFER
		, CopyWriteBuffer         = GL_COPY_WRITE_BUFFER
		, DispatchIndirectBuffer  = GL_DISPATCH_INDIRECT_BUFFER
		, DrawIndirectBuffer      = GL_DRAW_INDIRECT_BUFFER
		, ElementArrayBuffer      = GL_ELEMENT_ARRAY_BUFFER
		, PixelUnpackBuffer       = GL_PIXEL_UNPACK_BUFFER
		, PixelPackBuffer         = GL_PIXEL_PACK_BUFFER
		, QueryBuffer             = GL_QUERY_BUFFER
		, ShaderStorageBuffer     = GL_SHADER_STORAGE_BUFFER
		, TextureBuffer           = GL_TEXTURE_BUFFER
		, TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER
		, UniformBuffer           = GL_UNIFORM_BUFFER

		, None                    = static_cast<GLenum>(0)
	};

	enum class IndexBufferTarget : GLenum
	{
		AtomicCounterBuffer     = GL_ATOMIC_COUNTER_BUFFER
		, ShaderStorageBuffer     = GL_SHADER_STORAGE_BUFFER
		, TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER
		, UniformBuffer           = GL_UNIFORM_BUFFER
	};

	enum class BufferUsage : GLenum
	{
		StreamDraw  = GL_STREAM_DRAW
		, StreamRead  = GL_STREAM_READ
		, StreamCopy  = GL_STREAM_COPY
		, StaticDraw  = GL_STATIC_DRAW
		, StaticRead  = GL_STATIC_READ
		, StaticCopy  = GL_STATIC_COPY
		, DynamicDraw = GL_DYNAMIC_DRAW
		, DynamicRead = GL_DYNAMIC_READ
		, DynamicCopy = GL_DYNAMIC_COPY
		, None        = static_cast<GLenum>(0)
	};



	using TextureActive = GLenum; // can have many so it can't be wrapped in enum class

	enum class TextureActiveBase : GLenum
	{
		Texture0 = GL_TEXTURE0
	};

	using TextureUnit = GLuint;

	enum class TextureTarget : GLenum
	{
		Texture1D      = GL_TEXTURE_1D
		, ProxyTexture1D = GL_PROXY_TEXTURE_1D

		, Texture2D      = GL_TEXTURE_2D
		, ProxyTexture2D = GL_PROXY_TEXTURE_2D

		, Texture3D      = GL_TEXTURE_3D
		, ProxyTexture3D = GL_PROXY_TEXTURE_3D


		, Texture1D_Array      = GL_TEXTURE_1D_ARRAY
		, ProxyTexture1D_Array = GL_PROXY_TEXTURE_1D_ARRAY

		, Texture2D_Array      = GL_TEXTURE_2D_ARRAY
		, ProxyTexture2D_Array = GL_PROXY_TEXTURE_2D_ARRAY


		, TextureRectangle      = GL_TEXTURE_RECTANGLE
		, ProxyTextureRectangle = GL_PROXY_TEXTURE_RECTANGLE


		, TextureCubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X
		, TextureCubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		, TextureCubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		, TextureCubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		, TextureCubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		, TextureCubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		, ProxyTextureCubeMap     = GL_PROXY_TEXTURE_CUBE_MAP

		, None = static_cast<GLenum>(0)
	};

	enum class TextureParameter : GLenum
	{
		DepthStencilTextureMode = GL_DEPTH_STENCIL_TEXTURE_MODE

		, TextureBaseLevel = GL_TEXTURE_BASE_LEVEL

		, TextureBorderColor = GL_TEXTURE_BORDER_COLOR

		, TextureCompareFunc = GL_TEXTURE_COMPARE_FUNC

		, TextureCompareMode = GL_TEXTURE_COMPARE_MODE

		, TextureLOD_Bias = GL_TEXTURE_LOD_BIAS

		, TextureMinFilter = GL_TEXTURE_MIN_FILTER
		, TextureMagFilter = GL_TEXTURE_MAG_FILTER

		, TextureMinLOD   = GL_TEXTURE_MIN_LOD
		, TextureMaxLOD   = GL_TEXTURE_MAX_LOD
		, TextureMaxLevel = GL_TEXTURE_MAX_LEVEL

		, TextureSwizzleR    = GL_TEXTURE_SWIZZLE_R
		, TextureSwizzleG    = GL_TEXTURE_SWIZZLE_G
		, TextureSwizzleB    = GL_TEXTURE_SWIZZLE_B
		, TextureSwizzleA    = GL_TEXTURE_SWIZZLE_A
		, TextureSwizzleRGBA = GL_TEXTURE_SWIZZLE_RGBA

		, TextureWrapS = GL_TEXTURE_WRAP_S
		, TextureWrapT = GL_TEXTURE_WRAP_T
		, TextureWrapR = GL_TEXTURE_WRAP_R

		, None = static_cast<GLenum>(0)
	};

	enum class TextureParameterValue : GLint
	{
		//Filter
		Nearest = GL_NEAREST
		, Linear  = GL_LINEAR

		//Wrap
		, ClampToBorder  = GL_CLAMP_TO_BORDER
		, ClampToEdge    = GL_CLAMP_TO_EDGE
		, Repeat         = GL_REPEAT
		, MirroredRepeat = GL_MIRRORED_REPEAT

		, None = static_cast<GLint>(0)
	};

	// TODO : add another formats like GL_RGBA32F
	enum class InternalFormat : GLint
	{
		  RG   = GL_RG
		, RGB  = GL_RGB
		, RGBA = GL_RGBA

		, DepthComponent    = GL_DEPTH_COMPONENT
		, DepthComponent16  = GL_DEPTH_COMPONENT16
		, DepthComponent24  = GL_DEPTH_COMPONENT24
		, DepthComponent32  = GL_DEPTH_COMPONENT32
		, DepthComponent32F = GL_DEPTH_COMPONENT32F

		, DepthStencil   = GL_DEPTH_STENCIL

		, R16F    = GL_R16F
		, RG16F   = GL_RG16F
		, RGB16F  = GL_RGB16F
		, RGBA16F = GL_RGBA16F

		, R32F    = GL_R32F
		, RG32F   = GL_RG32F
		, RGB32F  = GL_RGB32F
		, RGBA32F = GL_RGBA32F

		, None = static_cast<GLint>(0)
	};

	enum class PixelDataFormat : GLenum
	{
		  Red            = GL_RED
		, RG             = GL_RG
		, RGB            = GL_RGB
		, BGR            = GL_BGR
		, RGBA           = GL_RGBA
		, BGRA           = GL_BGRA
		, RedInteger     = GL_RED_INTEGER
		, RG_Integer     = GL_RG_INTEGER
		, RGB_Integer    = GL_RGB_INTEGER
		, BGR_Integer    = GL_BGR_INTEGER
		, RGBA_Integer   = GL_RGBA_INTEGER
		, BGRA_Integer   = GL_BGRA_INTEGER
		, StencilIndex   = GL_STENCIL_INDEX
		, DepthComponent = GL_DEPTH_COMPONENT
		, DepthStencil   = GL_DEPTH_STENCIL

		, None = static_cast<GLint>(0)
	};



	enum class FramebufferTarget : GLenum
	{
		Framebuffer     = GL_FRAMEBUFFER
		, ReadFramebuffer = GL_READ_FRAMEBUFFER
		, DrawFramebuffer = GL_DRAW_FRAMEBUFFER

		, None = static_cast<GLenum>(0)
	};


	enum class BlitMask : GLbitfield
	{
		  Color   = GL_COLOR_BUFFER_BIT
		, Depth   = GL_DEPTH_BUFFER_BIT
		, Stencil = GL_STENCIL_BUFFER_BIT

		, ColorDepth   = Color | Depth
		, ColorStencil = Color | Stencil
		, DepthStencil = Depth | Stencil

		, ColorDepthStencil = Color | Depth | Stencil
	};


	enum class ClearMask : GLbitfield
	{
		  Color   = GL_COLOR_BUFFER_BIT
		, Depth   = GL_DEPTH_BUFFER_BIT
		, Stencil = GL_STENCIL_BUFFER_BIT

		, ColorDepth   = Color | Depth
		, ColorStencil = Color | Stencil
		, DepthStencil = Depth | Stencil

		, ColorDepthStencil = Color | Depth | Stencil
	};


	enum class FramebufferClearBufferfv : GLenum
	{
		  Color = GL_COLOR
		, Depth = GL_DEPTH
	};

	enum class FramebufferClearBufferiv : GLenum
	{
		  Color   = GL_COLOR
		, Stencil = GL_STENCIL
	};

	enum class FramebufferClearBufferuiv : GLenum
	{
		Color = GL_COLOR
	};

	enum class FramebufferClearBufferfi : GLenum
	{
		DepthStencil = GL_DEPTH_STENCIL
	};

	using BufferNumber = GLint;


	using FramebufferAttachment = GLenum; // can have many so it can't be wrapped in enum class

	enum class FramebufferAttachments: GLenum
	{
		  Color        = GL_COLOR_ATTACHMENT0 
		, Depth        = GL_DEPTH_ATTACHMENT
		, Stencil      = GL_STENCIL_ATTACHMENT
		, DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};


	using DrawBuffer = GLenum; // can have many so it can't be wrapped in enum class

	enum class DefaultDrawBuffer : GLenum
	{
		  None         = GL_NONE
		, FrontLeft    = GL_FRONT_LEFT
		, FrontRight   = GL_FRONT_RIGHT
		, BackLeft     = GL_BACK_LEFT
		, BackRight    = GL_BACK_RIGHT
		, Front        = GL_FRONT
		, Back         = GL_BACK
		, Left         = GL_LEFT
		, Right        = GL_RIGHT
		, FrontAndBack = GL_FRONT_AND_BACK
	};

	enum class FramebufferDrawBuffer : GLenum
	{
		  None             = GL_NONE
		, ColorAttachment0 = GL_COLOR_ATTACHMENT0
	};


	enum class FramebufferStatus : GLenum
	{
		  Complete               = GL_FRAMEBUFFER_COMPLETE
		, Undefined              = GL_FRAMEBUFFER_UNDEFINED
		, IncompleteAttachment   = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
		, IncompleteDrawBuffer   = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
		, IncompleteReadBuffer   = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
		, Unsupported            = GL_FRAMEBUFFER_UNSUPPORTED
		, IncompleteMultisample  = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
		, IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
	};

	enum class FramebufferFilter : GLenum
	{
		  Nearest = GL_NEAREST
		, Linear  = GL_LINEAR
	};

	enum class FramebufferParameteri : GLenum
	{
		  DefaultWidth                = GL_FRAMEBUFFER_DEFAULT_WIDTH
		, DefaultHeight               = GL_FRAMEBUFFER_DEFAULT_HEIGHT
		, DefaultLayers               = GL_FRAMEBUFFER_DEFAULT_LAYERS
		, DefaultSamples              = GL_FRAMEBUFFER_DEFAULT_SAMPLES
		, DefaultFixedSampleLocations = GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS
	};


	bool initialize();

	namespace state
	{
		//state management
		void blendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

		void blendEquation(BlendMode mode);

		void blendEquationi(BufferIndex number, BlendMode mode);

		void blendEquationSeparate(BlendMode rgb, BlendMode alpha);

		void blendEquationSeparatei(BufferIndex number, BlendMode rgb, BlendMode alpha);

		void blendFunc(BlendFactor srcFactor, BlendFactor dstFactor);

		void blendFunci(BufferIndex number, BlendFactor srcFactor, BlendFactor dstFactor);

		void blendFuncSeparate(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha);

		void blendFuncSeparatei(BufferIndex number, BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha);


		void cullFace(Face mode);


		void depthFunc(DepthFunc func);


		void enable(Capability parameter);

		void enablei(Capability parameter, CapabalityIndex index);

		void disable(Capability parameter);

		void disablei(Capability parameter, CapabalityIndex index);


		void frontFace(FrontFace mode);


		void polygonMode(Face face, PolygonMode mode);


		void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
	};
}