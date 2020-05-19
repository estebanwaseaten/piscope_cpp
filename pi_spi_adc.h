#ifndef pi_spi_adc_h
#define pi_spi_adc_h

#include <cstdint>
#include <fcntl.h>				//SPI
#include <sys/ioctl.h> 			//SPI
#include <linux/spi/spidev.h>	//SPI
#include <iostream>


const int cDefaultDataBufferSize = 256;

class pi_spi_adc
{
public:
    pi_spi_adc();
    pi_spi_adc( int dataBufferSize );
    ~pi_spi_adc();

    void mcp3201_setup( void );
    uint16_t mcp3201_readvalue(int);


private:
    int     dataBufferSize;
    float   *dataBuffer;
    float   *timeBuffer;

    int     spiID;

    //SPI parameters
    const char *spi_device = "/dev/spidev0.0";
    uint8_t  spi_mode;
    uint8_t  spi_bits{8};
    uint32_t spi_speed{100000};   //chip speed in Hz
    uint16_t spi_delay;


};


#endif
