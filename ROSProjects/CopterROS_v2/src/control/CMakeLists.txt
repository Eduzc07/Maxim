cmake_minimum_required(VERSION 3.8)
project(control)

# Configura flags de compilación
add_compile_options(-Wall -Wextra -Wpedantic -O3)

# Encuentra dependencias
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(copter REQUIRED)

# Directorios de include
include_directories(
  control_ert_rtw
)

# Biblioteca del controlador
add_library(control
  control_ert_rtw/control.cpp
)

target_include_directories(control PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/control_ert_rtw>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(control
  rclcpp
  copter
)

# Nodo ejecutable
add_executable(control_node src/control_node.cpp)

target_link_libraries(control_node
  control
)

ament_target_dependencies(control_node
  rclcpp
  copter
)

# Instalación
install(TARGETS
  control
  control_node
  DESTINATION lib/${PROJECT_NAME}
)

# Si tienes headers puedes instalar así (opcional):
# install(DIRECTORY include/ DESTINATION include)

ament_package()