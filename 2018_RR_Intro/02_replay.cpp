#include <memory>
#include <random>
#include <iostream>

bool choose()
{
    // ... some code
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 20);
    int rand = dist(rd);
    // ... some more code
    return (rand < 18);
}

int main()
{
    auto choice = choose();
    // .. some code
    if (choice)
    {
        std::cout << "totally normal, nothing to see...\n";
    }
    else
    {
        std::cout << "NANANANANANANANANANANANANANANANA BATMAN!\n";
    }
}
