QT       += core gui

TARGET = QPlayer
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES +=  src/common.cpp \
            src/ffmpeg.cpp \
            src/main.cpp \
            src/mainwindow.cpp \
            src/proto.cpp \
            src/sdl.cpp \
            src/timeline.cpp

HEADERS  += include/archive.hpp \
            include/common.hpp \
            include/ffmpeg.hpp \
            include/fun.hpp \
            include/fwd.hpp \
            include/mainwindow.hpp \
            include/proto.hpp \
            include/sdl.hpp \
            include/timeline.hpp

FORMS    += ui/mainwindow.ui

LIBS += -L/usr/local/lib/ -L/lib/x86_64-linux-gnu/ -lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale -lswresample -llzma -lm -ldl -lrt -lz

INCLUDEPATH += /usr/local/include/ /usr/include/ include/
DEPENDPATH += /usr/include/

#QMAKE_LFLAGS += -std=c++11
CONFIG += C++11
