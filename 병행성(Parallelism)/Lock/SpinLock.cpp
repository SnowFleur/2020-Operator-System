#include<iostream>
#include "SpinLock.h"


bool CSpinLock::CAS(int* volatile addr, int  expected, int new_val) {
    return std::atomic_compare_exchange_strong(
        reinterpret_cast<std::atomic_int* volatile>(addr), &expected, new_val);
}

void CSpinLock::lock()noexcept {
    while (CAS(&lockValue_, 0, 1) == false) {
        Sleep(1);
    }
}

void CSpinLock::unlock()noexcept {
    CAS(&lockValue_, 1, 0);
}

