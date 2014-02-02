#include "glm_helper.h"
#include <iostream>

std::ostream& operator<<(std::ostream& rOut, const glm::vec3& v)
{
    rOut << v[0] << ", " << v[1] << ", " << v[2];
    return rOut;
}

std::ostream& operator<<(std::ostream& rOut, const glm::vec4& v)
{
    rOut << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3];
    return rOut;
}
