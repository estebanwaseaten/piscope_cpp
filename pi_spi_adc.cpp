#include "pi_spi_adc.h"


pi_spi_adc::pi_spi_adc()
{
    this->setupSPI();
    spi_twoBytesBuffer = new char(sizeof(uint16_t));  //yeah, we allocate 2 bytes
    dataTimer = new pi_timer();
    acquistionTimer = new pi_timer();
}

pi_spi_adc::~pi_spi_adc()
{
    bcm2835_spi_end();
    bcm2835_close();
    delete dataTimer;
    delete acquistionTimer;
    delete spi_twoBytesBuffer;
}

int pi_spi_adc::setupSPI()
{
    if (!bcm2835_init())
    {
      std::cout << "bcm2835_init failed. Are you running as root??\n";
      return 1;
    }

    if (!bcm2835_spi_begin())
    {
      std::cout << "bcm2835_spi_begin failed. Are you running as root??\n";
      return 1;
    }

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    //bcm2835_spi_setClockDivider(divider); // The default
    //bcm2835_spi_set_speed_hz(1000000);
    shift = 0;
    this->setDivider();

    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    std::cout << "initialised bcm2835 library version: " << bcm2835_version() << std::endl;
    std::cout << "base clock speed: " << BCM2835_CORE_CLK_HZ/1000000.0 << " (MHz)" << std::endl;;


    std::cout << "SPI clock speed: " << (1.0*BCM2835_CORE_CLK_HZ/divider)/1000000.0 << " (MHz)" << std::endl;

    return 0;
}


uint16_t pi_spi_adc::mcp3201_readRaw()
{
    bcm2835_spi_transfern( spi_twoBytesBuffer, 2 );

    byteTwo = spi_twoBytesBuffer[1] >> 1;
	byteOne = spi_twoBytesBuffer[0] & 0b00011111;
	byteOne <<= 7;

    return byteOne + byteTwo;
}

float pi_spi_adc::mcp3201_readRaws( uint16_t *dataBuffer, float *timeBuffer, int bufferLength )
{
    dataTimer->start();

    acquistionTimer->start();
    for(int i = 0; i < bufferLength; i++ )
    {
        bcm2835_spi_transfern( spi_twoBytesBuffer, 2 );
        if( timeBuffer )
            timeBuffer[i] = dataTimer->currentDuration();

        byteTwo = spi_twoBytesBuffer[1] >> 1;
    	byteOne = spi_twoBytesBuffer[0] & 0b00011111;
    	byteOne <<= 7;
        dataBuffer[i] = byteOne + byteTwo;
    }
    acquistionTimer->stop();

    return acquistionTimer->totalDuration();
}

float pi_spi_adc::mcp3201_readValue()
{
    bcm2835_spi_transfern( spi_twoBytesBuffer, 2 );

    byteTwo = spi_twoBytesBuffer[1] >> 1;
	byteOne = spi_twoBytesBuffer[0] & 0b00011111;
	byteOne <<= 7;

    return conversion*(byteOne + byteTwo);
}

float pi_spi_adc::mcp3201_readValues( float *dataBuffer, float *timeBuffer, int bufferLength )
{


    acquistionTimer->start();
    dataTimer->start();
    for(int i = 0; i < bufferLength; i++ )
    {
        bcm2835_spi_transfern( spi_twoBytesBuffer, 2 );
        if( timeBuffer )
            timeBuffer[i] = dataTimer->currentDuration();

        byteTwo = spi_twoBytesBuffer[1] >> 1;
    	byteOne = spi_twoBytesBuffer[0] & 0b00011111;
    	byteOne <<= 7;
        dataBuffer[i] = (byteOne + byteTwo)*conversion;
    }
    acquistionTimer->stop();

    return acquistionTimer->totalDuration();
}

void pi_spi_adc::setAcquisitionTime( uint8_t sh )    //marker between 1 and
{
    shift = sh;

    this->setDivider();
}

void pi_spi_adc::setDivider()
{
    if( shift > 9 );
        shift = 9;

    if( shift == 9 )
        divider = 0;
    else
        divider = 128 << shift;

    bcm2835_spi_setClockDivider(divider);
}

void pi_spi_adc::increaseAcquitionTime()
{
    shift++;
    this->setDivider();
}

void pi_spi_adc::decreaseAcquitionTime()
{
    shift--;
    this->setDivider();
}
