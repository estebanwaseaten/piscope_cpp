#include "pi_spi_adc.h"


pi_spi_adc::pi_spi_adc()
{
    //this->dataBufferSize = cDefaultDataBufferSize;
    //dataBuffer = new float[dataBufferSize];
    //timeBuffer = new float[dataBufferSize];
    this->setupSPI();
    dataTimer = new pi_timer();
    acquistionTimer = new pi_timer();
}

pi_spi_adc::pi_spi_adc( int dataBufferSize )
{
    //this->dataBufferSize = dataBufferSize;
    //dataBuffer = new float[dataBufferSize];
    //timeBuffer = new float[dataBufferSize];
    this->setupSPI();
    dataTimer = new pi_timer();
    acquistionTimer = new pi_timer();
}

pi_spi_adc::~pi_spi_adc()
{
    //delete dataBuffer;
    //delete timeBuffer;
    //close(spi_device):??????
    delete dataTimer;
    delete acquistionTimer;
}

void pi_spi_adc::setupSPI()
{
	if( (spiID = open(spi_device, O_RDWR)) < 0 )
	{
		std::cout << "ERROR opening device" << std::endl;
		exit(1);
	}

	//set mode:
	if( ioctl(spiID, SPI_IOC_WR_MODE, &this->spi_mode) < 0 )
	{
		std::cout << "ERROR setting device mode" << std::endl;
		exit(1);
	}
	if( ioctl(spiID, SPI_IOC_WR_BITS_PER_WORD, &spi_bits) < 0 )
	{
		std::cout << "ERROR setting device bits" << std::endl;
		exit(1);
	}
	if( ioctl(spiID, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0 )
	{
		std::cout << "ERROR setting device speed" << std::endl;
		exit(1);
	}

	if( ioctl(spiID, SPI_IOC_RD_MODE, &spi_mode) < 0 )
	{
		std::cout << "ERROR reading mode" << std::endl;
		exit(1);
	}

	if( ioctl(spiID, SPI_IOC_RD_BITS_PER_WORD, &spi_bits) < 0 )
	{
		std::cout << "ERROR reading bits" << std::endl;
		exit(1);
	}

	if( ioctl(spiID, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed) < 0 )
	{
		std::cout << "ERROR reading speed" << std::endl;
		exit(1);
	}

	std::cout << "spi device.....: " << spi_device << std::endl;
	std::cout << "spi mode.......: " << spi_mode << std::endl;
	std::cout << "bits per word..: " << spi_bits << std::endl;
	std::cout << "Speed..........: " << spi_speed <<" Hz (" << spi_speed/1000 << "kHz)\n" << std::endl;
}

float pi_spi_adc::readValue()
{
    return (this->*readValueFctn)();
}

float pi_spi_adc::readValues( float *dataBuffer, float *timeBuffer, int bufferLength )
{
    acquistionTimer->start();
    (this->*readValuesFctn)( dataBuffer, timeBuffer, bufferLength );
    acquistionTimer->stop();
    return acquistionTimer->totalDuration();
}

float pi_spi_adc::mcp3201_readvalues( float *dataBuffer, float *timeBuffer, int bufferLength )
{
    acquistionTimer->start();
    dataTimer->start();
    for(int i = 0; i< bufferLength; i++ )
    {
        dataBuffer[i] = this->mcp3201_readvalue();
        if( timeBuffer )
            timeBuffer[i] = dataTimer->currentDuration();
    }
    acquistionTimer->stop();
    return acquistionTimer->totalDuration();
}

float pi_spi_adc::mcp3201_readvalue()
{
	uint8_t	*byteBuffer;//, *blaBuffer;

	uint16_t byteOne;
	uint8_t byteTwo;

	int length = 2;		//reads two bytes

	byteBuffer = (uint8_t*)malloc(sizeof(__u8) * length);
//	blaBuffer = malloc( sizeof(__u8) * length);

	struct spi_ioc_transfer xfer;	//from spidef.h - contains a "message"
	memset(&xfer, 0, sizeof(xfer));

	xfer.tx_buf = (unsigned long)NULL;
	xfer.rx_buf = (unsigned long)byteBuffer;
	xfer.len = length;						//is this in bytes?
	xfer.delay_usecs = spi_delay;
	xfer.speed_hz = spi_speed;	//50kHz for now -- need opamp buffer to increase
	xfer.bits_per_word = 8;

	int status = ioctl(spiID, SPI_IOC_MESSAGE(1), &xfer);

	if( status < 0 )
	{
			//explain_ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
			printf("error - status: %d\n", status);
			free(byteBuffer);
			//free(blaBuffer);
			return 0;
	}

	//printf("status: %d\n", status);
	//printf("byteBuffer: %d\n", *byteBuffer);
	byteTwo = byteBuffer[1] >> 1;
	byteOne = byteBuffer[0] & 0b00011111;
	byteOne <<= 7;

	free(byteBuffer);
//	free(blaBuffer);

	return byteOne + byteTwo;
}
