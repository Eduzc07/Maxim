cmake_minimum_required(VERSION 3.5)
project(quad_viz_pub)

# Activar C++14 o superior
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Buscar dependencias
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(copter REQUIRED)

# Ejecutable
add_executable(quad_viz_pub_node src/quad_viz_pub_node.cpp)

# Dependencias del ejecutable
ament_target_dependencies(quad_viz_pub_node
  rclcpp
  visualization_msgs
  geometry_msgs
  std_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  copter
)

# Instalar ejecutable
install(TARGETS
  quad_viz_pub_node
  DESTINATION lib/${PROJECT_NAME}
)

# ✅ Instalar el archivo conf.rviz correctamente
install(FILES
  conf.rviz
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY meshes
  DESTINATION share/${PROJECT_NAME}
)

ament_package()