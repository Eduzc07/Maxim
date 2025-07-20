from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    copter_params = os.path.join(
        get_package_share_directory('copter'),
        'parameters',
        'params.yaml'
    )

    rviz_config = os.path.join(
        get_package_share_directory('quad_viz_pub'),
        'conf.rviz'
    )

    return LaunchDescription([
        # Cargar parámetros
        # Node(
        #     package='copter',
        #     executable='parameter_loader',  # Asegúrate de tener un nodo si quieres cargar YAML (opcional)
        #     name='parameter_loader',
        #     parameters=[copter_params],
        #     output='screen'
        # ),

        # Nodo model
        Node(
            package='model',
            executable='model_node',
            name='model_node',
            parameters=[copter_params],
            output='screen'
        ),

        # Nodo control
        Node(
            package='control',
            executable='control_node',
            name='control_node',
            parameters=[copter_params],
            output='screen'
        ),

        # Nodo de visualización
        Node(
            package='quad_viz_pub',
            executable='quad_viz_pub_node',
            name='quad_viz_pub_node',
            output='screen'
        ),

        # Static transform publisher
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='world_frame',
            arguments=[
                '--x', '0.0',
                '--y', '0.0',
                '--z', '0.0',
                '--qx', '0.0',
                '--qy', '0.0',
                '--qz', '0.0',
                '--qw', '1.0',
                '--frame-id', 'map',
                '--child-frame-id', 'world_frame'
            ],
            output='screen'
        ),

        # Lanzar RViz con configuración personalizada
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_config],
            output='screen'
        )
    ])