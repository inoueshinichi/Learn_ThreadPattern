#include <iostream>
#include <string>

#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <functional>

//////////////////////////////////////
// 標準出力(std::cout)は排他制御すべし
//////////////////////////////////////
std::mutex stdout_mutex;

template <typename T>
void thread_safe_print(const T& value)
{
    // std::coutはスレッドセーフじゃないので排他制御するように
    std::lock_guard<std::mutex> locker(stdout_mutex);
    std::cout << value << std::endl;
}


int main(int argc, char** argv)
{
    /* std::mutexの使い方 */
    std::thread th1([]() {
        for (int i = 0; i < 100; ++i)
            thread_safe_print("Th1: " + std::to_string(i));
    });

    std::thread th2{[]() {
        for (int i = 0; i < 100; ++i)
            thread_safe_print("Th2: " + std::to_string(i));
    }};

    th1.join();
    th2.join();

    std::cout << "finish Main thread" << std::endl;
    return 0;
}