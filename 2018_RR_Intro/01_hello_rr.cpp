#include <iostream>
#include <chrono>
#include <date/date.h>

int main()
{
    using namespace date;
    auto now = std::chrono::system_clock::now();
    std::cout << now << "\n";
}
