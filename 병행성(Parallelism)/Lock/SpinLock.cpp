#include<iostream>
#include"SpinLock.h"
#include"SpinWait.h"

CSpinLock::CSpinLock() :state_(LOCK_AVAIL) {}

DWORD CSpinLock::CAS(DWORD volatile* target, int  cmp, int new_value) {

    std::atomic_compare_exchange_strong(
        reinterpret_cast<std::atomic_int volatile*>(target), &cmp, new_value);

    return *target;
}

void CSpinLock::lock()noexcept {

    DWORD   dwCurThrId = GetCurrentThreadId();

    if (dwCurThrId == state_) {
        //호출 스레드가 스핀락을 소유한 상태라면 예외를 던진다.
        throw HRESULT_FROM_WIN32(ERROR_INVALID_OWNER);
    }

    //스핀락을 가지고 있는 스레드가 이미 있을경우 스핀락을 얻기 위해 스핀락 시도
    //if (InterlockedCompareExchange(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL) {
    if (CAS(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL) {

        //스핀대기 구조체
        SpinWait sw;

        do {
            do {
                sw.SpinOnce();
            } while (state_ != LOCK_AVAIL);
            // } while (InterlockedCompareExchange(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL);
        } while (CAS(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL);
    }
}


void CSpinLock::unlock()noexcept {
    DWORD   dwCurThrId = GetCurrentThreadId();

    if (dwCurThrId != state_) {
        //호출 스레드가 스핀락을 소유한 상태라면 예외를 던진다.
        throw HRESULT_FROM_WIN32(ERROR_INVALID_OWNER);
    }

#ifdef WIN32
    InterlockedExchange(&state_, LOCK_AVAIL);
#else
    InterlockedExchange64(&state_, LOCK_AVAIL);
#endif // WIN32

}

