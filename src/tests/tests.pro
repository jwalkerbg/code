TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Logger.cpp \
        main.cpp \
        ringbuffer.c \
        singlepulsetimer.cpp
