#pragma once

#include "raylib.hpp"
#include "matrix.hpp"
#include <cmath>

#include <vector>


class Pendulum {

public:
    Pendulum();
    Pendulum(ConfigFile<float>& PARAMS);

    void control(const double force);
    void reset(ConfigFile<float>& PARAMS);

    void update(const double dt);
    void render(void) const;

    Matrix2D<double, 6, 1> get_state(void) const;

private:
    Matrix2D<double, 3, 1> coordinates;
    Matrix2D<double, 3, 1> velocities;
    Matrix2D<double, 3, 1> accelerations;

    double force;
    bool collided;

    double m1;
    double m2;
    double m3;
    double l1;
    double l2;
    double l3;
    double k1;
    double k2;
    double k3;
    double g;

    double m2_p_m3;
    double m3_l3;
    double m3_l2_l3;
    double m2_p_m3_l2;

    double a;
    double d;
    double f;
};