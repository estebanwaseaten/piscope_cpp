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

    std::chrono::seconds timespan(1);
    std::this_thread::sleep_for(timespan);

    mainController->mainLoop();

    delete mainController;

    tcsetattr( 0, TCSANOW, &old_tio);
    return 0;
}
