#pragma once

#include <Math/MathLib.h>

namespace comp
{
	using math::F32;

	using math::Vec3f32;
	using math::Vec4f32;

	using math::Mat3f32;
	using math::Mat4f32;

	using math::Quatf32;


	// rotation is calculated from the x axis
	// pitch is calculated from xz plane
	class Camera3rdPerson
	{
	public:
		Camera3rdPerson(
			  const Vec3f32& center = Vec3f32(0.0f)

			, F32 pitchLimit = math::PI_4

			, F32 initialPitch    = 0.0f
			, F32 initialRotation = 0.0f

			, F32 initialDistance = 1.0f
			, F32 minDistance = 0.5f
			, F32 maxDistance = 2.0f
		);
		
		Camera3rdPerson(const Camera3rdPerson&) = default;
		Camera3rdPerson(Camera3rdPerson&&)      = default;

		~Camera3rdPerson() = default;

		Camera3rdPerson& operator = (const Camera3rdPerson&) = default;
		Camera3rdPerson& operator = (Camera3rdPerson&&)      = default;


	public:
		void setCenter(const Vec3f32& center);

		void setAxes(const Mat3f32& axes);


		void travelDir(const Vec3f32& dir);

		void changeDistance(F32 distDelta);

		void rotateAround(F32 angleDelta);

		void rotatePitch(F32 angleDelta);

		void resetAxes();


		const Mat3f32& getAxes();

		const Vec3f32& getPosition();

		const Vec3f32& getCenter();

		const Mat4f32& viewMat();


	public:	
		void setMinDist(F32 minDist);

		void setMaxDist(F32 maxDist);


	private:
		void updateCamera();


	private:
		Mat4f32 m_camera{1.0f};

		Mat3f32 m_axes{1.0f};

		Vec3f32 m_center{0.0f};
		Vec3f32 m_position{0.0f};

		F32 m_pitchLimit{0.0f};

		F32 m_pitch{0.0f};
		F32 m_rotation{0.0f};

		F32 m_dist{0.0f};
		F32 m_minDist{0.0f};
		F32 m_maxDist{0.0f};

		bool m_needUpdate{false};
	};
}
