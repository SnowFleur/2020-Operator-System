#include<iostream>
#include "SpinLock.h"


CSpinLock::CSpinLock() {
    InitializeCriticalSectionAndSpinCount(&cs_, 4000);
}

bool CSpinLock::CAS(int* addr, int  expected, int new_val) {
    return std::atomic_compare_exchange_strong(
        reinterpret_cast<std::atomic_int*>(addr), &expected, new_val);
}

void CSpinLock::lock()noexcept {
    //while (CAS(&lockValue_, 0, 1)==false);
    EnterCriticalSection(&cs_);
}

void CSpinLock::unlock()noexcept {
    //CAS(&lockValue_, 1, 0);
    LeaveCriticalSection(&cs_);
}
