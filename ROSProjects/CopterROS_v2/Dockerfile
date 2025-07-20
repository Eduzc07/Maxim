# Definimos la distribución de ROS como un argumento externo
ARG ROS_DISTRO=jazzy
FROM osrf/ros:${ROS_DISTRO}-desktop

# Usa bash como shell por defecto
SHELL ["/bin/bash", "-c"]

# Configuración regional
RUN apt update && apt install -y locales && \
    locale-gen en_US en_US.UTF-8 && \
    update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8

ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8

# Herramientas útiles
RUN apt update && apt install -y \
    python3-colcon-common-extensions \
    python3-pip \
    ros-dev-tools \
    ros-${ROS_DISTRO}-turtlesim \
    x11-apps \
    git curl wget \
    && apt clean

# Workspace por defecto
WORKDIR /root/ros2_ws

# Entrypoint para cargar ROS automáticamente
#ENTRYPOINT ["/ros_entrypoint.sh"]
#CMD ["bash"]

# Sourcing automático
RUN echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
CMD ["bash"]