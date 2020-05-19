const int cDefaultDataBufferSize = 256;

class pi_spi_adc
{
public:
    pi_spi_adc();
    pi_spi_adc( int dataBufferSize );
    ~pi_spi_adc();

private:
    int     dataBufferSize;
    float   *dataBuffer;
    float   *timeBuffer;
};
