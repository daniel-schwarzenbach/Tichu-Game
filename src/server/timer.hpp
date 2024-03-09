#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include "dschwarze.hpp"

class Timer {
     private:
          using time_t = std::chrono::time_point<std::chrono::system_clock>;
          time_t start_time, stop_time;
          bool stopped = false;
     public:
          Timer();
          void start();
          void stop();   
          double time();
};
Timer::Timer(){
     start_time = std::chrono::system_clock::now();
     stopped = false;
}
void Timer::start(){
     this->start_time = std::chrono::system_clock::now();
     stopped = false;
}
void Timer::stop(){
     this->stop_time = std::chrono::system_clock::now();
     stopped = true;
}

double Timer::time(){
    time_t current;
    if(stopped){
        current = stop_time;
    } else{
        current = std::chrono::system_clock::now();
    }
    return std::chrono::duration<double>(current-start_time).count();
}

#endif // TIME_HPP