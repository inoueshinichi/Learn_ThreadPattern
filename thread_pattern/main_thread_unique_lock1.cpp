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

void worker_unique_lock1(Data& d)
{
    /* unique_lockによるスレッド1 */
	// unique_lockはコンストラクタ以外でもロックができる
    {
        std::unique_lock<std::mutex> locker(d.mtx, std::defer_lock);
        locker.lock();
        int n = d.data;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep 1sec
        n += 1;
        d.data = n;
        std::cout << "worker thread -> d.data = " << d.data << std::endl;
    } // アンロック
}


int main(int argc, char** argv)
{
    /* unique_lockによる排他制御1 */
    std::vector<std::thread> workers(4);
    Data d;
    d.data = 0;

    for (auto& worker : workers)
        worker = std::thread(worker_unique_lock1, std::ref(d));
    
    for (auto& worker : workers)
        worker.join();

    std::cout << "Main thread -> d.data = " << d.data << std::endl;

    return 0;
}