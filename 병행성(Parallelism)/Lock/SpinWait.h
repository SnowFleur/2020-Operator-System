#pragma once
#include<Windows.h>



struct SpinWait {
    struct SYSINFO;
public:
    static  SYSINFO s_si;
    int     count_;
public:
    const int YIELD_THRESHOLD = 25;     // 스핀 수행 한계치
    const int MAX_SPIN_INTERVAL = 32;   // 최대 스핀 인터벌
    const int SLEEP_0_TIMES = 2;        // Sleep(0) 호출을 위한 빈도
    const int SLEEP_1_TIMES = 10;       // Sleep(1) 호출을 위한 빈도

 //YiedlProcesser 호출 여부를 판별하는 매크로
#define NextSpinWillYield (s_si.dwNumberOfProcessors==1 || count_>=YIELD_THRESHOLD)

                                        /*
    CPU 개수를 획등하기 위한 구조체 상속
    */
    struct SYSINFO : public SYSTEM_INFO {
        SYSINFO() { GetSystemInfo(this); }
    };

    SpinWait() {
        count_ = 0;
    }

    void SpinOnce() {
        if (NextSpinWillYield) {

            int yieldSoFar = (count_ >= YIELD_THRESHOLD ? count_ - YIELD_THRESHOLD : count_);

            if ((yieldSoFar % SLEEP_1_TIMES) == SLEEP_0_TIMES - 1)
                Sleep(0);
            else if ((yieldSoFar % SLEEP_1_TIMES) == SLEEP_1_TIMES - 1)
                Sleep(1);
            else
                SwitchToThread();
        }
        else {
            //다중 CPU일 경우에는 우선 스핀을 수행
            int nLoopCount = (int)(count_ * ((float)MAX_SPIN_INTERVAL / YIELD_THRESHOLD)) + 1;
            //count_ 값을 기준으로 적절한 스핀 횟수를 구해 루프를 돌면서 YieldProcessor()를 호출한다.
            while (nLoopCount-- > 0) {
                YieldProcessor();
            }
            //누적 스핀횟수를 보관하기 위해 count_ 필드 값 증가
            count_ = (count_ == INT_MAX ? YIELD_THRESHOLD : count_ + 1);
        }
    }

    void Rest() {
        count_ = 0;
    }

    int Count()const {
        return count_;
    }

};

SpinWait::SYSINFO SpinWait::s_si;
