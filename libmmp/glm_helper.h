#if !defined(LIBMMP_GLM_HELPER_H)
#define LIBMMP_GLM_HELPER_H

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <iosfwd>

std::ostream& operator<<(std::ostream&, const glm::vec3&);
std::ostream& operator<<(std::ostream&, const glm::vec4&);

//! mutual conversion between Direct3D and OpenGL
void flip_z(glm::quat&);

#endif /* LIBMMP_GLM_HELPER_H */
