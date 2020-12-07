#pragma once
#include<mutex>
#include<atomic>
#include<Windows.h>



class CSpinLock {
    const int      LOCK_AVAIL = 0;
private:
#ifdef WIN32
    volatile DWORD  state_;
#else
    volatile LONG LONG  state_;
#endif // WIN32
    DWORD            CAS(DWORD volatile*, int, int);
public:
    CSpinLock();
    ~CSpinLock() = default;

    void            lock()noexcept;
    void            unlock()noexcept;
};


