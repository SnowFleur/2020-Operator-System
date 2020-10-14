#include"SRWLock.h"


CSrwLock::CSrwLock() {
    //Initialization SRWLock
    InitializeSRWLock(&srwlock_);
}

void CSrwLock::lock()noexcept {
    AcquireSRWLockExclusive(&srwlock_);
}

void CSrwLock::unlock()noexcept {
    ReleaseSRWLockExclusive(&srwlock_);
}


