#include <iostream>
#include <string>

#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <functional>

//////////////////////////////////////////////////////////////////////////////
// クラスのpublicなメンバ変数のネストした場合の排他制御はstd::recursive_mutexが便利
//////////////////////////////////////////////////////////////////////////////
class SomeClass
{
    // 任意のスレッドからnew/delete/全てのメンバ関数の呼び出しが可能なスレッドセーフ・クラス

    inline std::unique_lock<std::recursive_mutex> recursive_locker()
    {
        // std::unique_lock<std::recursive_mutex>クラスのファクトリ関数

        // std::unique_lockは、std::lock_guardの機能拡張版
		// シンプルな機能しか提供しないlock_guardクラスとの違いとして、以下の拡張機能を持つ：
		//
		//	1) コンストラクタでロックを取得せず、あとからロックを取得できる(defer_lock)
		//	2) コンストラクタでのロック取得に、lock()ではなくtry_lock()を使用できる(try_to_lock)
		//	3) ミューテックスの所有権を移動・交換(swap)・放棄(release)できる
		//	4) 任意のタイミングで所有ミューテックスのロック操作を呼び出せる
		//	5) 条件変数std::condition_variableオブジェクトと組み合わせて利用できるのは、std::unique_lock<std::mutex>型のオブジェクトに限定されている。
		return std::unique_lock<std::recursive_mutex>(rmtx_);
    }

    inline std::unique_lock<std::mutex> locker()
    {
        // std::unique_lock<std::mutex>クラスのファクトリ関数
		return std::unique_lock<std::mutex>(mtx_); // 比較用
    }

public:
	SomeClass()
		: cnt_(0)
		, val_(0)
	{}

    ~SomeClass() {}

    int inc_cnt()
    {
        std::unique_lock<std::recursive_mutex> locker = recursive_locker();
        // std::unique_lock<std::mutex> locker = locker(); // 比較用
        ++cnt_;
        printf("Value: %d, Count: %d \r\n", val_, cnt_);
        return cnt_;
    }

    int get_value()
    {
        std::unique_lock<std::recursive_mutex> locker = recursive_locker();
        inc_cnt();
        return val_;
    }

    void set_value(int v)
    {
        std::unique_lock<std::recursive_mutex> locker = recursive_locker();
        inc_cnt();
        val_ = v;
    }

private:
    int cnt_;
    int val_;

    // std::mutexと異なり，同一スレッドからの再帰的なロック取得を無視する。
	// std::mutexの場合，同一スレッドからの再帰的なロックが発生するとデッドロックに陥る。
	// 大抵は，実行時エラーでプログラムがAbortする.
	// std::recursive_mutexは, std::mutexよりオーバーヘッドがある
    std::recursive_mutex rmtx_;
    std::mutex mtx_;
};


int main(int argc, char** argv)
{
    /* std::recursive_mutexの使い方 */
    SomeClass someObj;

    std::thread worker([&someObj]() {
        someObj.set_value(100);
    });

    worker.join();
    std::cout << "Finish Main" << std::endl;
    return 0;
}