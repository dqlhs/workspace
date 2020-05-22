#ifndef CTIMER_H
#define CTIMER_H

#include <pthread.h>
#include <sys/time.h>
class CTimer
{
public:
    CTimer();

    CTimer(long second, long microsecond);
    virtual ~CTimer();
    void SetTimer(long second,long microsecond,void(*callbackFunc)());
    void StartTimer();
    void StopTimer();
    bool Active();
private:
    pthread_t thread_timer;
    long m_second, m_microsecond;
    static void *OnTimer_stub(void *p)
    {
        (static_cast<CTimer*>(p))->thread_proc();
    }
   typedef void (*HsaeTimeout)();
    void thread_proc();
    void OnTimer();

    HsaeTimeout g_timeout;
};

#endif // CTIMER_H
