#include "subsystems/arm.hpp"
namespace Robot{
// Inicializa el motor en el puerto deseado, aquí el puerto 1
arm::arm() : arm_motor(1), target_position(arm_motor.get_position()) {}

void arm::move(bool forward_button, bool backward_button) {
    // Si el botón de avance está presionado, incrementa la posición objetivo
    if (forward_button) {
        target_position += 10;
    } 
    // Si el botón de retroceso está presionado, decrementa la posición objetivo
    else if (backward_button) {
        target_position -= 10;
    }

    // Mueve el motor hacia la posición objetivo utilizando control de posición
    arm_motor.move_absolute(target_position, move_speed);
}
}