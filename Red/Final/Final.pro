TEMPLATE = app
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17 -pthread
QMAKE_CFLAGS += -std=c++17 -pthread
LIBS += -pthread
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    parse.cpp \
    search_server.cpp \
    invertedindex.cpp \
    splitintowords.cpp \
    new_profile.cpp

HEADERS += \
    iterator_range.h \
    parse.h \
    search_server.h \
    test_runner.h \
    invertedindex.h \
    new_profile.h

DISTFILES +=
