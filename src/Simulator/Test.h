#pragma once

#include <Math/MathLib.h>

namespace test
{
	using namespace math;

	const Mat4f32 TEST_VIEW = glm::lookAt(Vec3f32{30}, Vec3f32{}, Vec3f32{0.0, 1.0, 0.0});

	// gravitation constant
	const Float G = 1.0;

	// Planet
	const Float PLANET_R    = 6378.0;
	const Float PLANET_MASS = 5.97e24;
	const Float PLANET_MU   = 398600.0; // almost G * M

	const Float PLANET_G0 = PLANET_MU / PLANET_R / PLANET_R;

	const Vec3 PLANET_RAD = Vec3{0.0};
	const Vec3 PLANET_VEL = Vec3{0.0};
	const Vec3 PLANET_ROT_AXIS = glm::normalize(Vec3(0.0, 1.0, 0.0));
	const Float PLANET_ROT_ANGLE = 0.01;

	const Vec3f32 PLANET_SCALE     = Vec3{PLANET_R};
	const Quatf32 PLANET_ROTATION = glm::angleAxis(0.0, PLANET_ROT_AXIS);
	const Vec3f32 PLANET_TRANSLATE = Vec3{0.0};

	// Target
	const Float TAR_MASS = 1000.0;

	const Vec3f32 TAR_COLOR = Vec3f32(1.0, 0.5, 0.25);

	const Vec3 TAR_RAD = Vec3{PLANET_R + 400.0, 0.0, 0.0};
	const Vec3 TAR_VEL = Vec3{0.0, 0.0, std::sqrt(PLANET_MU / glm::length(TAR_RAD))};
	const Vec3 TAR_ROT_AXIS = glm::normalize(Vec3{1.0, 1.0, 1.0});
	const Float TAR_ROT_ANGLE = 1.0;

	const Vec3f32 TAR_SCALE     = Vec3{1.0};
	const Quatf32 TAR_ROTATION  = glm::angleAxis(0.0, TAR_ROT_AXIS);
	const Vec3f32 TAR_TRANSLATE = TAR_RAD;

	const Float PROP_ISP  = 200.0;
	const Float PROP_MASS = 10.0;

	// Chaser
	const Float CHA_MASS = 1000.0;

	const Vec3f32 CHA_COLOR = Vec3f32(1.0, 0.5, 0.25);

	const Vec3 CHA_RAD = Vec3{-PLANET_R + 400.0, 0.0, 0.0};
	const Vec3 CHA_VEL = Vec3{0.0, 0.0, -std::sqrt(PLANET_MU / glm::length(CHA_RAD))};
	const Vec3 CHA_ROT_AXIS = glm::normalize(Vec3{1.0, 1.0, 1.0});
	const Float CHA_ROT_ANGLE = 2.0;

	const Vec3f32 CHA_SCALE     = Vec3{1.0};
	const Quatf32 CHA_ROTATION  = glm::angleAxis(0.0, CHA_ROT_AXIS);
	const Vec3f32 CHA_TRANSLATE = CHA_RAD;
}