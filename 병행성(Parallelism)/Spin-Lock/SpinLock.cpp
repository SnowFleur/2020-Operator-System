#include<iostream>
#include "SpinLock.h"



bool CSpinLock::CAS(std::atomic_int* addr, int  expected, int new_val){
    return std::atomic_compare_exchange_strong(addr, &expected, new_val);
}

void CSpinLock::SpinLock() {
    while (CAS(&flag, 0, 1) == 1);
}

void CSpinLock::SpinUnLock() {
    CAS(&flag, 1, 0);
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

  

    //Spin Lock
  /*  {
        for (int i = 0; i < MAX_THREADS; ++i) {
            std::thread thread(&CSpinLock::SumBySpinLock, this);
        }
        auto startTime = high_resolution_clock::now();
        auto EndTime = high_resolution_clock::now();
        auto elapsedTime = EndTime - startTime;
        std::cout << "Sum Result: " << g_sum << "\n";
            std::cout << "Elapsed Time For Lock: " << 
            duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "sec\n";
    }*/


    //Lock
    {
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
            duration_cast<std::chrono::seconds>(elapsedTime).count() << "sec\n";
    }


}