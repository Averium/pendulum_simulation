#include "framework.hpp"

#include <string>
#include <iomanip>
#include <sstream>


Framework::Framework() {

    init_window();
    init_objects();
    init_controller();

    running = false;
    paused = false;
}


void Framework::init_window(void) {
    InitWindow(LAYOUT("WINDOW_WIDTH"), LAYOUT("WINDOW_HEIGHT"), "Double inverted pendulum simulation");
    SetTargetFPS(TIMING("RENDERING_FPS"));
}


void Framework::init_objects(void) {
    simulation_timer = Timer(TIMING("SIMULATION_REFRESH_PERIOD_MS") * MS_TO_S);
    font = LoadFont(PATH::FONT_FILE.c_str());
    pendulum = Pendulum(PARAMS);
}


void Framework::init_controller(void) {

    const unsigned int state_dim = 6U;
    const unsigned int input_dim = 1U; 

    ConfigFile<float> T(PATH::TUNING_FILE);

    Matrix2D<double, state_dim, state_dim> A;
    Matrix2D<double, state_dim, input_dim> B;

    // fill A and B matrices here //

    double q_diag[state_dim] = {T("Q1"), T("Q2"), T("Q3"), T("Q4"), T("Q5"), T("Q6")};
    auto Q = Matrix2D<double, state_dim, state_dim>::diagonal(q_diag);
    Matrix2D<double, input_dim, input_dim> R({T("R1")});

    controller = LQRController<state_dim, input_dim>(A, B, Q, R);
}


void Framework::start(void) {
    if (!running) {
        running = true;
        simulation = std::thread(&Framework::update_loop, this);
        render_loop();
    }
}


void Framework::stop(void) {
    running = false;
    if (simulation.joinable()) { simulation.join(); }
}


void Framework::reset(void) {
    pendulum.reset(PARAMS);
}


void Framework::events(void) {
    if (WindowShouldClose()) { running = false; }
    if (IsKeyPressed(KEY_R)) { reset(); }
    if (IsKeyPressed(KEY_P)) { paused = !paused; }
}


void Framework::update_loop(void) {
    while (running) {
        double time_passed = simulation_timer.tick();
        if (time_passed > 0.0F) {
            Matrix2D<double, 1, 1> reference({0.0F});
            double F = controller.control(pendulum.get_state())(0);
            pendulum.force(F);
            pendulum.update(time_passed);
            sim_time = time_passed;
        }
    }
}


void Framework::render(void) {
    BeginDrawing();

    ClearBackground(COLORS("BACKGROUND"));

    char buffer[30];
    
    snprintf(buffer, sizeof(buffer), "Rendering  : %.3f [ms]", GetFrameTime() * S_TO_MS);
    DrawTextEx(font, buffer, {10, 10}, 32, 2, COLORS("TEXT"));

    snprintf(buffer, sizeof(buffer), "Simulation : %.3f [ms]", sim_time * S_TO_MS);
    DrawTextEx(font, buffer, {10, 40}, 32, 2, COLORS("TEXT"));

    snprintf(buffer, sizeof(buffer), "Control    : -    [ms]", sim_time * S_TO_MS);
    DrawTextEx(font, buffer, {10, 70}, 32, 2, COLORS("TEXT"));

    pendulum.render();
    
    EndDrawing();
}


void Framework::render_loop(void) {
    while (running == true) {
        events();
        render();
    }
}