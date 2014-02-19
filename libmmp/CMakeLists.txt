set(libmmp_SRC
  bulletdebugdrawer.cpp
  bulletphysics.cpp
  glm_helper.cpp
  interpolation.cpp
  mmdphysics.cpp
  motioncontroller.cpp
  pmx.cpp
  shader.cpp
  texthandle.cpp
  vmd.cpp
  )

add_library(mmp STATIC ${libmmp_SRC})
target_link_libraries(mmp
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  ${GLFW_LIBRARY}
  ${GLEW_LIBRARY}
  ${BULLET_LIBRARIES})