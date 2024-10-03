#pragma once

#include <thread>
#include <mutex>

#include "raylib.hpp"
#include "const.hpp"

#include "timer.hpp"
#include "pendulum.hpp"
#include "controller.hpp"


class Framework {

public:
    Framework();

    void init_window(void);
    void init_objects(void);
    void init_controller(void);

    void start(void);
    void stop(void);
    void reset(void);

    void events(void);
    void render(void);

    void update_loop(void);
    void render_loop(void);

private:
    bool running;
    bool paused;
    bool control;

    double sim_time;
    double x_ref;

    Font font;

    Timer simulation_timer;
    Pendulum pendulum;
    LQRController<6, 1> controller;

    std::thread simulation;
};