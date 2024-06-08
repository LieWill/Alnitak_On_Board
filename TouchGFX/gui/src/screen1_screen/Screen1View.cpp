#include <gui/screen1_screen/Screen1View.hpp>

// My code
#include <gui/screen1_screen/Screen1View.hpp>
#include "rtc.h"

extern RTC_TimeTypeDef GetTime;
extern RTC_DateTypeDef GetDate;

Screen1View::Screen1View()
{
    static int n = 0;
    n++;
    if (n == 24)
        n = 0;
    analogClock1.setTime24Hour(n, 2, 10);
    analogHours = analogClock1.getCurrentHour();
    analogMinutes = analogClock1.getCurrentMinute();
    analogSeconds = analogClock1.getCurrentSecond();
}

void Screen1View::setupScreen()
{

    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
    tickCounter++;

    // if (tickCounter % 5 == 0)
    {
        //  Update the clocks
        analogClock1.setTime24Hour(GetTime.Hours, GetTime.Minutes, GetTime.Seconds);
    }
}
