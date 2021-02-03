#include "threadpool.hpp"

#include <vector>
#include <future>
#include <chrono>
#include <iostream>

int main(int argc, char** argv)
{
    using namespace Is;
    ThreadPool thread_pool(4);
    int task_num = 8;

    vector<future<int>> result_array;
    for (int i = 0; i < task_num; ++i)
    {
        result_array.emplace_back(thread_pool.enqueue([i]() {
            std::cout << "Hello " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "World " << i << std::endl;
            return i*i;
        }));
    }

    std::cout << "Wait future result on Main Thread." << std::endl;
    for (auto& result : result_array)
    {
        std::cout << "waiting future in Main for loop." << std::endl;
        result.wait();
        std::cout << result.get() << " ";
    }
    std::cout << std::endl;

    std::cout << "[END] Main Thread." << std::endl;

    std::cout << "__cplusplus=" << __cplusplus << std::endl;
    return 0;
}