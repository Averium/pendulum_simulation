#include "framework.hpp"

#include <string>
#include <iomanip>
#include <sstream>


Framework::Framework() {

    init_window();
    init_objects();

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

    double ctrl = 0.0F;
    if (IsKeyDown(KEY_A)) { ctrl = ctrl - 1.0F; }
    if (IsKeyDown(KEY_D)) { ctrl = ctrl + 1.0F; }
    pendulum.control(ctrl);
}


void Framework::update_loop(void) {
    while (running) {
        double time_passed = simulation_timer.tick();
        if (time_passed > 0.0F) {
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