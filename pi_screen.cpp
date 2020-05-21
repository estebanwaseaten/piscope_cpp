#include "pi_screen.h"


pi_screen::pi_screen()
{
    this->setBoundaries( gDefaultScreenSizeX, gDefaultScreenSizeY, gDefaultPaddingX, gDefaultPaddingY );
    this->initScreen();
}

pi_screen::pi_screen( int scrSizeX, int scrSizeY )
{
    this->setBoundaries( scrSizeX, scrSizeY, gDefaultPaddingX, gDefaultPaddingY );
    this->initScreen();
}

pi_screen::pi_screen( int scrSizeX, int scrSizeY, int scrPadX, int scrPadY )
{
    this->setBoundaries( scrSizeX, scrSizeY, scrPadX, scrPadY );
    this->initScreen();
}

void pi_screen::initScreen()
{
    screen = new char[totalPixelCount];
    overlay = new char[totalPixelCount];
    dataBuffer = new float[numDataPoints];
    defaultValue = 'O';
    screenThread = new std::thread( &pi_screen::screenLoop, this );
    running = true;
}

pi_screen::~pi_screen()
{

    delete screen;
    delete overlay;
    delete dataBuffer;
    screenThread->detach();
    delete screenThread;
    std::cout << "pi_screen::~pi_screen()" << std::endl;
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

    numDataPoints = screenDataAreaX;
}

void pi_screen::screenLoop()
{
    std::chrono::milliseconds timespan(200);

    while(running)
    {
        std::this_thread::sleep_for(timespan);  //sleep for 100ms
        this->prepareScreen();
        this->drawScreen();
    }
}

float *pi_screen::getDataBuffer()
{
    return dataBuffer;
}

int pi_screen::getDataBufferLength()
{
    return numDataPoints;
}

//takes some arbitrary data and bins it into the data buffer.... maybe... (where to trigger)
void pi_screen::binIntoDataBuffer( float *values, int numValues )
{

}

void pi_screen::rndTestData( float start, float stop )
{
    float range = stop-start;

    for( int i = 0; i < numDataPoints; i++)
    {
        dataBuffer[i] = (std::rand()/(float)RAND_MAX)*range + start;
    }
}

void pi_screen::sinTestData( float start, float stop, float freq )
{
    float range = stop-start;
    float mid = (stop+start)/2;
    std::srand(std::time(NULL));
    float offset = (std::rand()/(float)RAND_MAX)*10;

    for( int i = 0; i < numDataPoints; i++)
    {
        dataBuffer[i] = std::sin( i*freq + offset)*range/2.0 + mid;
    }
}


void pi_screen::setYAxis( float mid, float halfrange )
{
    this->y0 = mid;
    this->yRange2 = halfrange;
    this->prepareOverlay();
}

void pi_screen::setXAxis( float mid, float halfrange )
{
    this->x0 = mid;
    this->xRange2 = halfrange;
    this->prepareOverlay();
}

void pi_screen::prepareScreen() //set screen to all blank, then draw data from *dataBuffer, then overlay form *overlay
{
    memset(screen, ' ', totalPixelCount );
    this->drawData();
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
            std::cout << screen[x * screenSizeY + y];
            //printf( "%c", screen[x * screenSizeY + y]);
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

    std::string mid = std::to_string(std::abs(y0));
    std::string top = std::to_string(std::abs(ymax));
    std::string bot = std::to_string(std::abs(ymin));

    if( ymin < 0 )
        this->setOverlayPixel( 0, screenPaddingY, '-' );
    if( y0 < 0 )
        this->setOverlayPixel( 0, midpos, '-' );
    if( ymax < 0 )
        this->setOverlayPixel( 0, toppos, '-' );


    for( int i = 0; i < 4; i++ )
    {
        this->setOverlayPixel( i+1, screenPaddingY, bot.at(i) );
        this->setOverlayPixel( i+1, midpos, mid.at(i) );
        this->setOverlayPixel( i+1, toppos, top.at(i) );
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
