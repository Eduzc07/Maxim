from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    # Ruta absoluta al archivo de parámetros YAML
    params_file = os.path.join(
        get_package_share_directory('copter'),
        'parameters',
        'params.yaml'
    )

    return LaunchDescription([
        # Parámetros globales (para nodos que los usen)
        Node(
            package='batmon',
            executable='batmon_node.py',
            name='batmon',
            output='screen'
        ),
        Node(
            package='state_estimation',
            executable='ekf_node',
            name='ekf_node',
            output='screen',
            parameters=[params_file]
        ),
        # Descomentar si es necesario
        # Node(
        #     package='mpu6050',
        #     executable='mpu6050_node',
        #     name='mpu6050_node',
        #     output='screen'
        # ),

        # Node(
        #     package='control',
        #     executable='control_node',
        #     name='control_node'
        # ),

        Node(
            package='quad_viz_pub',
            executable='quad_viz_pub_node',
            name='quad_viz_pub_node',
            output='screen'
        ),

        Node(
            package='force_torque2pwm',
            executable='force_torque2pwm_node',
            name='force_torque2pwm_node',
            output='screen'
        ),

        Node(
            package='xmcprotocol',
            executable='serialTransmit',
            name='serialTransmit',
            output='screen'
        ),

        Node(
            package='xmcprotocol',
            executable='serialReceive',
            name='serialReceive',
            output='screen'
        ),
    ])