#ifndef pi_screen_h
#define pi_screen_h

#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>  //needed for memset etc.... could do this the c++ way, but I like the buffers fast for now
#include <string>
#include <cmath>
#include <cstdio>
#include <cstdlib>  //rand
#include <time.h>       /* time */


class pi_screen     //could subclass screen to data_screen
{
public:
    pi_screen();
    pi_screen( int scrSizeX, int scrSizeY );
    pi_screen( int scrSizeX, int scrSizeY, int scrPadX, int scrPadY );

    ~pi_screen();

    void start();
    void screenLoop();

    void clearScreen();     //empties the screen
    void prepareScreen();   //prepares screen --> calls
    void drawScreen();      //prints the actual *screen buffer to the console via cout

    void prepareOverlay();      //fills *overlay Buffer
    void setXAxis( float mid, float halfrange );
    void setYAxis( float mid, float halfrange );
    void drawOverlay();         //copies *overlay buffer onto *screen buffer

    void binIntoDataBuffer( float *values, int numValues );     //the screens data buffer has exactly the amount of Data points needed
    void prepareData( float *sourceBuffer, int sourceBufferLength );         //fills *dataBuffer
    void drawData();            //writes data to *screen buffer according to data in *dataBuffer

    void setPixel( int x, int y, char value );
    void setPixel( int x, int y );
    void setOverlayPixel( int x, int y, char value );

    void rndTestData( float start, float stop, int seed );
    void sinTestData( float start, float stop, float freq, int seed );

    float   *getDataBuffer();
    int     getDataBufferLength();
    void    swapBuffer();


private:
    char 	*screen;         //2D screen buffer
    char 	*blankScreen;    //2D screen buffer   memcopy to erase...
    char 	*overlay;        //2D overlay buffer
    float   *dataInputBuffer;
    float 	*dataBuffer;     //1D data buffer
    int     numDataPoints;

    char    defaultValue;
    int     screenSizeX;
    int     screenSizeY;
    int     totalPixelCount;
    int     screenPaddingX;
    int     screenPaddingY;
    int     screenDataAreaX;
    int     screenDataAreaY;

    //overlay parameters (could separate this out to own class - but dont)
    float   y0;
    float   yRange2;
    float   x0;
    float   xRange2;

    std::thread *screenThread;
    bool        running;

    void initScreen();
    void setBoundaries( int scrSizeX, int scrSizeY, int scrPadX, int scrPadY );
};

const int   gDefaultScreenSizeX = 134;
const int   gDefaultScreenSizeY = 38;
const int   gDefaultPaddingX = 6;
const int   gDefaultPaddingY = 6;

#endif
