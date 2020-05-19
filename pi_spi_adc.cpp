#include "pi_spi_adc.h"


pi_spi_adc::pi_spi_adc()
{
    this->dataBufferSize = cDefaultDataBufferSize;
    dataBuffer = new float[dataBufferSize];
    timeBuffer = new float[dataBufferSize];
}

pi_spi_adc::pi_spi_adc( int dataBufferSize )
{
    this->dataBufferSize = dataBufferSize;
    dataBuffer = new float[dataBufferSize];
    timeBuffer = new float[dataBufferSize];
}

pi_spi_adc::~pi_spi_adc()
{
    delete dataBuffer;
    delete timeBuffer;
}
