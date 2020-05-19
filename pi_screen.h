#ifndef pi_screen_h
#define pi_screen_h

class pi_screen     //could subclass screen to data_screen
{
public:
    pi_screen();
    pi_screen( int scrSizeX, int scrSizeY );
    pi_screen( int scrSizeX, int scrSizeY, int scrPadX, int scrPadY );

    ~pi_screen();

    void clearScreen();     //empties the screen
    void prepareScreen();   //prepares screen --> calls
    void drawScreen();      //prints the actual *screen buffer to the console via cout

    void prepareOverlay();      //fills *overlay Buffer
    void drawOverlay();         //copies *overlay buffer onto *screen buffer

    void binIntoDataBuffer( float *values, int numValues );     //the screens data buffer has exactly the amount of Data points needed
    void prepareData( float *sourceBuffer, int sourceBufferLength );         //fills *dataBuffer
    void drawData();            //writes data to *screen buffer according to data in *dataBuffer

    void setPixel( int x, int y, char value);
    void setPixel( int x, int y );
    void setOverlayPixel( int x, int y, char value);

private:
    //char 	*screen;     //screen buffer
    //char 	*overlay;     //screen buffer
    char 	*screen;     //screen buffer
    char 	*overlay;     //screen buffer
    float 	*dataBuffer;     //screen buffer     should we maintain this or just the pointer to it??
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

    void createBuffers();
    void setBoundaries( int scrSizeX, int scrSizeY, int scrPadX, int scrPadY );
};

const int   gDefaultScreenSizeX = 134;
const int   gDefaultScreenSizeY = 38;
const int   gDefaultPaddingX = 6;
const int   gDefaultPaddingY = 6;

#endif
