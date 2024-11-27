#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include <cmath>

namespace Robot{
class Odometry {
private:
    double prevEncdL;
    double prevEncdR;
    double delta_angle;
    double x;
    double y;
    double total_distance;
    double angle_rad;
    double wheel_circ;
    double angle_deg;

    pros::adi::Encoder left_encoder; // Asegúrate de que estos estén declarados externamente
    pros::adi::Encoder right_encoder;
    pros::IMU imu;


public:
    // Constructor para inicializar la posición y los valores de los encoders
    Odometry();

    // Método para inicializar los valores de odometría
    void reset(double initialX = 0, double initialY = 0, double initialAngle = 0);

    // Método para actualizar la posición de acuerdo con los encoders y el ángulo
    void update();

    // Métodos para obtener las coordenadas y el ángulo
    double getX() const;
    double getY() const;
    double getAngle() const;
    double getRadAngle() const;

    double getAbsDistance() const;
};
}
#endif // ODOMETRY_HPP
