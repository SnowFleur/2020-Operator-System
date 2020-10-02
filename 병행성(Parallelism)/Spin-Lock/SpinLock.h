#pragma once
#include<chrono>
#include<mutex>
#include<vector>
#include<atomic>
#include<thread>


constexpr int MAX_THREADS   = 6;
constexpr int MAX_LOOP      = 50000000;

using namespace std::chrono;
using Threads = std::vector<std::thread>;
using mutex = std::mutex;
class CSpinLock{
private:
    mutex       mutex_;
    Threads     threads_;
    void        SpinLock();
    void        SpinUnLock();
    bool        CAS(std::atomic_int* addr, int  expected, int new_val);
    
    void        SumBySpinLock();
    void        SumByLock();
public:
    CSpinLock() = default;
    ~CSpinLock() = default;
    
    void        Run();
};

static volatile int g_sum{ 0 };
static std::atomic_int flag{};