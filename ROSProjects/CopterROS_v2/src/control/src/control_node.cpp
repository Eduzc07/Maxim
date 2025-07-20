#include "rclcpp/rclcpp.hpp"
#include "copter/msg/force_torque.hpp"
#include "copter/msg/state.hpp"
#include "copter/msg/position.hpp"

#include "control.h"
#include "rtwtypes.h"

using std::placeholders::_1;

struct State {
  double x[3];
  double a[3];
  double dx[3];
  double da[3];
};

struct Output {
  double tau_x;
  double tau_y;
  double tau_z;
  double thrust;
  double F_des[3];
};

struct Params {
  double mass;
  double gravity;
  std::vector<double> x_C_des;
};

class ControlNode : public rclcpp::Node {
public:
  ControlNode() : Node("control_node") {
    // Parámetros
    this->declare_parameter("mass", 1.0);
    this->declare_parameter("gravity", 9.81);
    this->declare_parameter("x_C_des", std::vector<double>{0.0, 0.0, 1.0});
    this->get_parameter("mass", params_.mass);
    this->get_parameter("gravity", params_.gravity);
    this->get_parameter("x_C_des", params_.x_C_des);

    // Print
    RCLCPP_INFO(this->get_logger(), "Parámetro mass: %f", params_.mass);
    RCLCPP_INFO(this->get_logger(), "Parámetro gravity: %f", params_.gravity);

    std::ostringstream ss;
    ss << "Parámetro x_C_des: [";
    for (size_t i = 0; i < params_.x_C_des.size(); ++i) {
      ss << params_.x_C_des[i];
      if (i < params_.x_C_des.size() - 1) ss << ", ";
    }
    ss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", ss.str().c_str());

    // Publicador y suscriptores
    force_pub_ = this->create_publisher<copter::msg::ForceTorque>("/Force_Torque", 10);
    state_sub_ = this->create_subscription<copter::msg::State>("/State", 10, std::bind(&ControlNode::state_callback, this, _1));
    position_sub_ = this->create_subscription<copter::msg::Position>("/Position", 10, std::bind(&ControlNode::position_callback, this, _1));

    // Inicializar controlador
    control_obj_.initialize();

    // Timer para ejecutar a 200 Hz
    timer_ = this->create_wall_timer(std::chrono::milliseconds(5), std::bind(&ControlNode::step, this));
  }

private:
  void state_callback(const copter::msg::State::SharedPtr msg) {
    for (int i = 0; i < 3; i++) {
      quadrotor_state_.x[i] = (&msg->position.linear.x)[i];
      quadrotor_state_.a[i] = (&msg->position.angular.x)[i];
      quadrotor_state_.dx[i] = (&msg->velocity.linear.x)[i];
      quadrotor_state_.da[i] = (&msg->velocity.angular.x)[i];
    }
  }

  void position_callback(const copter::msg::Position::SharedPtr msg) {
    d_pos_[0] = msg->pos.x;
    d_pos_[1] = msg->pos.y;
    d_pos_[2] = msg->pos.z;

    d_vel_[0] = msg->vel.x;
    d_vel_[1] = msg->vel.y;
    d_vel_[2] = msg->vel.z;

    d_acc_[0] = msg->acc.x;
    d_acc_[1] = msg->acc.y;
    d_acc_[2] = msg->acc.z;

    RCLCPP_INFO(this->get_logger(), "d_pos: [%.3f, %.3f, %.3f] | d_vel: [%.3f, %.3f, %.3f] | d_acc: [%.3f, %.3f, %.3f]",
                                    d_pos_[0], d_pos_[1], d_pos_[2],
                                    d_vel_[0], d_vel_[1], d_vel_[2],
                                    d_acc_[0], d_acc_[1], d_acc_[2]
                                );
  }

  void step() {
    static bool overrun_flag = false;
    if (overrun_flag) return;
    overrun_flag = true;

    // Save FPU context here (if necessary)
    // Re-enable timer or interrupt here
    // Set model inputs here
    // Setting 3-dim vectors
    for (int i = 0; i < 3; i++) {
      control_obj_.control_U.r_T[i]    = d_pos_[i] + 0.0001;
      control_obj_.control_U.dr_T[i]   = d_vel_[i];
      control_obj_.control_U.ddr_T[i]  = d_acc_[i];
      control_obj_.control_U.x_C_des[i] = params_.x_C_des[i];
      control_obj_.control_U.B_w_BWT[i] = 0.0;

      // Setting the quadrocopter state from ros topic as input
      control_obj_.control_U.State[i]      = quadrotor_state_.x[i];
      control_obj_.control_U.State[i + 3]  = quadrotor_state_.a[i];
      control_obj_.control_U.State[i + 6]  = quadrotor_state_.dx[i];
      control_obj_.control_U.State[i + 9]  = quadrotor_state_.da[i];
    }

    control_obj_.control_U.m = params_.mass;
    control_obj_.control_U.g = params_.gravity;

    control_obj_.step();

    Output output;
    output.tau_x = control_obj_.control_Y.tau_x;
    output.tau_y = control_obj_.control_Y.tau_y;
    output.tau_z = control_obj_.control_Y.tau_z;
    output.thrust = control_obj_.control_Y.thrust;
    for (int i = 0; i < 3; i++) {
      output.F_des[i] = control_obj_.control_Y.F_des[i];
    }

    copter::msg::ForceTorque msg;
    msg.tau_x = output.tau_x;
    msg.tau_y = output.tau_y;
    msg.tau_z = output.tau_z;
    msg.thrust = output.thrust;
    msg.force_desired.x = output.F_des[0];
    msg.force_desired.y = output.F_des[1];
    msg.force_desired.z = output.F_des[2];

    force_pub_->publish(msg);

    overrun_flag = false;
  }

  rclcpp::Publisher<copter::msg::ForceTorque>::SharedPtr force_pub_;
  rclcpp::Subscription<copter::msg::State>::SharedPtr state_sub_;
  rclcpp::Subscription<copter::msg::Position>::SharedPtr position_sub_;
  rclcpp::TimerBase::SharedPtr timer_;

  controlModelClass control_obj_;
  Params params_;
  State quadrotor_state_;
  double d_pos_[3] = {0.0};
  double d_vel_[3] = {0.0};
  double d_acc_[3] = {0.0};
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ControlNode>());
  rclcpp::shutdown();
  return 0;
}