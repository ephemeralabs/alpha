// TODO(eduponz): alpha copyright notice

#include <atomic>
#include <condition_variable>
#include <csignal>
#include <iostream>
#include <mutex>
#include <thread>

#include <wiringPi.h>

std::atomic_bool run{false};
std::condition_variable run_cv;
std::mutex run_mtx;

void signal_handler_callback(
        int signum)
{
    std::cout << std::endl << "Caught signal " << signum << "; closing down..." << std::endl;
    {
        std::unique_lock<std::mutex> lck(run_mtx);
        run.store(false);
    }
    run_cv.notify_one();
}

void blink(const int led)
{
    pinMode(led, OUTPUT);

    while(run)
    {
        digitalWrite(led, HIGH);
        delay(500);
        digitalWrite(led, LOW);
        delay(500);
    }
}

int main()
{
    wiringPiSetup();

    run.store(true);
    std::thread working_thread(&blink, 5);
    signal(SIGINT, signal_handler_callback);

    std::unique_lock<std::mutex> lck(run_mtx);
    run_cv.wait(lck, [&]()
            {
                return !run.load();
            });

    working_thread.join();

    return 0;
}
