#include "rclcpp/rclcpp.hpp"
#include "copter/msg/force_torque.hpp"
#include "copter/msg/state.hpp"

#include "model.h"
#include "rtwtypes.h"
#include <vector>

using std::placeholders::_1;

class ModelNode : public rclcpp::Node {
public:
    ModelNode() : Node("model_node") {
    this->declare_parameter<double>("mass", 1.0);
    this->declare_parameter<double>("gravity", 9.81);
    this->declare_parameter<double>("Jxx", 0.01);
    this->declare_parameter<double>("Jyy", 0.01);
    this->declare_parameter<double>("Jzz", 0.02);
    this->declare_parameter<std::vector<double>>("state_init", std::vector<double>(12, 0.0));

    // Obtener parámetros
    this->get_parameter("mass", params.mass);
    this->get_parameter("gravity", params.gravity);
    this->get_parameter("Jxx", params.Jxx);
    this->get_parameter("Jyy", params.Jyy);
    this->get_parameter("Jzz", params.Jzz);
    this->get_parameter("state_init", params.state_init);

    // Escalares
    RCLCPP_INFO(this->get_logger(), "mass: %f", params.mass);
    RCLCPP_INFO(this->get_logger(), "gravity: %f", params.gravity);
    RCLCPP_INFO(this->get_logger(), "Jxx: %f", params.Jxx);
    RCLCPP_INFO(this->get_logger(), "Jyy: %f", params.Jyy);
    RCLCPP_INFO(this->get_logger(), "Jzz: %f", params.Jzz);

    // Vector
    std::ostringstream oss;
    oss << "state_init: [";
    for (size_t i = 0; i < params.state_init.size(); ++i) {
        oss << params.state_init[i];
        if (i < params.state_init.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());

    // Subscripción y publicación
    force_torque_sub_ = this->create_subscription<copter::msg::ForceTorque>(
        "/Force_Torque", 10, std::bind(&ModelNode::input_callback, this, _1));

    state_pub_ = this->create_publisher<copter::msg::State>("/State", 10);

    rtObj.initialize();
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(5),
        std::bind(&ModelNode::update_loop, this));
  }

private:
  void input_callback(const copter::msg::ForceTorque::SharedPtr msg) {
    input.tau_x = msg->tau_x;
    input.tau_y = msg->tau_y;
    input.tau_z = msg->tau_z;
    input.thrust = msg->thrust;
  }

  void update_loop() {
    rtObj.rtU.g = params.gravity;
    rtObj.rtU.m = params.mass;
    rtObj.rtU.Jxx = params.Jxx;
    rtObj.rtU.Jyy = params.Jyy;
    rtObj.rtU.Jzz = params.Jzz;
    rtObj.rtU.tau_x = input.tau_x;
    rtObj.rtU.tau_y = input.tau_y;
    rtObj.rtU.tau_z = input.tau_z;
    rtObj.rtU.thrust = input.thrust;

    rtObj.step();

    for (int i = 0; i < 12; i++) {
      state[i] = rtObj.rtY.state[i];
    }

    auto msg = copter::msg::State();
    msg.position.linear.x = state[0];
    msg.position.linear.y = state[1];
    msg.position.linear.z = state[2];
    msg.position.angular.x = state[3];
    msg.position.angular.y = state[4];
    msg.position.angular.z = state[5];
    msg.velocity.linear.x = state[6];
    msg.velocity.linear.y = state[7];
    msg.velocity.linear.z = state[8];
    msg.velocity.angular.x = state[9];
    msg.velocity.angular.y = state[10];
    msg.velocity.angular.z = state[11];

    state_pub_->publish(msg);
  }

  // Estructuras internas
  struct Input {
    double tau_x = 0.0;
    double tau_y = 0.0;
    double tau_z = 0.0;
    double thrust = 0.0;
  } input;

  struct Params {
    double mass = 1.0;
    double gravity = 9.81;
    double Jxx = 0.01;
    double Jyy = 0.01;
    double Jzz = 0.02;
    std::vector<double> state_init;
  } params;

  // static Input input;
  // static Params params;

  modelModelClass rtObj;
  double state[12] = {0.0};

  rclcpp::Subscription<copter::msg::ForceTorque>::SharedPtr force_torque_sub_;
  rclcpp::Publisher<copter::msg::State>::SharedPtr state_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ModelNode>());
  rclcpp::shutdown();
  return 0;
}