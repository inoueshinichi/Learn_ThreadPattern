#include <iostream>
#include <string>

#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <functional>

#include <windows.h>

class IncludedThreadClass
{
    /* クラスメンバのスレッド関数(std::thread) */
public:
    // スレッド本体
    void memberThread(int n)
    {
        for (int i = 0; i < n; i++)
		{
			printf("I'm Class Member-ThreadFunc: %d\r\n", i);
			std::this_thread::sleep_for(std::chrono::seconds(1)); // std::c++
																  /*if (i == 3)
																  {
																  printf("i==3, thus, thread retrun end...\r\n");
																  return;
																  }*/
		}
		std::cout << "Class Member-ThreadFunc end." << std::endl;
		return;
    }

    // 関数オブジェクトを返すラッパー
    std::function<void(int)> memberThreadCapsel()
    {
        // thisをキャプチャしたラムダ式でラッピング : this->memberThread(n)
        std::cout << "sizeof (std::function<void()>): " << sizeof(std::function<void(int)>) << std::endl; 
        return [this](int n) {
            this->memberThread(n);
        };
    }
};


int main(int argc, char** argv)
{
    IncludedThreadClass inc;
    std::thread worker(inc.memberThreadCapsel(), 5);
    std::cout << "Thread ID: " << worker.get_id() << std::endl;
    HANDLE threadNativeHandle = worker.native_handle();       // WindowsのNativeハンドルを取得
    std::cout << "Windows Native Thread HANDLE: " << threadNativeHandle << std::endl;

    for (int i = 0; i < 5; i++)
	{
		printf("I'm main: %d\r\n", i);
		Sleep(1000);
	}
	std::cout << "Waiting for thread end." << std::endl;

    worker.join();
    std::cout << "Finish Main" << std::endl;
    return 0;
}