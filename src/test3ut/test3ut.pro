include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += \
     $$PWD/../test3

SOURCES += \
        ../test3/util.cpp \
        main.cpp         tst_test3tcase1.cpp
