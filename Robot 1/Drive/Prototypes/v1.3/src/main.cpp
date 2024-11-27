#include "main.h"
// claw ports
pros::ADIDigitalOut piston ('A');

// Chassis ports
pros::MotorGroup left_mg({-19, -20});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup right_mg({12, 11}); 

// Odometry constans and ports
pros::ADIEncoder left_encoder('E', 'B', true);
pros::ADIEncoder right_encoder('C', 'D', false);
//pros::ADIEncoder perpendicular_encoder('E', 'F', false);
pros::Imu imu(8);
double baseWidth = 10;

// In-take ports
pros::Motor intake_motor(-18);

// Ramp ports

pros::Motor ramp_motor(13);

pros::Motor arm_motor(9);



// Definición de constantes
const double WHEEL_DIAMETER = 4.0; // Diámetro de la rueda en pulgadas
const int TICKS_PER_REVOLUTION = 360; // Ticks por revolución del encoder
const double WHEEL_CIRCUMFERENCE = M_PI * WHEEL_DIAMETER; // Circunferencia de la rueda en pulgadas

// Función para convertir ticks a pulgadas
double ticksToInches(double ticks) {
    return (ticks / TICKS_PER_REVOLUTION) * WHEEL_CIRCUMFERENCE;
}

double InchesToTicks(double inches){
    return (inches/WHEEL_CIRCUMFERENCE)*TICKS_PER_REVOLUTION;
}

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

// Función para convertir radianes a grados
double radiansToDegrees(double radians) {
    return radians * (180.0 / M_PI);
}

class Position {
public:
    // Atributos para almacenar las coordenadas y el ángulo
    double x;
    double y;
    double angle;

    // Constructor
    Position(double xPos, double yPos, double ang) : x(xPos), y(yPos), angle(ang) {

    }

    // Método para mostrar la posición
    void display() {
        double x_inches = ticksToInches(x); // Convertir de ticks a pulgadas
        double y_inches = ticksToInches(y); // Convertir de ticks a pulgadas
        std::string a = std::to_string(x_inches);
        std::string b = std::to_string(y_inches);

        
        pros::lcd::set_text(1, a);
        pros::lcd::set_text(2, b);
        pros::lcd::set_text(3, std::to_string(angle));
    }
};
Position position(0.0,0.0,0.0);

void odometry(void*){
double prevEncdL = 0;
double prevEncdR = 0;
double prevEncdLat = 0;
double prevAngle = 0;

while (true){
    double left_enc = left_encoder.get_value();
    double right_enc = right_encoder.get_value();

    double delta_left = left_enc - prevEncdL;
    double delta_right = right_enc - prevEncdR;
    double sumEncdChange = (delta_left + delta_right);

    position.angle = imu.get_heading();

    position.x += sumEncdChange * cos(position.angle);
    position.y += sumEncdChange * sin(position.angle);

    prevEncdL = left_enc;
    prevEncdR = right_enc;

    pros::delay(20);
}

}

void move(double targetX_inches, double targetY_inches, double targetAngle, Position &pos) {
    
    //Inches to ticks
    double targetX = InchesToTicks(targetX_inches);
    double targetY = InchesToTicks(targetY_inches);
    double angleInRadians = degreesToRadians(targetAngle);
    
    // Constantes de ganancia para el control proporcional
    const double KpX = 0.1; // Ganancia para la coordenada X
    const double KpY = 0.1; // Ganancia para la coordenada Y
    const double KpAngle = 0.1; // Ganancia para el ángulo

    while (true) {


        // Calcular errores
        double errorX = targetX - pos.x;
        double errorY = targetY - pos.y;
        double errorAngle = fmod(targetAngle - pos.angle + M_PI, 2*M_PI) - M_PI;// Normaliza el error del ángulo


        // Verificar si estamos cerca de la posición objetivo
        if (fabs(errorX) < 1.0 && fabs(errorY) < 1.0 && fabs(errorAngle) < 1.0) {
            // Detener los motores si estamos en la posición objetivo
            left_mg.move_velocity(0);
            right_mg.move_velocity(0);
            break; // Salir del bucle
        }

        // Calcular las velocidades de los motores
        double speedX = KpX * errorX;
        double speedY = KpY * errorY;
        double turnSpeed = KpAngle * errorAngle;

        // Combinar las velocidades para el control diferencial
        double leftSpeed = speedX + turnSpeed;
        double rightSpeed = speedY - turnSpeed;

        // Aplicar límites a las velocidades (puedes ajustar esto según sea necesario)
        leftSpeed = std::clamp(leftSpeed, -100.0, 100.0);
        rightSpeed = std::clamp(rightSpeed, -100.0, 100.0);

        // Mover los motores
        left_mg.move_velocity(leftSpeed);
        right_mg.move_velocity(rightSpeed);

        // Actualizar la posición (esto debería llamarse desde la función de odometría)
        // odometry(pos); // Asegúrate de tener un mecanismo que actualice la posición

        pros::delay(20); // Retardo para evitar un bucle muy rápido
    }
}


pros::Task odometry_task(odometry,NULL,"odometry");
void initialize() {
    Position position(0.0,0.0,0.0);
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
    piston.set_value(false);
    // Set gearing
    ramp_motor.set_gearing(pros::E_MOTOR_GEAR_GREEN);
	intake_motor.set_gearing(pros::E_MOTOR_GEAR_GREEN);
    left_mg.set_gearing(pros::E_MOTOR_GEAR_GREEN);
    right_mg.set_gearing(pros::E_MOTOR_GEAR_GREEN);
}


void disabled() {}




void competition_initialize() {}


void autonomous() {
    
    move(10.0,10.0,0,position);
}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
    Position pos(0.0,0.0,0.0);

	bool motorOn = false;  // Variable para almacenar el estado del motor
    bool lastButtonState = false; // Estado previo del botón
    bool piston_state = false;
    bool lastPistonButtonState = false;
    

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

        lastButtonState = currentButtonState;

        if (currentPistonButtonState && !lastPistonButtonState) {
            piston_state = !piston_state;
        }

        if (piston_state){
            piston.set_value(true);
        } else {
            piston.set_value(false);
        }

        // Guarda el estado del botón para el siguiente ciclo
        lastPistonButtonState = currentPistonButtonState;

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        arm_motor.move(120);  // Mueve el brazo hacia adelante
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
        arm_motor.move(-100);  // Mueve el brazo hacia atrás
        } else {
        arm_motor.move(0);  // Detiene el brazo si no se presiona ningún botón
        }


		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_LEFT_X);  // Gets the turn left/right from right joystick
        double left_speed = dir + turn;
        double right_speed = dir - turn;
		left_mg.move(left_speed);                      // Sets left motor voltage
		right_mg.move(right_speed);                     // Sets right motor voltage
        pros::lcd::print(0, "X: %f, Y: %f, Theta: %f", position.x, position.y, position.angle);
        pros::lcd::print(1, "Left Speed: %f, Right Speed: %f", left_speed, right_speed);
		pros::delay(20);                               // Run for 20 ms then update
	}
}