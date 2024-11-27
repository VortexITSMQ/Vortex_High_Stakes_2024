#ifndef MINIPID_H
#define MINIPID_H

// MiniPID, courtesy of https://github.com/tekdemo/MiniPID/tree/master

class pid_controller {
public:
    pid_controller(double, double, double);

    pid_controller(double, double, double, double);

    void
    set_p(double);

    void
    set_i(double);

    void
    set_d(double);

    void
    set_f(double);

    void
    set_pid(double, double, double);

    void
    set_pid(double, double, double, double);

    void
    set_max_i_output(double);

    void
    set_output_limits(double);

    void
    set_output_limits(double, double);

    void
    set_direction(bool);

    void
    set_setpoint(double);

    void
    reset();

    void
    set_output_ramp_rate(double);

    void
    set_setpoint_range(double);

    void
    set_output_filter(double);

    double
    get_output();

    double
    get_output(double);

    double
    get_output(double, double);

private:
    double
    clamp(double, double, double);

    bool
    bounded(double, double, double);

    void
    check_signs();

    void
    init();

    double P;
    double I;
    double D;
    double F;

    double maxIOutput;
    double maxError;
    double errorSum;

    double maxOutput;
    double minOutput;

    double setpoint;

    double lastActual;

    bool firstRun;
    bool reversed;

    double outputRampRate;
    double lastOutput;

    double outputFilter;

    double setpointRange;
};
#endif
