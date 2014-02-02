#if !defined(LIBMMP_GLM_HELPER_H)
#define LIBMMP_GLM_HELPER_H

#include <iosfwd>
#include <glm/glm.hpp>

std::ostream& operator<<(std::ostream&, const glm::vec3&);
std::ostream& operator<<(std::ostream&, const glm::vec4&);

#endif /* LIBMMP_GLM_HELPER_H */
