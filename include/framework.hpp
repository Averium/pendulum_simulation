#pragma once

#include <thread>
#include <mutex>

#include "raylib.hpp"
#include "const.hpp"

#include "timer.hpp"
#include "pendulum.hpp"


class Framework {

public:
    Framework();

    void init_window(void);
    void init_objects(void);

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

    double sim_time;

    Font font;

    Timer simulation_timer;
    Pendulum pendulum;
    
    std::thread simulation;
};