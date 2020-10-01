#include<iostream>
#include "SpinLock.h"



bool CSpinLock::SpinLock(std::atomic_int* addr, int  expected, int new_val){
    return std::atomic_compare_exchange_strong(addr, &expected, new_val);
}

void CSpinLock::Sum() {

    for (int i = 0; i < MAX_LOOP; ++i) {
        SpinLock(&flag, 0, 1);
        g_sum += 1;
    }
}

void CSpinLock::Run() {

    for (int i = 0; i < MAX_THREADS; ++i) {
        threads_.emplace_back(std::thread{Sum});
    }


    {
    auto startTime = high_resolution_clock::now();
    sum();
    auto EndTime = high_resolution_clock::now();
    auto elapsedTime = EndTime - startTime;
    std::cout << "Elapsed Time For SpinLock: " <<(double)elapsedTime.count() << "sec\n";
    }


    {
        auto startTime = high_resolution_clock::now();
        sum();
        auto EndTime = high_resolution_clock::now();
        auto elapsedTime = EndTime - startTime;
        std::cout << "Elapsed Time For Lock: " << (double)elapsedTime.count() << "sec\n";
    }

    for (auto& thread : threads_) {
        thread.join();
    }

}