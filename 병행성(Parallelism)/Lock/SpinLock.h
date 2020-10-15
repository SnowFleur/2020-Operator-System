#pragma once
#include<mutex>
#include<atomic>
#include<Windows.h>


class CSpinLock{
private:
#ifdef _WIN64
    LONG         lockValue_;
#else
    int          lockValue_;
#endif
    bool        CAS(int* volatile addr, int  expected, int new_val);
    void        AcquireSpinLock(volatile LONG*)noexcept;
    void        ReleaseSpinLock(volatile LONG*)noexcept;
public:
    CSpinLock() = default;
    ~CSpinLock() = default;
    
    
    void        lock()noexcept;
    void        unlock()noexcept;
};


