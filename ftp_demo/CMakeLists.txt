cmake_minimum_required(VERSION 3.21)
project(FileDuper_FTP_Demo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)

# Nur ftpdirectorydialog - das ist der echte Crash Prevention Code
set(SOURCES
    ../src/main_minimal.cpp
    ../src/ftpdirectorydialog.cpp
)

add_executable(FileDuper_Demo ${SOURCES})

target_link_libraries(FileDuper_Demo PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network
)

target_include_directories(FileDuper_Demo PRIVATE ../include)
