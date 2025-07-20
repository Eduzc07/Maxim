from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Nodo batmon
        Node(
            package='batmon',
            executable='batmon_node.py',
            name='batmon',
            output='screen'
        ),

        # Nodo IMU MPU6050
        Node(
            package='mpu6050',
            executable='mpu6050_node',
            name='mpu',
            output='screen'
        ),

        # Comunicación con XMC
        Node(
            package='xmcprotocol',
            executable='serialTransmit',
            name='serialTransmit',
            output='screen'
        ),
        Node(
            package='xmcprotocol',
            executable='exampleList',
            name='exampleList',
            output='screen'
        ),
        Node(
            package='xmcprotocol',
            executable='serialReceive',
            name='serialReceive',
            output='screen'
        ),
    ])