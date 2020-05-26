#include "pi_input.h"


void pi_input::inputLoop()
{
    char cmd;
    std::chrono::milliseconds timespan(100);

    while(true) //I guess deleting the thread will kill this.... somehow
    {
        std::this_thread::sleep_for(timespan);  //sleep for 100ms
        std::cin >> cmd;

        myController->keyboardInput( cmd );
    }

}

pi_input::pi_input( pi_controller *ctrlr )
{
    myController = ctrlr;
    inputThread = new std::thread( &pi_input::inputLoop, this );
    std::cout << "pi_input::pi_input()" << std::endl;
}

pi_input::~pi_input()
{
    inputThread->detach();
    delete inputThread;
    std::cout << "pi_input::~pi_input()" << std::endl;
}
