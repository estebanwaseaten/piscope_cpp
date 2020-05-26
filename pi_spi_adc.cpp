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
    dividers[0] = BCM2835_SPI_CLOCK_DIVIDER_128;
    dividers[1] = BCM2835_SPI_CLOCK_DIVIDER_256;
    dividers[2] = BCM2835_SPI_CLOCK_DIVIDER_512;
    dividers[3] = BCM2835_SPI_CLOCK_DIVIDER_1024;
    dividers[4] = BCM2835_SPI_CLOCK_DIVIDER_2048;
    dividers[5] = BCM2835_SPI_CLOCK_DIVIDER_4096;
    dividers[6] = BCM2835_SPI_CLOCK_DIVIDER_8192;
    dividers[7] = BCM2835_SPI_CLOCK_DIVIDER_16384;
    dividers[8] = BCM2835_SPI_CLOCK_DIVIDER_32768;
    dividers[9] = BCM2835_SPI_CLOCK_DIVIDER_65536;

    setAcquisitionTime(3);
    //bcm2835_spi_setClockDivider(128);

    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    std::cout << "initialised bcm2835 library version: " << bcm2835_version() << std::endl;
    std::cout << "base clock speed: " << BCM2835_CORE_CLK_HZ/1000000.0 << " (MHz)" << std::endl;;


    std::cout << "SPI clock speed: " << (1.0*BCM2835_CORE_CLK_HZ/dividers[dividerIndex])/1000000.0 << " (MHz)" << std::endl;

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

void pi_spi_adc::setAcquisitionTime( uint8_t i )    //marker between 1 and
{
    dividerIndex = i;

    this->setDivider();
}

uint16_t pi_spi_adc::getDivider()
{
    return dividers[dividerIndex];
}

uint8_t pi_spi_adc::getDividerIndex()
{
    return dividerIndex;
}

void pi_spi_adc::increaseAcquisitionTime()
{
    dividerIndex++;
    this->setDivider();
}

void pi_spi_adc::decreaseAcquisitionTime()
{
    if( dividerIndex != 0 )
        dividerIndex--;
    this->setDivider();
}

void pi_spi_adc::setDivider()
{
    if( dividerIndex > 9 )
        dividerIndex = 9;

//    std::cout << "i: " << dividerIndex << " div: " << dividers[dividerIndex];

    bcm2835_spi_setClockDivider(dividers[dividerIndex]);
}
