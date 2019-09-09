SOURCES = main.cpp mainwindow.cpp glframe.cpp camera.cpp gamecube.cpp
HEADERS = mainwindow.h glframe.h utils.h camera.h gamecube.h vec3.h
TARGET = piskvorky
QT += opengl
LIBS += -lGLU
CONFIG += qt opengl release
TEMPLATE = app
