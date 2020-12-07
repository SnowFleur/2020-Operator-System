#pragma once
#include<Windows.h>



struct SpinWait {
    struct SYSINFO;
public:
    static  SYSINFO s_si;
    int     count_;
public:
    const int YIELD_THRESHOLD = 25;     // ���� ���� �Ѱ�ġ
    const int MAX_SPIN_INTERVAL = 32;   // �ִ� ���� ���͹�
    const int SLEEP_0_TIMES = 2;        // Sleep(0) ȣ���� ���� ��
    const int SLEEP_1_TIMES = 10;       // Sleep(1) ȣ���� ���� ��

 //YiedlProcesser ȣ�� ���θ� �Ǻ��ϴ� ��ũ��
#define NextSpinWillYield (s_si.dwNumberOfProcessors==1 || count_>=YIELD_THRESHOLD)

                                        /*
    CPU ������ ȹ���ϱ� ���� ����ü ���
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
            //���� CPU�� ��쿡�� �켱 ������ ����
            int nLoopCount = (int)(count_ * ((float)MAX_SPIN_INTERVAL / YIELD_THRESHOLD)) + 1;
            //count_ ���� �������� ������ ���� Ƚ���� ���� ������ ���鼭 YieldProcessor()�� ȣ���Ѵ�.
            while (nLoopCount-- > 0) {
                YieldProcessor();
            }
            //���� ����Ƚ���� �����ϱ� ���� count_ �ʵ� �� ����
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
