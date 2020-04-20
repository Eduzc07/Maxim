cmake_minimum_required(VERSION 2.8)
project(test_1)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#find_package(Qt5Core REQUIRED)
#find_package(Boost COMPONENTS system serialization REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h")
#file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
#file(GLOB RC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc")

add_executable(${PROJECT_NAME}
                                ${CPP_FILES}
                                ${HEADER_FILES}
                                )

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS})
