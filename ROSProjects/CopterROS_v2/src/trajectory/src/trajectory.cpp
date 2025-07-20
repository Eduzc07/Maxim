#include "rclcpp/rclcpp.hpp"
#include "copter/msg/position.hpp"
#include <iostream>
#include "symbolicc++.h"
#include <Eigen/Dense>

using namespace Eigen;

void solve_alpha(MatrixXf&, MatrixXf, VectorXf);

void solve_alpha(MatrixXf &sol, MatrixXf waypoints, VectorXf d0) {
  int n = ((int)waypoints.rows() - 1);
  RCLCPP_INFO(rclcpp::get_logger("solve_alpha"), "Points: %d, Ti: %0.4fs", n, d0(n));
  sol.setZero(n * 8, 3);

  MatrixXf B(n * 8, 3);
  B.setZero(n * 8, 3);
  B.row(0) = waypoints.row(0);
  int m = 1;
  for (int i = 1; i < n; i++) {
    B.row(m) = waypoints.row(i);
    B.row(m + 1) = waypoints.row(i);
    m = m + 2;
  }
  B.row(2 * n - 1) = waypoints.row(n);

  Symbolic a("a", 8);
  Symbolic t("t");
  Symbolic Ti("Ti");
  Symbolic ft("ft", 7);

  ft(0) = a(0) +
          a(1)*(t/Ti) +
          a(2)*pow(t/Ti, Symbolic(2)) +
          a(3)*pow(t/Ti, Symbolic(3)) +
          a(4)*pow(t/Ti, Symbolic(4)) +
          a(5)*pow(t/Ti, Symbolic(5)) +
          a(6)*pow(t/Ti, Symbolic(6)) +
          a(7)*pow(t/Ti, Symbolic(7));

  for (int i = 1; i < 7; i++) {
    ft(i) = df(ft(i - 1), t);
  }

  MatrixXf A(n * 8, n * 8);
  A.setZero(n * 8, n * 8);
  VectorXf p(n * 8);

  int ki = n, df = 0, di = 1, vdi = n, q = 0;

  for (int j = 0; j < n * 8; j++) {
    p.setZero(n * 8);
    double si = 0, Tsi = 0;
    int init = 0;

    if (j < 2 * n) {
      if (j % 2 == 0) { ki--; si = 0; }
      else si = d0(n - ki);

      init = 8 * ki;
      Tsi = d0(n - ki);

      for (int i = 0; i < 8; i++) {
        p(init + i) = (double)ft(0).coeff(a(i))[t == si, Ti == Tsi];
      }
    } else if (j < 2 * n + 6) {
      if (j % 2 == 0) {
        ki--; si = 0; Tsi = d0(1); df++; init = 8 * (n - 1);
      } else {
        si = d0(n); Tsi = d0(n); init = 0;
      }
      for (int i = 0; i < 8; i++) {
        p(init + i) = (double)ft(df).coeff(a(i))[t == si, Ti == Tsi];
      }
    } else {
      q++; vdi--; si = d0(q); Tsi = d0(q);
      for (int i = 0; i < 8; i++) {
        p(8 * vdi + i) = (double)ft(di).coeff(a(i))[t == si, Ti == Tsi];
        p(8 * (vdi - 1) + i) -= (double)ft(di).coeff(a(i))[t == 0, Ti == Tsi];
      }
      if (q == n - 1) {
        di++; q = 0; vdi = n;
      }
    }

    A.row(j) = p.transpose();
  }

  for (int i = 0; i < 3; i++) {
    VectorXf b = B.col(i);
    VectorXf sol_i = A.colPivHouseholderQr().solve(b);
    sol.col(i) = sol_i;
  }
}


int main(int argc, char **argv) 
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("points");
  auto position_pub = node->create_publisher<copter::msg::Position>("/Position", 1);
  rclcpp::Rate loop_rate(50);

  copter::msg::Position pos;

  //----------------------------------------------------------------
  //Points
  //----------------------------------------------------------------
  MatrixXf waypoints(7, 3);
  waypoints << 0, 0, 0,
               0, 0, 2,
               2, 2, 2,
               2, -2, 2,
              -2, 2, 2,
              -2, -2, 2,
               0, 0, 2;

  int way_rows = waypoints.rows() - 1;
  MatrixXf d = waypoints.block(1, 0, way_rows, 3) - waypoints.block(0, 0, way_rows, 3);
  VectorXf traj_time = VectorXf::Zero(way_rows + 1);
  VectorXf d0 = VectorXf::Zero(way_rows + 1);
  double cusum = 0;
  for (int i = 0; i < way_rows; i++) {
    cusum = 6 * sqrt(pow(d(i, 0), 2) + pow(d(i, 1), 2) + pow(d(i, 2), 2));
    traj_time(i + 1) = traj_time(i) + cusum;
    d0(i + 1) = cusum;
  }

  MatrixXf sol(way_rows * 8, 3);
  solve_alpha(sol, waypoints, d0);

  VectorXf fp(8), dfp(8), ddfp(8);
  double begin = rclcpp::Clock().now().seconds();
  int t_index = 0;

  RCLCPP_INFO(node->get_logger(), "Ready to start sending . . .");

  while (rclcpp::ok()) {
    rclcpp::spin_some(node);

    double now = rclcpp::Clock().now().seconds();
    double time = now - begin;

    if (time > traj_time(way_rows))
      time = traj_time(way_rows);

    if (time > traj_time(t_index + 1))
      t_index++;

    time = time - traj_time(t_index);

    // RCLCPP_INFO(node->get_logger(), "time:%0.2fs (%0.2fs), t_index:%d, Ti:%0.03fs", now - begin, time, t_index, d0(t_index + 1));

    if (time == 0) {
      pos.pos.x = pos.pos.y = pos.pos.z = 0;
    } else {
      double Ti = d0(t_index + 1);
      double scale = time / Ti;

      for (int i = 0; i < 3; i++) {
        fp << 1, scale, pow(scale, 2), pow(scale, 3), pow(scale, 4), pow(scale, 5), pow(scale, 6), pow(scale, 7);
        dfp << 0, 1 / Ti, 2 * scale / Ti, 3 * pow(scale, 2) / Ti, 4 * pow(scale, 3) / Ti,
                5 * pow(scale, 4) / Ti, 6 * pow(scale, 5) / Ti, 7 * pow(scale, 6) / Ti;
        ddfp << 0, 0, 2 / Ti, 6 * scale / Ti, 12 * pow(scale, 2) / Ti, 20 * pow(scale, 3) / Ti,
                 30 * pow(scale, 4) / Ti, 42 * pow(scale, 5) / Ti;

        auto segment = sol.col(i).segment(8 * (way_rows - 1) - 8 * t_index, 8);
        double pt = fp.dot(segment);
        double dp = dfp.dot(segment);
        double ap = ddfp.dot(segment);

        if (i == 0) {
          pos.pos.x = pt; pos.vel.x = dp; pos.acc.x = ap;
        } else if (i == 1) {
          pos.pos.y = pt; pos.vel.y = dp; pos.acc.y = ap;
        } else {
          pos.pos.z = pt; pos.vel.z = dp; pos.acc.z = ap;
        }
      }
    }

    position_pub->publish(pos);
    loop_rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}