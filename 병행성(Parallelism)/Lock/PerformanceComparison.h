#pragma once

#include<chrono>
#include<mutex>
#include<vector>
#include<thread>

#include"SpinLock.h"
#include"SRWLock.h"

constexpr int          MAX_THREADS          = 8;
constexpr unsigned int MAX_LOOP             = 10000000;

//Lock
constexpr int          MUTEX                = 1;
constexpr int          SPIN_LOCK_WITH_CAS   = 2;
constexpr int          SRW_LOCK             = 3;

using namespace std::chrono;
using Threads   = std::vector<std::thread>;
using mutex     = std::mutex;

class CPerformanceComparison {
private:
    
    volatile int    sum_;
    
    mutex           mutex_;
    CSpinLock       spinLock_;
    CSrwLock        srwLock_;

    void            SumBySPinLock()noexcept;
    void            SumByMutex()noexcept;
    void            SumBySRWLock()noexcept;

public:
    CPerformanceComparison() = default;
    ~CPerformanceComparison() = default;
    
    //Delete Copy, Move operator
    CPerformanceComparison(const CPerformanceComparison&)= delete;
    CPerformanceComparison(CPerformanceComparison&&) = delete;
    CPerformanceComparison& operator=(const CPerformanceComparison&) = delete;
    CPerformanceComparison&& operator=( CPerformanceComparison&&) = delete;
    
    void            Run();
};