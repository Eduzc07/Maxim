cmake_minimum_required(VERSION 3.5)
project(trajectory)

# Requiere C++14 o superior para compatibilidad con ROS 2 y rclcpp
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encuentra dependencias de ROS 2
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(copter REQUIRED)  # Debe existir como paquete de ROS 2
find_package(Eigen3 REQUIRED)

# Incluir directorios
include_directories(
  include/Symbolic
  ${EIGEN3_INCLUDE_DIRS}
)

# Declara ejecutable
add_executable(trajectory_node src/trajectory.cpp)

# Vincula dependencias
ament_target_dependencies(trajectory_node
  rclcpp
  Eigen3
  copter
)

# Instalación
install(TARGETS
  trajectory_node
  DESTINATION lib/${PROJECT_NAME}
)

# Exportar el tipo de build para ament
ament_package()