#pragma once


class Timer {

public:
    Timer(void);
    Timer(double period);

    double tick(void);

private:
    double period;
    double statemark;

};