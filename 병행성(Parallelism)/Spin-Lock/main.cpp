#include<memory>
#include"SpinLock.h"

int main() {
    std::unique_ptr<CSpinLock>uPtrSpinLock = std::make_unique <CSpinLock>();
    uPtrSpinLock->Run();
}