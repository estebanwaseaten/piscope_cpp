#ifndef pi_controller_h
#define pi_controller_h

#include "pi_input.h"
#include "pi_screen.h"
#include "pi_timer.h"

class pi_input;
class pi_screen;
class pi_timer;

class pi_controller
{
public:
    pi_controller();
    ~pi_controller();

    void mainLoop();

private:
    pi_input    *mainInput;
    pi_screen   *mainScreen;

    pi_timer    *mainTimer;
    long        counter{0};
};

#endif
