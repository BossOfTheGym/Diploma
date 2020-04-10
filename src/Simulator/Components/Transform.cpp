#include "Transform.h"


namespace comp
{
	Mat4f32 Transform::toMat() const
	{
		Mat4f32 res = glm::scale(glm::toMat4(rotation), scale);
		res[3][0] += translate.x;
		res[3][1] += translate.y;
		res[3][2] += translate.z;
		return res;
	}
}