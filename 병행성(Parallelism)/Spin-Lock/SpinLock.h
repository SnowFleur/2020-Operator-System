#pragma once
#include<chrono>
#include<mutex>
#include<vector>
#include<atomic>
#include<thread>


constexpr int MAX_THREADS   = 4;
constexpr int MAX_LOOP      = 10000000;

using namespace std::chrono;
using Threads   = std::vector<std::thread>;
using mutex     = std::mutex; 

class CSpinLock{
private:
    mutex           mutex_;
    int             casValue_=0;
    volatile int    g_sum=0;
    std::atomic_flag lock = ATOMIC_FLAG_INIT;

    void        SpinLock()noexcept;
    void        SpinUnLock()noexcept;
    bool        CAS( int*  addr, int  expected, int new_val);
    void        SumBySpinLock();
    void        SumByLock();
public:
    CSpinLock() = default;
    ~CSpinLock() = default;
    
    void        Run();
};
