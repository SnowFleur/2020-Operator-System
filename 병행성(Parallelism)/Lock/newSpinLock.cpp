#include<chrono>
#include<mutex>
#include<vector>
#include<atomic>
#include<thread>
#include<iostream>
#include<windows.h>

constexpr int          MAX_THREADS = 4;
constexpr unsigned int MAX_LOOP = 10000000;

using namespace std::chrono;
using Threads = std::vector<std::thread>;
using mutex = std::mutex;
void SumByLock();
void SumBySpinLock();

class SpinLock {
private:
    std::atomic_flag lck = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (lck.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        lck.clear(std::memory_order_release);
    }
};

LONG g_lSpinLock;

void AcquireSpinLock(volatile LONG* plCount)
{
    LONG    lOldCount;

lb_try:
    lOldCount = _InterlockedCompareExchange(plCount, 1, 0);

    if (lOldCount)
    {
        for (DWORD i = 0; i < 1024; i++)
        {
            YieldProcessor();   // __asm pause
        }
        goto lb_try;
    }

}
void ReleaseSpinLock(volatile LONG* plCount)
{
    _InterlockedDecrement(plCount);
}

volatile int            g_sum;
mutex                   g_mutex;
SpinLock                g_spinLock;

int main() {
    std::cout << "====number of Thread: " << MAX_THREADS << "=====\n\n\n";


#pragma region Mutex Lock
    {
        Threads     threads_;
        g_sum = 0;
        auto startTime = high_resolution_clock::now();
        for (int i = 0; i < MAX_THREADS; ++i) {
            threads_.emplace_back(SumByLock);
        }
        for (auto& thread : threads_)
            thread.join();
        auto EndTime = high_resolution_clock::now();

        auto elapsedTime = EndTime - startTime;
        std::cout << "Sum Result: " << g_sum << "\n";
        std::cout << "Elapsed Time For Lock: " <<
            duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "millsec\n";
    }
#pragma endregion

#pragma region Spin Lock
    {
        Threads     threads_;
        g_sum = 0;
        auto startTime = high_resolution_clock::now();
        for (int i = 0; i < MAX_THREADS; ++i) {
            threads_.emplace_back(SumBySpinLock);
        }
        for (auto& thread : threads_)
            thread.join();
        auto EndTime = high_resolution_clock::now();

        auto elapsedTime = EndTime - startTime;
        std::cout << "Sum Result: " << g_sum << "\n";
        std::cout << "Elapsed Time For Spin Lock: " <<
            duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "millsec\n\n";
    }
#pragma endregion 
}

void SumByLock() {
    for (unsigned int i = 0; i < MAX_LOOP / MAX_THREADS; ++i) {
        g_mutex.lock();
        g_sum += 1;
        g_mutex.unlock();
    }
}

void SumBySpinLock() {
    for (unsigned int i = 0; i < MAX_LOOP / MAX_THREADS; ++i) {
        //g_spinLock.lock();
        AcquireSpinLock(&g_lSpinLock);
        g_sum += 1;
        ReleaseSpinLock(&g_lSpinLock);
        //g_spinLock.unlock();
    }
}