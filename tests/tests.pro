TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

linux: LIBS += -pthread

INCLUDEPATH += \
  googletest/googletest/include \
  googletest/googletest \
  googletest/googlemock/include \
  googletest/googlemock \
  ../src

SOURCES += \
        googletest/googletest/src/gtest-all.cc \
        googletest/googlemock/src/gmock-all.cc \
        main.cpp \
        test_point.cpp
