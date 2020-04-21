#pragma once

#include "../Config/Config.h"

#include <cmath>
#include <limits>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace math
{
	namespace detail
	{
		template<class FloatType>
		constexpr FloatType default_eps_value();

		template<>
		constexpr float default_eps_value<float>()
		{
			return 5e-6f;
		}

		template<>
		constexpr double default_eps_value<double>()
		{
			return 5e-15;
		}
	}

	using Int    = int;
	using Int32  = std::int32_t;
	using Int64  = std::int64_t;
	using UInt32 = std::uint32_t;
	using UInt64 = std::uint64_t;

	using config::Float;

	using F32 = float;
	using F64 = double;

	Float operator "" _FL(long double value);	

	const Float EPS = detail::default_eps_value<Float>();
	const Float SQ2 = std::sqrt(2.0_FL);
	const Float SQ3 = std::sqrt(3.0_FL);

	const Float PI = static_cast<Float>(std::acos(-1.0_FL));
	const Float PI2 = 2 * PI;
	const Float PI_2 = PI / 2;
	const Float PI_3 = PI / 3;
	const Float PI_4 = PI / 4;
	const Float PI_5 = PI / 5;
	const Float PI_6 = PI / 6;
	const Float PI_7 = PI / 7;
	const Float PI_8 = PI / 8;



	using Vec4 = glm::tvec4<Float>;
	using Mat4 = glm::tmat4x4<Float>;

	using Vec3 = glm::tvec3<Float>;
	using Mat3 = glm::tmat3x3<Float>;

	using Vec2 = glm::tvec2<Float>;
	using Mat2 = glm::tmat2x2<Float>;

	using Quat = glm::tquat<Float>;


	using Vec4f32 = glm::tvec4<float>;
	using Mat4f32 = glm::tmat4x4<float>;

	using Vec3f32 = glm::tvec3<float>;
	using Mat3f32 = glm::tmat3x3<float>;

	using Vec2f32 = glm::tvec2<float>;
	using Mat2f32 = glm::tmat2x2<float>;

	using Quatf32 = glm::tquat<float>;


	using Vec4f64 = glm::tvec4<double>;
	using Mat4f64 = glm::tmat4x4<double>;

	using Vec3f64 = glm::tvec3<double>;
	using Mat3f64 = glm::tmat3x3<double>;

	using Vec2f64 = glm::tvec2<double>;
	using Mat2f64 = glm::tmat2x2<double>;

	using Quatf64 = glm::tquat<double>;


	using Vec3i = glm::tvec3<Int>;

	using Vec3i32 = glm::tvec3<Int32>;
	using Vec3u32 = glm::tvec3<Int32>;

	using Vec3i64 = glm::tvec3<Int64>;
	using Vec3u64 = glm::tvec3<Int64>;	


	using Vec2i = glm::tvec2<Int>;

	using Vec2i32 = glm::tvec2<Int32>;
	using Vec2u32 = glm::tvec2<Int32>;

	using Vec2i64 = glm::tvec2<Int64>;
	using Vec2u64 = glm::tvec2<Int64>;	
}
