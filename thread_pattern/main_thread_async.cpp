#include <iostream>
#include <string>

#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <functional>

/////////////////
/*  std::async */
/////////////////
/* std::async スレッドの戻り値を非同期で取得したい場合 */

int main(int argc, char** argv)
{
    // 第一引数
	// std::launch::async   -> 別スレッドで実行
	// std::launch::deffred -> 遅延評価
	// std::launch::async | std::launch::deffred 上記のいずれか(実装依存)
	// 指定なし	　　　　　　　 -> 両方指定した場合と同様

    // 非同期なスレッドを1つ作成し，適当な計算を担当してもらう
    auto result_async = std::async(std::launch::async, []() -> int {
        int sum = 0;
        for (int i = 0; i < 100; ++i)
        {
            sum += i;
        }
        return sum;
    });

    // メインスレッドでは適当な処理
    for (int i = 0; i < 50; ++i)
    {
        std::cout << std::to_string(i) << std::endl;
    }

    // 非同期の戻り値を出力(戻り値がまだ戻ってきていない場合，どうなるかは不明。調査必要)
    std::printf("Main future get -> %d\r\n", result_async.get());

    std::cout << "finish Main thread" << std::endl;
    return 0;
}