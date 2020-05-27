#ifndef pi_spi_adc_h
#define pi_spi_adc_h

#include <cstdint>
#include <cstring>
#include <cmath>
#include <iostream>
#include <bcm2835.h>

#include "pi_timer.h"

class pi_spi_adc
{
public:
    pi_spi_adc();
    ~pi_spi_adc();

    uint16_t  mcp3201_readRaw();
    float     mcp3201_readRaws( uint16_t *dataBuffer, float *timeBuffer, int bufferLength );

    void   setAcquisitionTime( uint8_t i );
    void        setDivider();
    uint16_t    getDivider();
    uint8_t     getDividerIndex();
    void   increaseAcquisitionTime();
    void   decreaseAcquisitionTime();

    float   mcp3201_readValue();
    float   mcp3201_readValues( float *dataBuffer, float *timeBuffer, int bufferLength );

//    float (pi_spi_adc::*readValueFctn)() = &pi_spi_adc::mcp3201_readvalue;
    //AdcReadValueFctn readValueFunction = &pi_spi_adc::mcp3201_readvalue;        //can set this to others for other ads or other methods
//    float (pi_spi_adc::*readValuesFctn)( float *dataBuffer, float *timeBuffer, int bufferLength) = &pi_spi_adc::mcp3201_readvalues;

private:

    int setupSPI();


    //SPI setup parameters
    //float   divider = BCM2835_SPI_CLOCK_DIVIDER_128;
    uint8_t     dividerIndex;
    static const uint8_t numDividers = 13;
    uint16_t    dividers[numDividers];

    float       conversion = 3.3/4096;

    char	    *spi_twoBytesBuffer;
    uint16_t    byteOne;
	uint8_t     byteTwo;

    pi_timer    *dataTimer;
    pi_timer    *acquistionTimer;

};


#endif
