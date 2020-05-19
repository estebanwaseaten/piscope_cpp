

class pi_timer
{
public:
    pi_timer();
    ~pi_timer();

    void    start();
    void    reset();
    void    stop();
    void    increaseCounter();
    double  currentDuration(); //costly
    double  totalDuration();
    long    getCounter();
    double  cycleDuration();
    double  averageFrequency();
    void    printReport();
    void    setCounter( long c );
    long    counter;

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> stopTime;


    std::chrono::duration<double> elapsedSeconds;
    bool running;

};
