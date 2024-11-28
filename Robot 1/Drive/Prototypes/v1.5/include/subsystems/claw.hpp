#include "pros/adi.hpp"
namespace Robot {
    class claw {
        private:
        pros::ADIDigitalOut piston;
        bool piston_state;
        bool last_button_state;
        public:
        //constructor
        claw();

        //methods
        void toggle_state(bool button);
        void set_state(bool state);
    };
}