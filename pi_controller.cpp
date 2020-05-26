#include "pi_controller.h"


pi_controller::pi_controller()
{
    mainInput   = new pi_input( this );
    mainScreen  = new pi_screen();
    mainTimer   = new pi_timer();
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
    //mainScreen->sinTestData( -1, 1, 0.05, 0 );
    mainScreen->start();
    mainScreen->setModeGraph();
    mainScreen->setYAxis( 0, 1.2 );

    int dataBufferLength = mainScreen->getDataBufferLength();
    float *myDataInput = mainScreen->getDataInputBuffer();

    float *timeBuffer = new float[dataBufferLength];

    counter = 0;
    float daqduration = 0;
    mainTimer->start();
    clock_t start_time = clock();
    while( running )//mainInput->isRunning() )
    {
        daqduration = SPInterface->mcp3201_readValues( myDataInput, timeBuffer, dataBufferLength ); //pi_spi_adc::(SPInterface->*readValueFctn)();
        mainScreen->swapBuffer();
        //std::cout << SPInterface->mcp3201_readvalue() << " - ";
        //mainScreen->sinTestData( -1, 1, 0.05, counter );
        mainScreen->setNote( 0, std::to_string(daqduration) );
        //std::cout << daqduration << std::endl;
        counter++;
    }
    long duration = clock() - start_time;
    mainTimer->stop();

    std::cout << "daq rate 1: "<< (1.0*counter*dataBufferLength)/(1.0*duration/CLOCKS_PER_SEC) <<" (Hz)" << std::endl;
    mainTimer->setCounter( counter );
    mainTimer->printReport();
    std::cout << "last daq amountx: " << dataBufferLength << std::endl;
    std::cout << "last daqduration: " << daqduration << std::endl;
}

void pi_controller::keyboardInput( char cmd )
{
    std::string temp;
    switch( cmd )
    {
        case 'q':
            running = false;
            break;
        case 'm':
            if( mainScreen )
                mainScreen->changeMode();
            break;
        case 'T':

            break;
        case 't':

            break;
        case 'A':
            if( SPInterface )
            {
                SPInterface->decreaseAcquisitionTime();
                temp = "adiv:" + std::to_string(SPInterface->getDivider());//SPInterface->getDivider());
                mainScreen->setNote( 1, temp);
            }
            break;
        case 'D':
            if( SPInterface )
            {
                SPInterface->increaseAcquisitionTime();
                temp = "ddiv:" + std::to_string(SPInterface->getDivider());
                mainScreen->setNote( 1, temp );
            }
            break;
        default:
            std::cout << "\33[2K\r";
            break;
    }
}
