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

void worker_lock_guard(Data& d)
{
    /* lock_guardによるスレッド */
	// lock_guardはコンストラクタでロックする(RAII)
    {
        std::lock_guard<std::mutex> locker(d.mtx); // ロック
        int n = d.data;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep 1sec
        n += 1;
        d.data = n;
        std::cout << "worker thread -> d.data: " << d.data << std::endl;
    } // アンロック
}


int main(int argc, char** argv)
{
    /* lock_guardによる排他制御 */
    std::vector<std::thread> workers(4);
    Data d;
    d.data = 0;

    for (auto& worker : workers)
    {
        worker = std::thread(worker_lock_guard, std::ref(d)); // 参照型(Data&)を渡すときはstd::ref()が必要
    }

    for (auto& worker : workers)
        worker.join();

    std::cout << "Main thread -> d.data = " << d.data << std::endl;
    return 0;
}