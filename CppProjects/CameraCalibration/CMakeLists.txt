cmake_minimum_required(VERSION 3.1)

project(CameraCalibration CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(OpenCV REQUIRED)

file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h")
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui")
file(GLOB RC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.qrc")

#QT5_ADD_RESOURCES(ENTITY_DEBUGGER_RC_SRCS ${RC_FILES} )
qt5_wrap_ui( ENTITY_DEBUGGER_UI_HDRS ${UI_FILES})
qt5_wrap_cpp( GUI_SRC ${HEADER_FILES})

add_executable( ${PROJECT_NAME}
                ${CPP_FILES}
                ${HEADER_FILES}
                ${UI_FILES}
                ${RC_FILES}
                ${GUI_SRC}
              )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
qt5_use_modules(${PROJECT_NAME} Widgets Core Gui)

