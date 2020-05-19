#include <string>
#include <thread>
#include <iostream>
#include <iomanip>
#include <termios.h>	//terminal stuff (linux)
#include <chrono>
#include <ctime>

#include "piscope.h"
#include "pi_spi_adc.h"
#include "pi_screen.h"
#include "pi_input.h"
#include "pi_timer.h"


int main( void )
{
    int i (0);

    struct termios old_tio, new_tio;
    tcgetattr( 0, &old_tio);
	new_tio = old_tio;
	new_tio.c_lflag &= ~(ICANON);
	tcsetattr( 0, TCSANOW, &new_tio);

    std::cout << "\033c";   //still works in cpp
    std::cout << "piscope_cpp 1.0.0" << std::endl;
    std::cout << "hardware threads: " << std::thread::hardware_concurrency() << std::endl;
    auto currentTime = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(currentTime);
    std::cout << "started: " << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << std::endl;

    pi_input    *mainInput = new pi_input();    //this guy should generate the thread
    pi_screen   *mainScreen = new pi_screen();

    std::thread inputThread(&pi_input::inputLoop, mainInput);     //could use async instead of thread??



    pi_timer *myTimer = new pi_timer();
    pi_timer *screenTimer = new pi_timer();
    long counter {0};
    myTimer->start();
    screenTimer->start();

    mainScreen->prepareOverlay();   //only needs to be updated whenever the overlay changes...

    while( mainInput->isRunning() )
    {
        counter++;
        //1. daq

        //2. analysis

        //3. draw screen
        if( screenTimer->currentDuration() > 0.05 )     //this increases speed from 0.03 kHz to 81 kHz
        {
            mainScreen->prepareScreen();
            mainScreen->drawScreen();
            screenTimer->reset();
        }

    }

    myTimer->setCounter( counter );
    myTimer->stop();
    myTimer->printReport();

    inputThread.join();
    delete mainInput;
    delete myTimer;

    tcsetattr( 0, TCSANOW, &old_tio);
    return 0;
}
