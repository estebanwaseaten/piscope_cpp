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
                std::cout << "\33[2K\r";
                break;
        }

    }

}

bool pi_input::isRunning()
{
    return this->running;
}

pi_input::pi_input( pi_controller *ctrlr )
{
    myController = ctrlr;
    inputThread = new std::thread( &pi_input::inputLoop, this );
    this->running = true;
    std::cout << "pi_input::pi_input()" << std::endl;
}

pi_input::~pi_input()
{
    inputThread->detach();
    delete inputThread;
    std::cout << "pi_input::~pi_input()" << std::endl;
}
