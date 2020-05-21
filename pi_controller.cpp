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
}

void pi_controller::mainLoop()
{
    mainTimer->start();

    //mainScreen->rndTestData( -1, 1 );
    //mainScreen->sinTestData( -1, 1, 0.05 );
    mainScreen->setYAxis( 0, 1.2 );
    SPInterface->setupSPI();

    int dataBufferLength = mainScreen->getDataBufferLength();
//    float *dataBuffer = mainScreen->getDataBuffer();

    float *myBuffer = new float[dataBufferLength];

    float temp;
    while( mainInput->isRunning() )
    {
        //temp = std::invoke( &pi_spi_adc::readValueFunction, SPInterface );
        std::cout << SPInterface->mcp3201_readvalues( myBuffer, NULL, dataBufferLength ) << std::endl;//pi_spi_adc::(SPInterface->*readValueFctn)();

        //std::cout << temp << std::endl;
        counter++;
    }

    mainTimer->stop();
    mainTimer->setCounter( counter );
    mainTimer->printReport();
}
