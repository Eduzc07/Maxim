cmake_minimum_required(VERSION 3.8)
project(model)

# Opciones de compilación
add_compile_options(-Wall -Wextra -Wpedantic -O3)

# Dependencias de ROS 2
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(copter REQUIRED)

# Incluir directorios propios y de dependencias
include_directories(
  R2015b/simulink/include
  model_ert_rtw
)

# Archivos fuente
add_library(model
  model_ert_rtw/model.cpp
)

add_executable(model_node
  src/model_node.cpp
)

# Enlace de librerías
target_link_libraries(model_node
  model
)

# Incluir dependencias de ROS
ament_target_dependencies(model_node
  rclcpp
  copter
)

ament_target_dependencies(model
  rclcpp
  copter
)

# Instalación
install(TARGETS
  model
  model_node
  DESTINATION lib/${PROJECT_NAME}
)

# Si tienes headers que instalar luego puedes usar esto:
# install(DIRECTORY include/
#   DESTINATION include/
# )

# Exportación
ament_package()