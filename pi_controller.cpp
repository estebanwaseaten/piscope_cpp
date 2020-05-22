#include "pi_controller.h"


pi_controller::pi_controller()
{
    mainInput = new pi_input( this );
    mainScreen = new pi_screen();
    mainTimer = new pi_timer();
    SPInterface = new pi_spi_adc();
}

pi_controller::~pi_controller()
{
    delete mainInput;
    delete mainScreen;
    delete mainTimer;
    delete SPInterface;
}

void pi_controller::mainLoop()
{
    //mainScreen->rndTestData( -1, 1 );
    mainScreen->sinTestData( -1, 1, 0.05, 0 );
    mainScreen->start();
    mainScreen->setYAxis( 0, 1.2 );

    int dataBufferLength = mainScreen->getDataBufferLength();
//    float *dataBuffer = mainScreen->getDataBuffer();

    float *myBuffer = new float[dataBufferLength];


    counter = 0;
    mainTimer->start();
    while( mainInput->isRunning() )//mainInput->isRunning() )
    {
        //std::cout << SPInterface->mcp3201_readvalues( myBuffer, NULL, dataBufferLength ) << std::endl;//pi_spi_adc::(SPInterface->*readValueFctn)();
        //SPInterface->mcp3201_readvalue();
        mainScreen->sinTestData( -1, 1, 0.05, counter );
        counter++;
    }
    mainTimer->stop();

    mainTimer->setCounter( counter );
    mainTimer->printReport();
}
