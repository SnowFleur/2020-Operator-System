#include<iostream>
#include"SpinLock.h"
#include"SpinWait.h"

CSpinLock::CSpinLock() :state_(LOCK_AVAIL) {}

DWORD CSpinLock::CAS(volatile DWORD*  target, int  new_value, int cmp) {
    DWORD originValue = *target;
    std::atomic_compare_exchange_strong(
        reinterpret_cast<volatile std::atomic_int* >(target), &cmp, new_value);
    return originValue;
}


void CSpinLock::lock()noexcept {

    DWORD   dwCurThrId = GetCurrentThreadId();

    if (dwCurThrId == state_) {
        //ȣ�� �����尡 ���ɶ��� ������ ���¶�� ���ܸ� ������.
        throw HRESULT_FROM_WIN32(ERROR_INVALID_OWNER);
    }

    //���ɶ��� ������ �ִ� �����尡 �̹� ������� ���ɶ��� ��� ���� ���ɶ� �õ�
    if (InterlockedCompareExchange(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL) {
        //if (CAS(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL) {

            //���ɴ�� ����ü
        SpinWait sw;

        do {
            do {
                sw.SpinOnce();
            } while (state_ != 0);
            //ȣ���� �����尡 ���ɶ��� ������ ������ ������ state�� �ڱ� ID�� ���� ������ �� ���� �� �����ݺ�
        } while (InterlockedCompareExchange(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL);
        //} while (CAS(&state_, dwCurThrId, LOCK_AVAIL) != LOCK_AVAIL);
    }
}


void CSpinLock::unlock()noexcept {
    DWORD   dwCurThrId = GetCurrentThreadId();

    if (dwCurThrId != state_) {
        //ȣ�� �����尡 ���ɶ��� ������ ���¶�� ���ܸ� ������.
        throw HRESULT_FROM_WIN32(ERROR_INVALID_OWNER);
    }

#ifdef _WIN32
    InterlockedExchange(&state_, LOCK_AVAIL);
#else
    InterlockedExchange64(&state_, LOCK_AVAIL);
#endif // WIN32

}

