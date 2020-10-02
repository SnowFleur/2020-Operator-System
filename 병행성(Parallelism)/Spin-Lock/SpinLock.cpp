#include<iostream>
#include "SpinLock.h"



bool CSpinLock::CAS( int*  addr, int  expected, int new_val){
    return std::atomic_compare_exchange_strong(
        reinterpret_cast<std::atomic_int*>(addr), &expected, new_val);
}

void CSpinLock::SpinLock()noexcept {
    while (lock.test_and_set(std::memory_order_acquire));
    //while (!CAS(&casValue_, 0, 1));
}

void CSpinLock::SpinUnLock()noexcept {
    lock.clear(std::memory_order_release);
    //CAS(&casValue_, 1, 0);
}

void CSpinLock::SumByLock() {
    for (int i = 0; i < MAX_LOOP; ++i) {
        mutex_.lock();
        g_sum += 1;
        mutex_.unlock();
    }
}

void CSpinLock::SumBySpinLock() {
    for (int i = 0; i < MAX_LOOP; ++i) {
        SpinLock();
        g_sum += 1;
        SpinUnLock();
    }
}

void CSpinLock::Run() {

    std::cout << "====Number Theads: " << MAX_THREADS << "=====\n\n\n";
#pragma region Spin Lock
    {
        Threads     threads_;
        auto startTime = high_resolution_clock::now();
        for (int i = 0; i < MAX_THREADS; ++i) {
            threads_.emplace_back(std::thread(&CSpinLock::SumBySpinLock, this));
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

#pragma region Mutex Lock
    {
        Threads     threads_;
        g_sum = 0;
        auto startTime = high_resolution_clock::now();
        for (int i = 0; i < MAX_THREADS; ++i) {
            threads_.emplace_back(std::thread(&CSpinLock::SumByLock, this));
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
}