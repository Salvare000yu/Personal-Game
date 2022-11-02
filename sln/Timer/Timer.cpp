#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

Timer* Timer::GetInstance()
{
    static Timer instance;
    return &instance;
}

void Timer::TimerPlay(bool TimeResetFlag)
{
    if (TimeResetFlag == true)
    {
        time = frame / 60;	// 60fps‘z’è
        frame += 1.f;
    }
    else {
        frame = 0;
        TimeResetFlag = true;
    }
}

void Timer::TitleEaseFrame()
{
    titleEaseFrame+=1.f;
}