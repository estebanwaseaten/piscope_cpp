#include "piscope.h"    //includes some very standard libs and globals

#include <iostream>
#include <chrono>       //libs needed for this class
#include <thread>       //libs needed for this class

#include "pi_input.h"


void pi_input::inputLoop()
{
    char cmd;
    std::chrono::milliseconds timespan(100);

    while(running)
    {
        std::this_thread::sleep_for(timespan);  //sleep for 100ms
        std::cin >> cmd;
        switch( cmd )
        {
            case 'q':
                running = false;

                break;
            default:
                break;
        }
        std::cout << "\33[2K\r";
    }

}

bool pi_input::isRunning()
{
    return this->running;
}

pi_input::pi_input()
{
    this->running = true;
    std::cout << "pi_input::pi_input()" << std::endl;
}

pi_input::~pi_input()
{
    std::cout << "pi_input::~pi_input()" << std::endl;
}
