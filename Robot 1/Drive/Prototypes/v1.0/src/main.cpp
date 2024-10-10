#include "main.h"
pros::ADIAnalogOut piston ('A');

class Position {
public:
    // Atributos para almacenar las coordenadas y el ángulo
    double x;
    double y;
    double angle;

    // Constructor
    Position(double xPos, double yPos, double ang) : x(xPos), y(yPos), angle(ang) {}

    // Método para mostrar la posición
    void display() {
        std::cout << "Posición: (" << x << ", " << y << "), Ángulo: " << angle << " grados" << std::endl;
    }
};


void initialize() {
    pros::ADIAnalogOut piston ('A');
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
    piston.set_value(true);

}


void disabled() {}




void competition_initialize() {}


void autonomous() {}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({-1, -2});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup right_mg({6, 7});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6
	pros::Motor ramp_motor(11);
	pros::Motor intake_motor(20);
    pros::ADIDigitalOut ('A');
	bool motorOn = false;  // Variable para almacenar el estado del motor
    bool lastButtonState = false; // Estado previo del botón
    bool piston_state = false;
    bool lastPistonButtonState = false;
	ramp_motor.set_gearing(pros::E_MOTOR_GEAR_GREEN);
	intake_motor.set_gearing(pros::E_MOTOR_GEAR_GREEN);
    

	while (true) {

		bool currentButtonState = master.get_digital(pros::E_CONTROLLER_DIGITAL_X);
        bool currentPistonButtonState = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
        bool currentPistonButtonStateOff = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);

        // Detecta si el botón ha sido presionado (cambio de estado de no presionado a presionado)
        if (currentButtonState && !lastButtonState) {
            motorOn = !motorOn;  // Cambia el estado del motor (encendido/apagado)
        }

        // Actualiza el estado del motor según la variable motorOn
        if (motorOn) {
            ramp_motor.move(-100);// Enciende el motor
			intake_motor.move(-100);

        } else {
            ramp_motor.move(0);// Apaga el motor
			intake_motor.move(0);
        }

        if (currentPistonButtonState && !lastPistonButtonState) {
            piston_state = !piston_state;
        }

        if (piston_state){
            piston.set_value(true);
        } else {
            piston.set_value(false);
        }

        // Guarda el estado del botón para el siguiente ciclo
        lastButtonState = currentButtonState;
        lastPistonButtonState = currentPistonButtonState;

        /*if (currentPistonButtonStateOn) {
        piston.set_value(true);  // Extiende el pistón
        }
        // Si el botón "B" del controlador está presionado, retrae el pistón
        if (currentPistonButtonStateOff) {
        piston.set_value(false);  // Retrae el pistón
        }*/

		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_LEFT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir + turn);                      // Sets left motor voltage
		right_mg.move(dir - turn);                     // Sets right motor voltage
		pros::delay(20);                               // Run for 20 ms then update
	}
}