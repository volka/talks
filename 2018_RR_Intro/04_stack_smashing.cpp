#include <iostream>
#include <memory>
#include <cstring>

const char* input = "abcdefghijklmnopqrstuvwxyz";

int main()
{
    int x = 5;
    std::cout << "foo\n";

    char test[4];
    std::memcpy(test, input, 8);
    
    std::cout << "bar\n";

    std::cout << x << "\n";
}
