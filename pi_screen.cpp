#include <iostream>
#include <cstring>  //needed for memset etc.... could do this the c++ way, but I like the buffers fast for now
#include <string>
#include <cmath>
#include <cstdio>

#include "pi_screen.h"


pi_screen::pi_screen()
{
    this->setBoundaries( gDefaultScreenSizeX, gDefaultScreenSizeY, gDefaultPaddingX, gDefaultPaddingY );
    defaultValue = 'O';
    this->createBuffers();
}

pi_screen::pi_screen( int scrSizeX, int scrSizeY )
{
    this->setBoundaries( scrSizeX, scrSizeY, gDefaultPaddingX, gDefaultPaddingY );
    defaultValue = 'O';
    this->createBuffers();
}

pi_screen::pi_screen( int scrSizeX, int scrSizeY, int scrPadX, int scrPadY )
{
    this->setBoundaries( scrSizeX, scrSizeY, scrPadX, scrPadY );
    defaultValue = 'O';
    this->createBuffers();
}

void pi_screen::setBoundaries( int scrSizeX, int scrSizeY, int scrPadX, int scrPadY )
{
    screenSizeX = scrSizeX;
    screenSizeY = scrSizeY;
    screenPaddingX = scrPadX;
    screenPaddingY = scrPadY;
    screenDataAreaX = screenSizeX - gDefaultPaddingX;
    screenDataAreaY = screenSizeY - gDefaultPaddingY;
    totalPixelCount = screenSizeX*screenSizeY;
}

void pi_screen::createBuffers()
{
    screen = new char[totalPixelCount];
    overlay = new char[totalPixelCount];
    dataBuffer = new float[screenDataAreaX];
}

pi_screen::~pi_screen()
{
    delete screen;
}

//takes some arbitrary data and bins it into the data buffer.... maybe... (where to trigger)
void pi_screen::binIntoDataBuffer( float *values, int numValues )
{

}

void pi_screen::prepareScreen() //set screen to all blank, then draw data from *dataBuffer, then overlay form *overlay
{
    memset(screen, ' ', totalPixelCount );
    //this->drawData();
    this->drawOverlay();
}

void pi_screen::clearScreen()
{
    std::cout << "\033c";
}

void pi_screen::drawScreen()
{
    //std::cout << std::endl;
    this->clearScreen();
    for( int y = screenSizeY -1; y >= 0; y-- )
    {
        for( int x = 0; x < screenSizeX; x++ )
        {
            //std::cout << screen[x * screenSizeY + y];
            printf( "%c", screen[x * screenSizeY + y]);
        }
        printf("\n");
        //std::cout << std::endl;
    }
}

void pi_screen::prepareOverlay()
{
    float ymin = y0 - yRange2;
    float ymax = y0 + yRange2;

    int midpos = (screenPaddingY + screenSizeY)/2;
    int toppos = screenSizeY-1;

    memset( overlay, ' ', totalPixelCount);

    std::string mid = std::to_string(y0);
    std::string top = std::to_string(ymax);
    std::string bot = std::to_string(ymin);

    for( int i = 0; i < 3; i++ )
    {
        this->setOverlayPixel( i, screenPaddingY, bot.at(i) );
        this->setOverlayPixel( i, midpos, mid.at(i) );
        this->setOverlayPixel( i, toppos, top.at(i) );
    }

    //y-axis
    for(int i = screenPaddingY-1; i < screenSizeY; i++ )
    {
        this->setOverlayPixel( (screenPaddingX-1), i ,(char)124);
    }

    //x-axis
    for( int i = screenPaddingX-1; i  < screenSizeX; i++ )
    {
        this->setOverlayPixel( i, (screenPaddingY-1) , '_');
    }
}

void pi_screen::drawOverlay()
{
    char pixel;
    for( int y = 0; y < screenSizeY; y++ )
    {
        for( int x = 0; x < screenSizeX; x++ )
        {
            pixel = overlay[ x * screenSizeY + y ];
            if( pixel != ' ' )
            {
                screen[ x * screenSizeY + y ] = pixel;
            }
        }
    }
}

void pi_screen::prepareData( float *sourceBuffer, int sourceBufferLength )
{
    //copies data into data buffer
}

void pi_screen::drawData()
{
    float yRange = 2*yRange2;		//this is mapped onto ysize-8	(8 is some arbitrary space)
	float yMin = y0 - yRange2;
	float yMax = y0 + yRange2;

	//calculate mapping:
	float dy = yRange/(screenSizeY - screenPaddingY);	//debug 28

	// loop through x and set pixel at y:
	int i = 0;
    int yPixel;
	for( int x = screenPaddingX; x < screenSizeX; x++ )	//loop through x values
	{
		if( i < numDataPoints )
		{
				if( (dataBuffer[i] >= yMin) && (dataBuffer[i] <= yMax ) && !std::isnan(dataBuffer[i]) )
				{
					yPixel = std::lround( (dataBuffer[i]-yMin) / dy ) + screenPaddingY;// + SCREEN_YPAD;
					this->setPixel( x, yPixel );
				}
			i++;
		}
	}
}



void pi_screen::setOverlayPixel( int x, int y, char value)
{
    if( (x < screenSizeX) && ( y < screenSizeY) )
        overlay[ x * screenSizeY + y ] = value;
}

void pi_screen::setPixel( int x, int y, char value)
{
    if( (x < screenSizeX) && ( y < screenSizeY) )
		screen[ x * screenSizeY + y ] = value;
}

void pi_screen::setPixel( int x, int y )
{
    if( (x < screenSizeX) && ( y < screenSizeY) )
		screen[ x * screenSizeY + y ] = defaultValue;
}
