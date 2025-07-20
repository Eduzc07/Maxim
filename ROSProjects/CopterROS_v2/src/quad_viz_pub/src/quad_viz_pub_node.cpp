#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include "copter/msg/state.hpp"
#include "copter/msg/position.hpp"
#include <tf2/LinearMath/Quaternion.h>

using std::placeholders::_1;

struct State {
  double x[3];   // position
  double a[3];   // angles
  double dx[3];  // velocity
  double da[3];  // angular velocity
};

class QuadVizNode : public rclcpp::Node {
public:
  QuadVizNode()
  : Node("quad_viz_pub") {

    marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("visualization_marker", 10);

    state_sub_ = this->create_subscription<copter::msg::State>(
      "/State", 10, std::bind(&QuadVizNode::stateUpdate, this, _1));
      
    position_sub_ = this->create_subscription<copter::msg::Position>(
      "/Position", 10, std::bind(&QuadVizNode::positionUpdate, this, _1));

    initializeMarkers();
    initializeWaypoints();

    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(20),  // 25 Hz
      std::bind(&QuadVizNode::publishMarkers, this)
    );

    trajectory_timer_ = this->create_wall_timer(
      std::chrono::milliseconds(200),  // 5 Hz
      std::bind(&QuadVizNode::publishTrajectories, this)
    );
  }

private:
  void stateUpdate(const copter::msg::State::SharedPtr msg) {
    for (int i = 0; i < 3; ++i) {
      quadrotorState_.x[i]  = (&msg->position.linear.x)[i];
      quadrotorState_.a[i]  = (&msg->position.angular.x)[i];
      quadrotorState_.dx[i] = (&msg->velocity.linear.x)[i];
      quadrotorState_.da[i] = (&msg->velocity.angular.x)[i];
    }
  }

  void positionUpdate(const copter::msg::Position::SharedPtr msg) {
    d_pos_[0] = msg->pos.x;
    d_pos_[1] = msg->pos.y;
    d_pos_[2] = msg->pos.z;
  }

  void initializeMarkers() {
    line_strip_.header.frame_id = "world_frame";
    line_strip_.ns = "Lines";
    line_strip_.id = 1;
    line_strip_.type = visualization_msgs::msg::Marker::LINE_STRIP;
    line_strip_.action = visualization_msgs::msg::Marker::ADD;
    line_strip_.scale.x = 0.03;
    line_strip_.color.r = 0.8f;
    line_strip_.color.a = 0.6;
    line_strip_.pose.orientation.w = 1.0;
    line_strip_.lifetime = rclcpp::Duration::from_seconds(0);

    line_des_.header.frame_id = "world_frame";
    line_des_.ns = "Trajectory";
    line_des_.id = 2;
    line_des_.type = visualization_msgs::msg::Marker::LINE_STRIP;
    line_des_.action = visualization_msgs::msg::Marker::ADD;
    line_des_.scale.x = 0.02;
    line_des_.color.b = 0.8f;
    line_des_.color.a = 0.6;
    line_des_.pose.orientation.w = 1.0;
    line_des_.lifetime = rclcpp::Duration::from_seconds(0);
  }

  void publishMarkers() {
    auto now = this->now();

    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "world_frame";
    marker.header.stamp = now;
    marker.ns = "stl_ns";
    marker.id = 0;
    marker.type = visualization_msgs::msg::Marker::MESH_RESOURCE;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.mesh_resource = "package://quad_viz_pub/meshes/quadrotor.stl";
    marker.mesh_use_embedded_materials = false;

    marker.pose.position.x = quadrotorState_.x[0] - 0.04;
    marker.pose.position.y = quadrotorState_.x[1] + 0.06;
    marker.pose.position.z = quadrotorState_.x[2] + 0.09;

    tf2::Quaternion q;
    q.setRPY(quadrotorState_.a[0], quadrotorState_.a[1], quadrotorState_.a[2]);
    marker.pose.orientation.x = q.x();
    marker.pose.orientation.y = q.y();
    marker.pose.orientation.z = q.z();
    marker.pose.orientation.w = q.w();

    marker.scale.x = 1.0e-2;
    marker.scale.y = 1.0e-2;
    marker.scale.z = 1.0e-2;
    // marker.scale.x = 0.01;
    // marker.scale.y = 0.01;
    // marker.scale.z = 0.01;

    marker.color.r = 0.0f;
    marker.color.g = 0.8f;
    marker.color.b = 0.0f;
    marker.color.a = 0.6;

    marker.lifetime = rclcpp::Duration::from_seconds(0);

    marker_pub_->publish(marker);

    // geometry_msgs::msg::Point p;

    // p.x = d_pos_[0];
    // p.y = d_pos_[1];
    // p.z = d_pos_[2];
    // line_des_.points.push_back(p);

    // p.x = quadrotorState_.x[0];
    // p.y = quadrotorState_.x[1];
    // p.z = quadrotorState_.x[2];
    // line_strip_.points.push_back(p);
    // // LÍNEAS - IDs 1 y 2 (ya están bien)
    // line_strip_.ns = "Lines";
    // line_strip_.id = 1;
    // marker_pub_->publish(line_des_);
    

    // // LÍNEAS - IDs 1 y 2 (ya están bien)
    // line_strip_.ns = "Trajectory";
    // line_strip_.id = 2;
    // marker_pub_->publish(line_strip_);
  }

  void publishTrajectories() {
    auto now = this->now();

    geometry_msgs::msg::Point p;

    // Deseada
    p.x = d_pos_[0];
    p.y = d_pos_[1];
    p.z = d_pos_[2];
    line_des_.points.push_back(p);

    // Real
    p.x = quadrotorState_.x[0];
    p.y = quadrotorState_.x[1];
    p.z = quadrotorState_.x[2];
    line_strip_.points.push_back(p);

    line_strip_.header.stamp = now;
    line_des_.header.stamp = now;

    marker_pub_->publish(line_strip_);
    marker_pub_->publish(line_des_);

    //Define este contador como variable miembro o `static` en tu función
    static int publish_counter = 0;
    const int MAX_PUBLISH_CYCLES = 30;  // Número de ciclos a publicar (p.ej. 30 frames)
  
    if (publish_counter < MAX_PUBLISH_CYCLES) {
      waypoint_marker_.header.stamp = now;
      marker_pub_->publish(waypoint_marker_);
      publish_counter++;
      RCLCPP_INFO(rclcpp::get_logger("publishTrajectories"), "Counts: %d", publish_counter);
    }
  }

  void initializeWaypoints() {
    // Lista de coordenadas en bruto
    std::vector<std::array<float, 3>> raw_coords = {
      { 0,  0, 2},
      { 2,  2, 2},
      { 2, -2, 2},
      {-2,  2, 2},
      {-2, -2, 2},
      { 0,  0, 2}
    };

    // Convertir a geometry_msgs::msg::Point
    for (const auto& coord : raw_coords) {
      geometry_msgs::msg::Point p;
      p.x = coord[0];
      p.y = coord[1];
      p.z = coord[2];
      waypoints_.push_back(p);
    }

    // Configurar el marker
    waypoint_marker_.header.frame_id = "world_frame";
    waypoint_marker_.ns = "waypoints";
    waypoint_marker_.id = 3;
    waypoint_marker_.type = visualization_msgs::msg::Marker::SPHERE_LIST;
    waypoint_marker_.action = visualization_msgs::msg::Marker::ADD;
    waypoint_marker_.scale.x = 0.1;
    waypoint_marker_.scale.y = 0.1;
    waypoint_marker_.scale.z = 0.1;
    waypoint_marker_.color.r = 0.2f;
    waypoint_marker_.color.g = 1.0f;
    waypoint_marker_.color.b = 0.2f;
    waypoint_marker_.color.a = 0.8;
    waypoint_marker_.pose.orientation.w = 1.0;
    waypoint_marker_.lifetime = rclcpp::Duration::from_seconds(0);

    // Agregar los puntos al marker
    waypoint_marker_.points = waypoints_;
  }

  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
  rclcpp::Subscription<copter::msg::State>::SharedPtr state_sub_;
  rclcpp::Subscription<copter::msg::Position>::SharedPtr position_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr trajectory_timer_;

  State quadrotorState_;
  double d_pos_[3] = {0.0, 0.0, 0.0};

  visualization_msgs::msg::Marker line_strip_;
  visualization_msgs::msg::Marker line_des_;
  std::vector<geometry_msgs::msg::Point> waypoints_;
  visualization_msgs::msg::Marker waypoint_marker_;

};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<QuadVizNode>());
  rclcpp::shutdown();
  return 0;
}