cmake_minimum_required(VERSION 3.15)
project( test_2 )

find_package( OpenCV REQUIRED )

#add_executable( Test main.cpp )
#target_link_libraries( Test ${OpenCV_LIBS} )

find_package(Boost COMPONENTS system serialization REQUIRED )

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable( ${PROJECT_NAME} main.cpp)
    target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES}  ${OpenCV_LIBS} )
endif()



