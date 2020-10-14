#include<iostream>
#include "SpinLock.h"


bool CSpinLock::CAS(int* addr, int  expected, int new_val) {
    return std::atomic_compare_exchange_strong(
        reinterpret_cast<std::atomic_int*>(addr), &expected, new_val);
}

void CSpinLock::lock()noexcept {
    while (CAS(&lockValue_, 0, 1)==false);
}



void CSpinLock::unlock()noexcept {
    CAS(&lockValue_, 1, 0);
}

#pragma region  https://megayuchi.com/2017/06/25/srwlock-%EB%B9%A0%EB%A5%B8-%EC%84%B1%EB%8A%A5%EC%9D%98-%EB%B9%84%EA%B2%B0/
void CSpinLock::AcquireSpinLock(volatile LONG* plCount)noexcept {
    LONG    lOldCount;
lb_try:
    lOldCount = _InterlockedCompareExchange(plCount, 1, 0);

    if (lOldCount)
    {
        for (DWORD i = 0; i < 1024; i++)
        {
            YieldProcessor();   // __asm pause
        }
        goto lb_try;
    }
}

void CSpinLock::ReleaseSpinLock(volatile LONG* plCount)noexcept {
    _InterlockedDecrement(plCount);
}
#pragma endregion


