// subsystems/chassis.hpp

#pragma once
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "control/odometry.hpp"
#include "control/pid_controller.hh"
#include "pros/motor_group.hpp"

namespace Robot{
class chassis {
private:


    pros::MotorGroup left_motors;
    pros::MotorGroup right_motors;


    double left_speed;
    double right_speed;

    // PID controller
    pid_controller _turn_controler;
    pid_controller _movement_controler;


public:
    // Constructor
    chassis();

    // Methods
    void drive(double dir, double turn);
    void stop();
    void turn_to_angle(double curerentAngle,double targetAngle);
    void move_to_distance(double currentDistance, double targetDistance);
};
}