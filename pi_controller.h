#ifndef pi_controller_h
#define pi_controller_h

#include <functional>
#include <time.h>
#include "pi_input.h"
#include "pi_screen.h"
#include "pi_timer.h"
#include "pi_spi_adc.h"

class pi_input;
class pi_screen;
class pi_timer;

class pi_controller
{
public:
    pi_controller();
    ~pi_controller();

    void mainLoop();
    void keyboardInput( char cmd );

private:
    pi_input    *mainInput;
    pi_screen   *mainScreen;
    pi_spi_adc  *SPInterface;

    pi_timer    *mainTimer;
    long        counter{0};
    bool        running = true;
};

#endif
