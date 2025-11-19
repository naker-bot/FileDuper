QT += core gui widgets network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileDuper
TEMPLATE = app
CONFIG += c++17

INCLUDEPATH += . include src

# Use wildcards for all sources and headers  
SOURCES += src/*.cpp
HEADERS += include/*.h

# Exclude problematic files
SOURCES -= src/advancedduplicatedialog.cpp
SOURCES -= src/intelligentftpclient.cpp
SOURCES -= src/lightningscan.cpp

# Compiler flags
QMAKE_CXXFLAGS += -O2 -flto -fno-fat-lto-objects
LIBS += -lcurl -lssh2 -lcrypto
