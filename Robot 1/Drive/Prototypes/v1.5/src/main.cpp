#include "main.h"
#include "subsystems/chassis.hpp"
#include "subsystems/claw.hpp"
#include "subsystems/arm.hpp"

#include "control/odometry.hpp"
Robot::chassis _chassis;
Robot::claw _claw;
Robot::arm _arm;
pros::IMU imu(1);
Robot::Odometry odometry;
void odometry_task(void*) {
    while (true) {
        odometry.update();
        // Puedes acceder a la posición actual de esta forma:
        double currentX = odometry.getX();
        double currentY = odometry.getY();
        double currentAngle = odometry.getAngle();

        // Imprime la posición para verificar:
        pros::lcd::print(0, "X: %f, Y: %f, Angle: %f", currentX, currentY, currentAngle);


        pros::delay(20);  // Espera para no sobrecargar la CPU
    }
}

pros::Task odometryTask(odometry_task,NULL,"odometry");
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Reseting Odometry");
	odometry.reset(); 
    pros::lcd::set_text(1, "Odometry Reseted");
    pros::lcd::set_text(1, "Calibrating IMU...");
    
    imu.reset(); // Resetea el IMU para iniciar la calibración
    while (imu.is_calibrating()) {
        pros::delay(20); // Espera a que el IMU termine de calibrarse
    }
    
    pros::lcd::set_text(1, "IMU Calibration Complete");

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    _chassis.move_to_distance(odometry.getAbsDistance(),10);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	while (true) {
		_chassis.drive(master.get_analog(ANALOG_LEFT_Y),master.get_analog(ANALOG_RIGHT_X));
        _claw.toggle_state(master.get_digital(DIGITAL_A));
		pros::delay(20);
	}
}