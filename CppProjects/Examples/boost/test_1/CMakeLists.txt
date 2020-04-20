cmake_minimum_required(VERSION 2.8)
project (test_1)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
#find_package( Boost 1.61.0 COMPONENTS program_options REQUIRED)
find_package(Boost COMPONENTS locale system REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable( ${PROJECT_NAME} main.cpp)
    target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
endif()


#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

#add_executable(hello hello.cpp)
#target_link_libraries(hello ${Boost_LIBRARIES} )
