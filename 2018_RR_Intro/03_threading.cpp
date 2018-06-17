#include <iostream>
#include <atomic>
#include <vector>
#include <memory>
#include <thread>

using namespace std;

static atomic<int> val{0};

void write_val(int i)
{
    val.store(i);
}

void exit_marker() {}

int main()
{
    vector<thread> threads;
    std::atomic<bool> running{true};
    
    for (int i = 0; i < 4; ++i)
    {
        threads.emplace_back([i, &running] {
            do
            {
                write_val(i);
                this_thread::yield();
            }
            while (running.load());
            write_val(i);
            std::cout << val.load() << "\n";

        });
    }

    this_thread::yield();
    running.store(false);
    for (auto& t: threads)
    {
        t.join();
    }
    std::cout << val.load() << "\n";
}
