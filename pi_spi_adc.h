#ifndef pi_spi_adc_h
#define pi_spi_adc_h

#include <cstdint>
#include <cstring>
#include <fcntl.h>				//SPI
#include <sys/ioctl.h> 			//SPI
#include <linux/spi/spidev.h>	//SPI
#include <iostream>

#include "pi_timer.h"

const int cDefaultDataBufferSize = 256;

class pi_spi_adc
{
public:
    pi_spi_adc();
    pi_spi_adc( int dataBufferSize );
    ~pi_spi_adc();

    void setupSPI();
    void prepCom();

    float   readValue();
    float    readValues( float *dataBuffer, float *timeBuffer, int bufferLength );   //-> generic call

    float   mcp3201_readvalue();
    float   mcp3201_readvalues( float *dataBuffer, float *timeBuffer, int bufferLength );


    float (pi_spi_adc::*readValueFctn)() = &pi_spi_adc::mcp3201_readvalue;
    //AdcReadValueFctn readValueFunction = &pi_spi_adc::mcp3201_readvalue;        //can set this to others for other ads or other methods
    float (pi_spi_adc::*readValuesFctn)( float *dataBuffer, float *timeBuffer, int bufferLength) = &pi_spi_adc::mcp3201_readvalues;


private:
    //int     dataBufferSize;
    //float   *dataBuffer;
    //float   *timeBuffer;
    pi_timer    *dataTimer;
    pi_timer    *acquistionTimer;

    //SPI parameters
    int     spiID;
    uint8_t	*byteBuffer;
    int     byteBufferLength = 2;		//reads two bytes
    struct spi_ioc_transfer xfer;	//from spidef.h - contains a "message"


    const char *spi_device = "/dev/spidev0.0";
    uint8_t  spi_mode{0};
    uint8_t  spi_bits{8};
    uint32_t spi_speed{1000000};   //chip speed in Hz
    uint16_t spi_delay;


};


#endif
