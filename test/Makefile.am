TESTS = glm_helper_test
check_PROGRAMS = glm_helper_test
glm_helper_test_SOURCES = glm_helper_test.cpp
glm_helper_test_CPPFLAGS = -I$(top_srcdir)/libmmp
glm_helper_test_LDADD = ../libmmp/libmmp.la -lm @opengl_LIBS@ -lSOIL -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody
