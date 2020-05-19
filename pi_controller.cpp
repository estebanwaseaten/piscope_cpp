#include "pi_controller.h"


pi_controller::pi_controller()
{
    mainInput = new pi_input( this );
    mainScreen = new pi_screen();
    mainTimer = new pi_timer();

}

pi_controller::~pi_controller()
{
    delete mainInput;
    delete mainScreen;
    delete mainTimer;
}

void pi_controller::mainLoop()
{
    mainTimer->start();

    mainScreen->rndTestData( -1, 1 );
    mainScreen->sinTestData( -1, 1, 0.05 );
    mainScreen->setYAxis( 0, 1.2 );

    while( mainInput->isRunning() )
    {
        counter++;
    }

    mainTimer->stop();
    mainTimer->setCounter( counter );
    mainTimer->printReport();
}
