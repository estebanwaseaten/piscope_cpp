#ifndef pi_input_h
#define pi_input_h

class pi_input
{
public:
    void inputLoop();
    bool isRunning();
    pi_input();
    ~pi_input();

private:
    bool        running;
    std::thread *inputThread;//(&pi_input::inputLoop, mainInput);
};

#endif
