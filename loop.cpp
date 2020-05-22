#include <iostream>
#include <chrono>




int main(void)
{
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> stop_time;
    std::chrono::duration<double> elapsedSeconds;

    std::ios_base::sync_with_stdio(false); //use '\n' instead of endl

    long counter = 0;
    long amount = 100000000;

    start_time = std::chrono::steady_clock::now();
    for( long i = 0; i < amount; i++ )
    {
        counter++;
    }
    stop_time = std::chrono::steady_clock::now();

    elapsedSeconds = stop_time - start_time;
    std::cout << "total time: " << elapsedSeconds.count() << " (s)" << std::endl;
    std::cout << "time per cylce: " << 1.0*elapsedSeconds.count()/amount << " (s)" << std::endl;

    return 0;
}
