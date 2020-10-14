#pragma once
#include<mutex>
#include<atomic>
#include<Windows.h>


class CSpinLock{
private:
    LONG                    spinLock_;
    int                     lockValue_=0;

    bool        CAS(int*  addr, int  expected, int new_val);
    void        AcquireSpinLock(volatile LONG*)noexcept;
    void        ReleaseSpinLock(volatile LONG*)noexcept;
public:
    CSpinLock() = default;
    ~CSpinLock() = default;
    
    void        lock()noexcept;
    void        unlock()noexcept;
};
