# what flags you want to pass to the C compiler & linker
__top_builddir__pmx_CPPFLAGS = -c -DDATA_PATH=\"$(pkgdatadir)\" -I$(top_srcdir)/include @bullet_CXXFLAGS@ -std=c++0x
__top_builddir__pmx_LDFLAGS = -ggdb
__top_builddir__pmx_LDADD = ../libmmp/libmmp.la -lm @opengl_LIBS@ -lSOIL -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = $(top_builddir)/pmx
__top_builddir__pmx_SOURCES = main.cpp pmxvLogger.cpp viewer.cpp viewer.h pmxvLogger.h glfw_func_callbacks.cpp glfw_func_callbacks.h


if ENABLE_SOUND
__top_builddir__pmx_LDADD += -lSDL2 -lSDL2_mixer
__top_builddir__pmx_SOURCES += sound.h sound.cpp
else
__top_builddir__pmx_SOURCES += dummysound.h dummysound.cpp
endif
