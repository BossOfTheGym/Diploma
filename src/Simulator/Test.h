#pragma once

#include <Math/MathLib.h>

namespace test
{
	using namespace math;

	const Mat4f32 TEST_VIEW = glm::lookAt(Vec3f32{30}, Vec3f32{}, Vec3f32{0.0, 1.0, 0.0});

	const Float PLANET_R    = 10.0;
	const Float PLANET_MU   = 1000.0;
	const Float PLANET_MASS = 1.0;



	const Vec3 PLANET_RAD = Vec3{0.0};
	const Vec3 PLANET_VEL = Vec3{0.0};
	const Vec3 PLANET_ROT_AXIS = Vec3(0.0, 1.0, 0.0);
	const Float PLANET_ROT_ANGLE = 0.5;

	const Vec3f32 PLANET_SCALE     = Vec3{PLANET_R};
	const Quatf32 PLANET_ROTATTION = glm::angleAxis(0.0, PLANET_ROT_AXIS);
	const Vec3f32 PLANET_TRANSLATE = Vec3{0.0};


	const Float SAT_MASS = 1.0;

	const Vec3f32 SAT_COLOR = Vec3f32(1.0, 0.5, 0.25);

	const Vec3 SAT_RAD = Vec3{2 * PLANET_R, 0.0, 0.0};
	const Vec3 SAT_VEL = Vec3{0.0, 0.0, 7.0};
	const Vec3 SAT_ROT_AXIS = glm::normalize(Vec3{1.0, 1.0, 1.0});
	const Float SAT_ROT_ANGLE = 0.5;

	const Vec3f32 SAT_SCALE     = Vec3{1.0};
	const Quatf32 SAT_ROTATION  = glm::angleAxis(0.0, SAT_ROT_AXIS);
	const Vec3f32 SAT_TRANSLATE = SAT_RAD;
}