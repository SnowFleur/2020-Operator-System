#pragma once
#include<chrono>
#include<mutex>
#include<vector>
#include<atomic>
#include<thread>


constexpr int MAX_THREADS   = 6;
constexpr int MAX_LOOP      = 100000000;

using namespace std::chrono;
using Threads = std::vector<std::thread>;

class CSpinLock{
private:
    Threads     threads_;
    bool        SpinLock(std::atomic_int* addr, int  expected, int new_val);

public:
    CSpinLock() = default;
    ~CSpinLock() = default;

    void        Run();
    void        Sum();
};

static int g_sum{ 0 };
std::atomic_int flag{};