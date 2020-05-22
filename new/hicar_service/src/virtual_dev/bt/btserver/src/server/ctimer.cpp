//#include "ctimer.h"

//CTimer::CTimer()
//{

//}
//CTimer.cpp:
/*
* CTimer.cpp
*
* Created on: 2009-7-13
*      Author: DEAN
*/

#include "ctimer.h"
#include <iostream>
#include <sys/select.h>
#include <time.h>
#include <pthread.h>

using namespace std;
//////////////////////////public methods//////////////////////////
CTimer::CTimer():
    m_second(0), m_microsecond(0),g_timeout(NULL),thread_timer(0)
{
}

CTimer::CTimer(long second, long microsecond) :
    m_second(second), m_microsecond(microsecond)
{
}

CTimer::~CTimer()
{
}

//void CTimer::SetTimer(long second, long microsecond, void *callbackFunc())
//{

//}

void CTimer::SetTimer(long second, long microsecond, void (*callbackFunc)())
{
    m_second = second;
    m_microsecond = microsecond;
    g_timeout = callbackFunc;
}

//void CTimer::SetTimer(long second, long microsecond)
//{

//}

void CTimer::StartTimer()
{
   if (thread_timer ==0)
   {
       pthread_create(&thread_timer, NULL, OnTimer_stub, this);

   }
   else
   {
       StopTimer();
       pthread_create(&thread_timer, NULL, OnTimer_stub, this);
   }

}
void CTimer::StopTimer()
{
    pthread_cancel(thread_timer);
    pthread_join(thread_timer, NULL); //wait the thread stopped
    thread_timer = 0;
    printf("StopTimer end \r\n");
}

bool CTimer::Active()
{
    if (thread_timer != 0)
        return true;
    else
        return false;
}

//////////////////////////private methods//////////////////////////
void CTimer::thread_proc()
{
    while (true)
    {
//        OnTimer(); 
        struct timeval tempval;
        tempval.tv_sec = m_second;
        tempval.tv_usec = m_microsecond;
        select(0, NULL, NULL, NULL, &tempval);
        g_timeout();
        pthread_testcancel();
    }
    return  ;
}

void CTimer::OnTimer()
{
    cout<<"Timer once..."<<endl;
}
