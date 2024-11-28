
#include "pros/motors.hpp"

namespace Robot{
class arm {
public:
    // Constructor
    arm();

    // Método para mover el brazo con los botones dados como parámetros
    void move(bool forward_button, bool backward_button);

private:
    pros::Motor arm_motor; // Motor del brazo
    int target_position;   // Posición objetivo del brazo
    int move_speed = 120;  // Velocidad de movimiento del brazo
};

}