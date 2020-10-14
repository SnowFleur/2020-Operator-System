#pragma once
#include<mutex>
#include<atomic>
#include<Windows.h>


class CSrwLock {
private:
    SRWLOCK srwlock_;
public:
    void    lock()noexcept;
    void    unlock()noexcept;
    CSrwLock();
    ~CSrwLock() = default;
};