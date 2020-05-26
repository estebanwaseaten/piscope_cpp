#ifndef pi_input_h
#define pi_input_h

#include <iostream>
#include <chrono>
#include <thread>

#include "pi_controller.h"
#include "pi_screen.h"


class pi_controller;

class pi_input
{
public:
    void inputLoop();
    bool isRunning();
    pi_input( pi_controller *ctrlr );
    ~pi_input();

private:
    std::thread     *inputThread;   //;
    pi_controller   *myController;  //so that we can send messages back to the main controller...
};

#endif
