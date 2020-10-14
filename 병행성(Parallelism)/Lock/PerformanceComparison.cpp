#include<iostream>
#include"PerformanceComparison.h"




void CPerformanceComparison::SumBySPinLock()noexcept {
    for (unsigned int i = 0; i < MAX_LOOP; ++i) {
        std::lock_guard<CSpinLock> lg(spinLock_);
        sum_ += 1;
    }
}

void CPerformanceComparison::SumByMutex()noexcept {
    for (unsigned int i = 0; i < MAX_LOOP; ++i) {
        std::lock_guard<mutex> lg(mutex_);
        sum_ += 1;
    }
}

void CPerformanceComparison::SumBySRWLock()noexcept {
    for (unsigned int i = 0; i < MAX_LOOP; ++i) {
        std::lock_guard<CSrwLock> lg(srwLock_);
        sum_ += 1;
    }
}



void CPerformanceComparison::Run() {

    std::cout << "====number of Theads: " << MAX_THREADS << "=====\n\n\n";

#pragma region Mutex Lock
    {
        Threads     threads_;
        sum_ = 0;
        auto startTime = high_resolution_clock::now();
        for (int i = 0; i < MAX_THREADS; ++i) {
            threads_.emplace_back(std::thread(&CPerformanceComparison::SumByMutex, this));
        }
        for (auto& thread : threads_)
            thread.join();
        auto EndTime = high_resolution_clock::now();

        auto elapsedTime = EndTime - startTime;
        std::cout << "Sum Result: " << sum_ << "\n";
        std::cout << "Elapsed Time For Mutex Lock: " <<
            duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "millsec\n\n";
    }
#pragma endregion

#pragma region Spin Lock With CAS
    {
        Threads     threads_;
        sum_ = 0;
        auto startTime = high_resolution_clock::now();
        for (int i = 0; i < MAX_THREADS; ++i) {
            threads_.emplace_back(std::thread(&CPerformanceComparison::SumBySPinLock, this));
        }
        for (auto& thread : threads_)
            thread.join();
        auto EndTime = high_resolution_clock::now();

        auto elapsedTime = EndTime - startTime;
        std::cout << "Sum Result: " << sum_ << "\n";
        std::cout << "Elapsed Time For Spin Lock With CAS: " <<
            duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "millsec\n\n";
    }
#pragma endregion 

#pragma region SRW Lock
    {
        Threads     threads_;
        sum_ = 0;
        auto startTime = high_resolution_clock::now();
        for (int i = 0; i < MAX_THREADS; ++i) {
            threads_.emplace_back(std::thread(&CPerformanceComparison::SumBySRWLock, this));
        }
        for (auto& thread : threads_)
            thread.join();
        auto EndTime = high_resolution_clock::now();

        auto elapsedTime = EndTime - startTime;
        std::cout << "Sum Result: " << sum_ << "\n";
        std::cout << "Elapsed Time For SRW Lock: " <<
            duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "millsec\n\n";
    }
#pragma endregion
}