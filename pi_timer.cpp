#include <chrono>
#include <iostream>

#include "pi_timer.h"



pi_timer::pi_timer()
{
    //startTime = 0;
    //stopTime = 0;
    counter = 0;
    running = false;
}

pi_timer::~pi_timer()
{

}

//starts timer and also resets stop time
void pi_timer::start()
{
    this->reset();
    //stopTime = 0;
    //elapsedSeconds = 0;
    running = true;
}

void pi_timer::reset()
{
    startTime = std::chrono::steady_clock::now();
    counter = 0;
}

void pi_timer::stop()
{
    stopTime = std::chrono::steady_clock::now();
    elapsedSeconds = stopTime - startTime;
    running = false;
}

void pi_timer::increaseCounter()    //is slower than a raw counter++ due to the function call - use your own counter & setCounter() to increase speed
{
    if( running )
        counter++;
}

void pi_timer::setCounter( long c )
{
    counter = c;
}

double pi_timer::totalDuration()
{
    return elapsedSeconds.count();
}

double pi_timer::currentDuration()
{
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> curDuration = currentTime - startTime;
    return curDuration.count();
}

long pi_timer::getCounter()
{
    return this->counter;
}

double pi_timer::cycleDuration()
{
    return elapsedSeconds.count()/counter;
}

double pi_timer::averageFrequency()
{
    return counter/elapsedSeconds.count();
}

void pi_timer::printReport()
{
    if( !running )
        std::cout << "totalDuration (s): " << elapsedSeconds.count() << "; counter: " << counter << "; cycleDuration (s): " << cycleDuration() << "; avg. Frequency (kHz): " << averageFrequency()/1000.0 << std::endl;
    else
        std::cout << "timer still running!" << std::endl;
}
