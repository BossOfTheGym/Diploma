#include "Camera.h"

#include <algorithm>
#include <iostream>

namespace comp
{
	Camera3rdPerson::Camera3rdPerson(
		  const Vec3f32& center

		, F32 pitchLimit

		, F32 initialPitch
		, F32 initialRotation

		, F32 initialDistance
		, F32 minDistance
		, F32 maxDistance
	)
		: m_center(center)

		, m_dist(initialDistance)
		, m_minDist(minDistance)
		, m_maxDist(maxDistance)
	{
		pitchLimit = pitchLimit - std::floor(pitchLimit / math::PI_2) * math::PI_2;
		pitchLimit = std::abs(pitchLimit);
		pitchLimit = (pitchLimit > math::EPS ? pitchLimit : math::PI_4);
		m_pitchLimit = pitchLimit;

		m_pitch    = std::min(std::max(initialPitch, -m_pitchLimit), m_pitchLimit);
		m_rotation = initialRotation - std::floor(initialRotation / math::PI2) * math::PI2;
		
		F32 minDist = std::abs(minDistance);
		F32 maxDist = std::abs(maxDistance);
		m_minDist = std::min(minDist, maxDist);
		m_maxDist = std::max(minDist, maxDist);
		m_dist    = std::min(std::max(std::abs(initialDistance), m_minDist), m_maxDist); 

		updateCamera();
	}


	void Camera3rdPerson::setCenter(const Vec3f32& center)
	{
		m_center = center;
		m_needUpdate = true;
	}

	void Camera3rdPerson::setAxes(const Mat3f32& axes)
	{
		m_axes = axes;
	}


	void Camera3rdPerson::travelDir(const Vec3f32& dir)
	{
		m_center += dir;
		m_needUpdate = true;
	}

	void Camera3rdPerson::changeDistance(F32 distDelta)
	{
		m_dist += distDelta;
		m_dist = std::min(std::max(m_dist, m_minDist), m_maxDist);
		m_needUpdate = true;
	}

	void Camera3rdPerson::rotateAround(F32 angleDelta)
	{
		m_rotation += angleDelta;
		m_rotation = m_rotation - std::floor(m_rotation / math::PI2) * math::PI2;
		m_needUpdate = true;
	}

	void Camera3rdPerson::rotatePitch(F32 angleDelta)
	{
		m_pitch += angleDelta;
		m_pitch = std::min(std::max(m_pitch, -m_pitchLimit), m_pitchLimit);
		m_needUpdate = true;
	}

	void Camera3rdPerson::resetAxes()
	{
		m_axes = Mat3f32{1.0};
	}


	const Mat3f32& Camera3rdPerson::getAxes()
	{
		return m_axes;
	}

	const Vec3f32& Camera3rdPerson::getPosition()
	{
		if (m_needUpdate)
		{
			updateCamera();
		}
		return m_position;
	}

	const Vec3f32& Camera3rdPerson::getCenter()
	{
		return m_center;
	}

	const Mat4f32& Camera3rdPerson::viewMat()
	{
		if (m_needUpdate)
		{
			updateCamera();
		}
		return m_camera;
	}


	void Camera3rdPerson::setMinDist(F32 minDist)
	{
		minDist = std::abs(minDist);
		m_minDist = std::min(minDist, m_maxDist);
		m_maxDist = std::max(minDist, m_maxDist);
	}

	void Camera3rdPerson::setMaxDist(F32 maxDist)
	{
		maxDist = std::abs(maxDist);
		m_minDist = std::min(maxDist, m_maxDist);
		m_maxDist = std::max(maxDist, m_maxDist);
	}

	void Camera3rdPerson::updateCamera()
	{
		F32 cosP = std::cos(m_pitch);
		F32 sinP = std::sin(m_pitch);
		F32 cosR = std::cos(m_rotation);
		F32 sinR = std::sin(m_rotation);

		Vec3f32 r = m_axes[0] * (cosP * cosR) + m_axes[1] * (sinP) + m_axes[2] * (cosP * sinR);
		Vec3f32 p = glm::normalize(glm::cross(m_axes[1], r)); // -pitch
		Vec3f32 y = glm::normalize(glm::cross(r, p)); // yaw

		m_position = r * m_dist + m_center;

		m_camera[0][0] = p.x ; m_camera[1][0] = p.y ; m_camera[2][0] = p.z ; m_camera[3][0] = -glm::dot(m_position, p);
		m_camera[0][1] = y.x ; m_camera[1][1] = y.y ; m_camera[2][1] = y.z ; m_camera[3][1] = -glm::dot(m_position, y);
		m_camera[0][2] = r.x ; m_camera[1][2] = r.y ; m_camera[2][2] = r.z ; m_camera[3][2] = -glm::dot(m_position, r);
		m_camera[0][3] = 0.0f; m_camera[1][3] = 0.0f; m_camera[2][3] = 0.0f; m_camera[3][3] = 1.0f;

		m_needUpdate = false;
	}
}
