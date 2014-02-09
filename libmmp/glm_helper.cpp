#include "glm_helper.h"
#include <iostream>

namespace
{
    float safe_asin(float x)
    {
        return asin(glm::clamp<float>(x, -1, +1));
    }
}

std::ostream&
operator<<(std::ostream& rOut, const glm::vec3& v)
{
	rOut << v[0] << ", " << v[1] << ", " << v[2];
	return rOut;
}

std::ostream&
operator<<(std::ostream& rOut, const glm::vec4& v)
{
	rOut << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3];
	return rOut;
}

std::ostream&
operator<<(std::ostream& rOut, const glm::quat& v)
{
	rOut << v.w << ", " << v.x << ", " << v.y << ", " << v.z;
	return rOut;
}

void
flipZ(glm::quat& rQuat)
{
	rQuat.x *= -1;
	rQuat.y *= -1;
}

glm::vec3
toEulerAnglesRadians(const glm::quat& q)
{
	return glm::vec3(atan2(2*(q.y*q.z+q.w*q.x), 1-2*(q.x*q.x+q.y*q.y)),
					-safe_asin(2*(q.x*q.z-q.w*q.y)),
					atan2(2*(q.x*q.y+q.w*q.z), 1-2*(q.y*q.y+q.z*q.z)));
}

glm::quat
fromEulerAnglesRadians(const glm::vec3& eulerAngles)
{
	const glm::quat qx = fromAxisAngleRadians(glm::vec3(1, 0, 0), eulerAngles.x);
	const glm::quat qy = fromAxisAngleRadians(glm::vec3(0, 1, 0), eulerAngles.y);
	const glm::quat qz = fromAxisAngleRadians(glm::vec3(0, 0, 1), eulerAngles.z);
	return qz * qy * qx;
}

glm::quat
fromAxisAngleRadians(const glm::vec3& axis, float angle)
{
	const float c = cos(0.5 * angle);
	const float s = sin(0.5 * angle);
	const glm::vec3 n = glm::normalize(axis);
	return glm::quat(c, s * n.x, s * n.y, s * n.z);
}
