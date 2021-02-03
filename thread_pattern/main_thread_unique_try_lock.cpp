#include <iostream>
#include <string>

#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <functional>

#include <vector>

struct Data
{
    int data;
    std::mutex mtx;
};

void worker_unique_try_lock(Data& d)
{
    /* unique_lockによるスレッド2 */
	// unique_lockはtry_lockができる
    {
        std::unique_lock<std::mutex> locker(d.mtx, std::defer_lock);
        for (;;)
        {
            // ロックの取得を試みる
            if (locker.try_lock())
            {
                int n = d.data;
                std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep 1sec
                n += 1;
                d.data = n;
                std::cout << "worker thread -> d.data = " << d.data << std::endl;
                return;
            }
            // リソースの取得に失敗した場合、CPUにリソースを解放
            else
            {
                std::this_thread::yield();
            }
        }
    } // アンロック
}

int main(int argc, char** argv)
{
    std::vector<std::thread> workers(4);
    Data d;
    d.data = 0;

    for (auto& worker : workers)
        worker = std::thread(worker_unique_try_lock, std::ref(d));
    
    for (auto& worker : workers)
        worker.join();

    std::cout << "Main thread -> d.data = " << d.data << std::endl;
    return 0;
}