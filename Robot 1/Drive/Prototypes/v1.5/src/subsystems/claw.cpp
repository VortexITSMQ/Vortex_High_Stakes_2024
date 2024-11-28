#include "subsystems/claw.hpp"
namespace Robot {
    claw::claw()
    : piston('A') {}

    void claw::toggle_state(bool button) {
        // Solo cambia el estado cuando el botón se presiona y luego se suelta
        if (button && !last_button_state) {
            piston_state = !piston_state;
            piston.set_value(piston_state);
        }

        // Actualiza el estado anterior del botón
        last_button_state = button;
    }

}