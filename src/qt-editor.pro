QT += qml quick gui

HEADERS += openglscene.h
SOURCES += openglscene.cpp main.cpp
#RESOURCES +=

target.path = $$[QT_INSTALL_EXAMPLES]/quick/scenegraph/openglunderqml
INSTALLS += target
