#include "pendulum.hpp"


Pendulum::Pendulum() {}


Pendulum::Pendulum(ConfigFile<float>& PARAMS) {
    reset(PARAMS);
    collided = false;
}

void Pendulum::reset(ConfigFile<float>& PARAMS) {

    coordinates = Matrix2D<double, 3, 1>(
        {PARAMS("INITIAL_Q1"), PARAMS("INITIAL_Q2"), PARAMS("INITIAL_Q3")}
    );
    velocities = Matrix2D<double, 3, 1>(
        {PARAMS("INITIAL_V1"), PARAMS("INITIAL_V2"), PARAMS("INITIAL_V3")}
    );

    m1 = PARAMS("CART_MASS");
    m2 = PARAMS("LOWER_MASS");
    m3 = PARAMS("HIGHER_MASS");
    l1 = PARAMS("RAIL_LENGTH");
    l2 = PARAMS("LOWER_ROD_LENGTH");
    l3 = PARAMS("HIGHER_ROD_LENGTH");
    k1 = PARAMS("CART_DAMPING_RATIO");
    k2 = PARAMS("LOWER_JOINT_DAMPING_RATIO");
    k3 = PARAMS("HIGHER_JOINT_DAMPING_RATIO");
    g  = PARAMS("GRAVITY");

    m2_p_m3 = m2 + m3;
    m3_l3 = m3 * l3;
    m3_l2_l3 = m3_l3 * l2;
    m2_p_m3_l2 = m2_p_m3 * l2;

    a = m1 + m2_p_m3;
    d = m2_p_m3_l2 * l2;
    f = m3_l3 * l3;
}

Matrix2D<double, 6, 1> Pendulum::get_state(void) const {
    return Matrix2D<double, 6, 1>({
        coordinates(0),
        velocities(0),
        coordinates(1),
        velocities(1),
        coordinates(2),
        velocities(2)
    });
}

void Pendulum::force(const double input) {
    this->input = input;
}

void Pendulum::update(const double dt) {

    double q1 = coordinates(0);
    double q2 = coordinates(1);
    double q3 = coordinates(2);
    double dq1 = velocities(0);
    double dq2 = velocities(1);
    double dq3 = velocities(2);

    double dq2_dq2 = dq2 * dq2;
    double dq3_dq3 = dq3 * dq3;

    double m2_p_m3_l2_sin_q2 = m2_p_m3_l2 * sin(q2);
    double m3_l3_sin_q3 = m3_l3 * sin(q3);
    double l2_m3_l3_sin_q2_q3 = m3_l2_l3 * sin(q2 - q3);

    double b = -m2_p_m3_l2 * cos(q2);
    double c = -m3_l3 * cos(q3);
    double e = +m3_l2_l3 * cos(q2 - q3);

    double A =
        + input
        - m2_p_m3_l2_sin_q2 * dq2_dq2
        - m3_l3_sin_q3 * dq3_dq3
        - k1 * dq1;

    double B =
        - l2_m3_l3_sin_q2_q3 * dq3_dq3
        + g * m2_p_m3_l2_sin_q2
        - k2 * dq2;

    double C =
        + l2_m3_l3_sin_q2_q3 * dq2_dq2
        + g * m3_l3_sin_q3
        - k3 * dq3;

    Matrix2D<double, 3, 3> M({a, b, c, b, d, e, c, e, f});
    Matrix2D<double, 3, 1> G({A, B, C});
    Matrix2D<double, 3, 3> iM = M.inverse();

    if (collided) { G(0) = -(dq1 / dt + B * iM(0, 1) + C * iM(0, 2)) / iM(0, 0); }

    accelerations = M.inverse() * G;
    velocities += accelerations * dt;
    coordinates += velocities * dt;

    if (coordinates(0) < -l1 / 2.0F) {
        coordinates(0) = -l1 / 2.0F;
        collided = true;
    }
    else if (coordinates(0) > l1 / 2.0F) {
        coordinates(0) = l1 / 2.0F;
        collided = true;
    }
    else {
        collided = false;
    }
}

void Pendulum::render(void) const {
    
    const float scale = LAYOUT("SCALE");
    const float rect_x = LAYOUT("S_RECT_X") / 100.0F * LAYOUT("SCALE");
    const float rect_y = LAYOUT("S_RECT_Y") / 100.0F * LAYOUT("SCALE");
    const float circle = LAYOUT("CIRCLE") / 100.0F * LAYOUT("SCALE");
    const float wall = (l1 + LAYOUT("S_RECT_X") / 100.0F) / 2.0F * LAYOUT("SCALE");
    const float line = LAYOUT("LINE") / 100.0F * LAYOUT("SCALE");

    double q1 = coordinates(0), q2 = coordinates(1), q3 = coordinates(2);
    double dq1 = velocities(0), dq2 = velocities(1), dq3 = velocities(2);

    const float x1 = (q1 * scale) + LAYOUT("WINDOW_WIDTH") / 2;
    const float y1 = LAYOUT("WINDOW_HEIGHT") / 2;

    const float x2 = x1 - l2 * scale * sin(q2);
    const float y2 = y1 - l2 * scale * cos(q2);
    
    const float x3 = x2 - l3 * scale * sin(q3);
    const float y3 = y2 - l3 * scale * cos(q3);

    const unsigned int wall_width = rect_x * 0.2F;
    const unsigned int wall_height = rect_y * 2.0F;

    const float left_wall_x = LAYOUT("WINDOW_WIDTH") / 2.0F - wall - wall_width;
    const float right_wall_x = LAYOUT("WINDOW_WIDTH") / 2.0F + wall;
    const float wall_y = (LAYOUT("WINDOW_HEIGHT") - wall_height) / 2.0F;

    DrawRectangle(left_wall_x, wall_y, wall_width, wall_height, COLORS("WALL"));
    DrawRectangle(right_wall_x, wall_y, wall_width, wall_height, COLORS("WALL"));

    DrawLineEx({x1, y1}, {x2, y2}, line, COLORS("ROD"));
    DrawLineEx({x2, y2}, {x3, y3}, line, COLORS("ROD"));

    DrawRectangle(x1 - rect_x / 2, y1 - rect_y / 2, rect_x, rect_y, COLORS("MASS"));
    DrawCircle(x2, y2, circle, COLORS("MASS"));
    DrawCircle(x3, y3, circle, COLORS("MASS"));
}
