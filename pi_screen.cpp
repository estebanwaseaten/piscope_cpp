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
    screen = new char[totalPixelCount + screenSizeY + 1];   // add one column for linefeed chars + 1 for c termination
    blankScreen = new char[totalPixelCount + screenSizeY + 1];
    memset(blankScreen, ' ', totalPixelCount + screenSizeY + 1);
    for( int i = 0; i < screenSizeY; i++ )
    {
        blankScreen[screenSizeX + i*(screenSizeX+1)] = '\n'; //linefeed char for c string
    }
    blankScreen[totalPixelCount + screenSizeY] ='\0'; //termination char for c string

    memcpy(screen, blankScreen, totalPixelCount + screenSizeY + 1 );

    overlay = new char[totalPixelCount];

    dataBuffer = new float[numDataPoints];          //allocated here... others may write into dataInputBuffer...
    dataInputBuffer = new float[numDataPoints];
    defaultValue = 'O';
    screenThread = NULL;
    std::cout << "initScreen()\n";
}

pi_screen::~pi_screen()
{

    delete screen;
    delete overlay;
    delete dataBuffer;
    if( screenThread )
    {
        screenThread->detach();
        delete screenThread;
    }
    std::cout << "pi_screen::~pi_screen()" << std::endl;
}

void pi_screen::start()
{
    screenThread = new std::thread( &pi_screen::screenLoop, this );
    running = true;
    std::cout << "startScreen()\n";
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
    std::chrono::milliseconds timespan(100);

    while(running)
    {
        std::this_thread::sleep_for(timespan);  //sleep for 100ms
        if( redraw )
        {
            redraw = false;     //something needs to set redraw to true again
            switch (screenMode)
            {
                case kScreenModeGraph:
                    this->prepareScreen();      //prepares "screen" buffer
                    this->clearScreen();        //clear terminal
                    this->drawScreen();         // draw screen buffer to terminal
                    break;
                case kScreenModeList:
                    this->clearScreen();        //clear terminal
                    this->dumpData();           //simply print Data Buffer
                    break;
                default:
                    break;
            }
        }

    }
}


void pi_screen::prepareScreen() //set screen to all blank, then draw data from *dataBuffer, then overlay form *overlay
{
    memcpy(screen, blankScreen, totalPixelCount + screenSizeY + 1);

    this->drawData();
    this->drawOverlay();
}

void pi_screen::drawScreen()        //line by line drawing prefers consecutive chars in x directions
{
    printf( "%s", screen );
}


void pi_screen::dumpData()
{

    for( int i = 0; i < numDataPoints; i++ )
    {
        std::cout << dataBuffer[i] << " - ";
    }
    std::cout << std::endl;
}

void pi_screen::clearScreen()
{
    std::cout << "\033c";
}

void pi_screen::setModeGraph()
{
    screenMode = kScreenModeGraph;
}

void pi_screen::setModeList()
{
    screenMode = kScreenModeList;
}

void pi_screen::changeMode()
{
    if( screenMode == kScreenModeGraph )
        screenMode = kScreenModeList;
    else
        screenMode = kScreenModeGraph;
}

void pi_screen::rndTestData( float start, float stop, int seed )
{
    float range = stop-start;
    std::srand(seed);

    for( int i = 0; i < numDataPoints; i++)
    {
        dataInputBuffer[i] = (std::rand()/(float)RAND_MAX)*range + start;
    }
    memcpy(dataBuffer, dataInputBuffer, numDataPoints*sizeof(float));
}

float *pi_screen::getDataInputBuffer()
{
    return dataInputBuffer;
}

int pi_screen::getDataBufferLength()
{
    return numDataPoints;
}

void pi_screen::swapBuffer()
{
    memcpy(dataBuffer, dataInputBuffer, numDataPoints*sizeof(float));
    redraw = true;
}

void pi_screen::sinTestData( float start, float stop, float freq, int seed )
{
    float range = stop-start;
    float mid = (stop+start)/2;

    std::srand(seed);
    float offset = (std::rand()/(float)RAND_MAX)*10;

    for( int i = 0; i < numDataPoints; i++)
    {
        dataInputBuffer[i] = std::sin( i*freq + offset)*range/2.0 + mid;
    }
    memcpy(dataBuffer, dataInputBuffer, numDataPoints*sizeof(float));
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

void pi_screen::setNote( int index, std::string content )
{
    if( index >= 0 && index < 3 )
    {
        notes[index] = content;
    }
    this->prepareOverlay();
}

void pi_screen::clearNote( int index )
{
    if( index >= 0 && index < 3 )
    {
        notes[index] = "";
    }
    this->prepareOverlay();
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


    // draw y axis description
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

    //draw notes:
    for(int i = 0; i < 10; i++ )
    {
        //first line:
        if( !notes[0].empty() && notes[0].size() > i )
            this->setOverlayPixel( i, (screenPaddingY-2), notes[0].at(i));
        if( !notes[1].empty() && notes[1].size() > i )
            this->setOverlayPixel( i, (screenPaddingY-3), notes[1].at(i));
        if( !notes[2].empty() && notes[2].size() > i )
            this->setOverlayPixel( i, (screenPaddingY-4), notes[2].at(i));/**/
    }
    redraw = true;
}

void pi_screen::drawOverlay()
{
    char pixel;
    for( int y = 0; y < screenSizeY; y++ )
    {
        for( int x = 0; x < screenSizeX; x++ )
        {
            pixel = overlay[ y * screenSizeX + x ];
            if( pixel != ' ' )
            {
                this->setPixel( x, y, pixel);
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
	float dy = yRange/(screenSizeY - screenPaddingY);

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
        overlay[ y * screenSizeX + x ] = value;
}

void pi_screen::setPixel( int x, int y, char value)
{
    if( (x < screenSizeX) && ( y < screenSizeY) )
		screen[ (screenSizeY-1-y) * (screenSizeX+1) + x ] = value;
}

void pi_screen::setPixel( int x, int y )
{
    if( (x < screenSizeX) && ( y < screenSizeY) )
		screen[ (screenSizeY-1-y) * (screenSizeX+1) + x ] = defaultValue;
}
