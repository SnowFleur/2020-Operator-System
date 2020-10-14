#include<memory>
#include"PerformanceComparison.h"

int main() {
    std::unique_ptr<CPerformanceComparison>uPtrTestCode= std::make_unique<CPerformanceComparison>();
    uPtrTestCode->Run();
}