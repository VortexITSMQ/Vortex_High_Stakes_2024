#include "subsystems/Chassis.hpp"
#include <math.h>
#include "pros/llemu.hpp"
#include "control/odometry.hpp"
#include "math.h"
// Constructor
namespace Robot{
    chassis::chassis()
    : left_motors({-18,-19}),right_motors({12,13}), _movement_controler(0,0,0), _turn_controler(0,0,0) {}

// Método para avanzar y retroceder
void chassis::drive(double dir, double turn) {
    left_speed = dir + turn;
    right_speed = dir - turn;
    left_motors.move_velocity(left_speed);
    right_motors.move_velocity(right_speed);
}

// Método para detener el chasis
void chassis::stop() {
left_motors.move_velocity(0);
left_motors.move_velocity(0);
}

void chassis::turn_to_angle(double currentAngle,double targetAngle) {
        // Since our turn P coefficient is negative, we must invert the angle
        
        targetAngle = std::abs((targetAngle * M_PI / 180) - M_PI);
        double heading = currentAngle - M_PI;

        double shortest_distance
            = std::fmod(targetAngle - heading + M_PI,
                                M_TWOPI)
            - M_PI;

        double rotational_output
            = std::clamp(_turn_controler.get_output(
                             0.0,
                             std::abs(shortest_distance)),
                         -1.0,
                         1.0);

        drive(0.0,
              static_cast<std::int32_t>(
                  rotational_output * 127.0
                  * (shortest_distance < 0 ? -1.0 : 1.0)));
}
// Método de PID para conducir hasta un punto
void chassis::move_to_distance(double currentDistance,double targetDistance) {
    double current_distance = currentDistance;

        double output = std::clamp(
        _movement_controler.get_output(current_distance,
                                           targetDistance),
        -1.0,
        1.0);

    drive(output * 127.0, 0.0);
}
}