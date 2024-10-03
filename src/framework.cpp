#include "framework.hpp"

#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>


Framework::Framework() {

    init_window();
    init_objects();
    init_controller();

    x_ref = PARAMS("INITIAL_Q1");

    running = false;
    paused = false;
    control = false;
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
    TUNING.read_data();

    double init[6] = {
        TUNING("K1"),
        TUNING("K2"),
        TUNING("K3"),
        TUNING("K4"),
        TUNING("K5"),
        TUNING("K6")
    };

    Matrix2D<double, 1, 6> K(init);
    controller = LQRController<6, 1>(K);
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
    x_ref = PARAMS("INITIAL_Q1");
    init_controller();
}


void Framework::events(void) {
    if (WindowShouldClose()) { running = false; }
    if (IsKeyPressed(KEY_R)) { reset(); }
    if (IsKeyPressed(KEY_P)) { paused = !paused; }
    if (IsKeyPressed(KEY_SPACE)) { control = !control; }

    double frame_time = GetFrameTime();
    if (IsKeyDown(KEY_A)) { x_ref -= frame_time; }
    if (IsKeyDown(KEY_D)) { x_ref += frame_time; }
}


void Framework::update_loop(void) {
    while (running) {
        double time_passed = simulation_timer.tick();
        if (time_passed > 0.0F && !paused) {

            Matrix2D<double, 6, 1> reference({x_ref, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F});

            double F = controller.control(pendulum.get_state(), reference)(0);
            F = std::clamp<double>(F, -10, 10);

            if (control) { pendulum.force(F); }
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

    //snprintf(buffer, sizeof(buffer), "Control : %.3f [m]", x_ref);
    //DrawTextEx(font, buffer, {10, 70}, 32, 2, COLORS("TEXT"));

    if (paused) {
        DrawTextEx(font, "paused", {10, 70}, 32, 2, COLORS("CIRCLE"));
    }

    pendulum.render();

    if (control) {
        float x = ((float)x_ref * LAYOUT("SCALE")) + LAYOUT("WINDOW_WIDTH") / 2;
        float y = LAYOUT("WINDOW_HEIGHT") * 0.6;
        DrawTriangleLines({x, y}, {x + 10.0F, y + 20.0F}, {x - 10.0F, y + 20.0F}, COLORS("REFERENCE"));
    }
    
    EndDrawing();
}


void Framework::render_loop(void) {
    while (running == true) {
        events();
        render();
    }
}