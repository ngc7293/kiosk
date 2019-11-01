cmake_minimum_required(VERSION 3.1.0)
project(kiosk)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set( SOURCES
src/app.cc
src/main.cc
)

SET( HEADERS
inc/app.h
)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# Qt5
set (CMAKE_PREFIX_PATH /opt/Qt/5.12.1/gcc_64/)
find_package(Qt5 COMPONENTS Core Widgets Network WebEngine WebEngineWidgets REQUIRED)

add_executable(kiosk ${SOURCES} ${HEADERS})
target_link_libraries(kiosk Qt5::Widgets Qt5::Network Qt5::WebEngine Qt5::WebEngineWidgets)
target_include_directories(kiosk PRIVATE inc)
