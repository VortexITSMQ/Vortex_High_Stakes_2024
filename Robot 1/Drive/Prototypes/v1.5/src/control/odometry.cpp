#include "odometry.hpp"

namespace Robot{
// Constructor para inicializar los valores de la clase
Odometry::Odometry() : prevEncdL(0), prevEncdR(0), delta_angle(0), x(0), y(0), total_distance(0), angle_rad(0), wheel_circ(M_PI * 4), angle_deg(0),
      imu(1), left_encoder('C', 'D', true), right_encoder('G', 'H', false) {}

// Método para reiniciar los valores de odometría
void Odometry::reset(double initialX, double initialY, double initialAngle) {
    x = initialX;
    y = initialY;
    angle_rad = initialAngle;
    prevEncdL = left_encoder.get_value();
    prevEncdR = right_encoder.get_value();
}

// Método para actualizar la posición según los encoders y el IMU
void Odometry::update() {
    angle_deg = imu.get_heading();

    double left_enc = left_encoder.get_value() * wheel_circ / 360;
    double right_enc = right_encoder.get_value() * wheel_circ / 360;

    pros::lcd::print(1, "Left %f, Right %f", left_enc, right_enc);

    double delta_left = left_enc - prevEncdL;
    double delta_right = right_enc - prevEncdR;

    double delta_distance = (delta_left + delta_right) / 2;

    pros::lcd::print(2, "Delta Distance: %f", delta_distance);

    delta_angle = (delta_left - delta_right) / 10;
    angle_rad = imu.get_heading() * M_PI / 180;

    double cos_val = cos(angle_rad);
    double sin_val = sin(angle_rad);
    pros::lcd::print(3, "cos(angle): %f, sin(angle): %f", cos_val, sin_val);

    total_distance += delta_distance;
    pros::lcd::print(4, "total distance: %f", total_distance);

    x += delta_distance * cos(angle_rad);
    y += delta_distance * sin(angle_rad);

    prevEncdL = left_enc;
    prevEncdR = right_enc;
}

// Métodos para obtener las coordenadas y el ángulo
double Odometry::getX() const {
    return x;
}

double Odometry::getY() const {
    return y;
}

double Odometry::getAngle() const {
    return angle_deg;
}

double Odometry::getRadAngle() const {
    return angle_rad;
}

double Odometry::getAbsDistance() const {
    return total_distance;
}
}