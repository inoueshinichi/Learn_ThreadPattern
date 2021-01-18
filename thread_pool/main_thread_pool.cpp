#include "threadpool.hpp"

#include <vector>
#include <future>
#include <chrono>
#include <iostream>

int main(int argc, char** argv)
{
    using namespace std;
    using namespace Is;
    ThreadPool thread_pool(4);
    int task_num = 8;

    vector<future<int>> result_array;
    for (int i = 0; i < task_num; ++i)
    {
        result_array.emplace_back(thread_pool.enqueue([i]() {
            cout << "Hello " << i << endl;
            this_thread::sleep_for(chrono::seconds(1));
            cout << "World " << i << endl;
            return i*i;
        }));
    }

    cout << "Wait future result on Main Thread." << endl;
    for (auto& result : result_array)
    {
        result.wait();
        cout << result.get() << " ";
    }
    cout << endl;

    cout << "[END] Main Thread." << endl;

    cout << "__cplusplus=" << __cplusplus << endl;
    return 0;
}