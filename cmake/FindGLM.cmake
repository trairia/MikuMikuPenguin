# - Locate GLM library
# This module defines
#  GLM_LIBRARY, the name of the library to link against
#  GLM_FOUND
#  GLM_INCLUDE_DIR, where to find glm/glm.hpp
# To Adding search path, set GLM_ROOT_DIR as follows
#    set(GLM_DIR "path/to/glm")
# or launch cmake with -DGLM_DIR="/path/to/GLM_DIR"
#
# author: Kazunori Kimura
# email : kazunori.abu@gmail.com

find_path(GLM_INCLUDE_DIR glm/glm.hpp
  HINTS ${GLM_DIR}
  PATH_SUFFIXES include
  )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM
  REQUIRED_VARS GLM_INCLUDE_DIR)