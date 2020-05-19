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
#include "pi_controller.h"


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

    pi_controller *mainController = new pi_controller();

    mainController->mainLoop();

    /*pi_input    *mainInput = new pi_input();    //this guy should generate the thread
    pi_screen   *mainScreen = new pi_screen();
    pi_timer *myTimer = new pi_timer();
    long counter {0};
    myTimer->start();

    mainScreen->rndTestData( -1, 1 );
    mainScreen->sinTestData( -1, 1, 0.05 );
    mainScreen->setYAxis( 0, 1.2 );
    //mainScreen->prepareOverlay();   //only needs to be updated whenever the overlay changes...


    while( mainInput->isRunning() )
    {
        counter++;
        //1. daq

        //2. analysis

        /*if( screenTimer->currentDuration() > 0.05 )     //this increases speed from 0.03 kHz to 81 kHz
                                                        //putting screen in their own thread -->16251.5kHz = 16 MHz
                                                        // other threads completely off: 62577.6kHz = 62MHz
    }

    myTimer->setCounter( counter );
    myTimer->stop();
    myTimer->printReport();

    //inputThread.join();
    delete mainInput;
    delete mainScreen;
    delete myTimer;*/

    delete mainController;

    tcsetattr( 0, TCSANOW, &old_tio);
    return 0;
}
