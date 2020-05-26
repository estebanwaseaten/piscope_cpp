#ifndef pi_spi_adc_h
#define pi_spi_adc_h

#include <cstdint>
#include <cstring>
#include <iostream>
#include <bcm2835.h>

#include "pi_timer.h"


// BCM2835_SPI_CLOCK_DIVIDER_65536 = 0,       /*!< 65536 = 3.814697260kHz on Rpi2, 6.1035156kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_32768 = 32768,   /*!< 32768 = 7.629394531kHz on Rpi2, 12.20703125kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_16384 = 16384,   /*!< 16384 = 15.25878906kHz on Rpi2, 24.4140625kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192,    /*!< 8192 = 30.51757813kHz on Rpi2, 48.828125kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_4096  = 4096,    /*!< 4096 = 61.03515625kHz on Rpi2, 97.65625kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_2048  = 2048,    /*!< 2048 = 122.0703125kHz on Rpi2, 195.3125kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_1024  = 1024,    /*!< 1024 = 244.140625kHz on Rpi2, 390.625kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_512   = 512,     /*!< 512 = 488.28125kHz on Rpi2, 781.25kHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_256   = 256,     /*!< 256 = 976.5625kHz on Rpi2, 1.5625MHz on RPI3 */
// BCM2835_SPI_CLOCK_DIVIDER_128   = 128,     /*!< 128 = 1.953125MHz on Rpi2, 3.125MHz on RPI3 */


class pi_spi_adc
{
public:
    pi_spi_adc();
    ~pi_spi_adc();

    uint16_t  mcp3201_readRaw();
    float     mcp3201_readRaws( uint16_t *dataBuffer, float *timeBuffer, int bufferLength );

    void   setAcquisitionTime( uint8_t sh );
    void   setDivider();
    void   increaseAcquitionTime();
    void   decreaseAcquitionTime();

    float   mcp3201_readValue();
    float   mcp3201_readValues( float *dataBuffer, float *timeBuffer, int bufferLength );

//    float (pi_spi_adc::*readValueFctn)() = &pi_spi_adc::mcp3201_readvalue;
    //AdcReadValueFctn readValueFunction = &pi_spi_adc::mcp3201_readvalue;        //can set this to others for other ads or other methods
//    float (pi_spi_adc::*readValuesFctn)( float *dataBuffer, float *timeBuffer, int bufferLength) = &pi_spi_adc::mcp3201_readvalues;

private:

    int setupSPI();


    //SPI setup parameters
    //float   divider = BCM2835_SPI_CLOCK_DIVIDER_128;
    uint8_t     shift = 0;
    int         divider = 128;
    float       conversion = 3.3/4096;

    char	    *spi_twoBytesBuffer;
    uint16_t    byteOne;
	uint8_t     byteTwo;

    pi_timer    *dataTimer;
    pi_timer    *acquistionTimer;

};


#endif
