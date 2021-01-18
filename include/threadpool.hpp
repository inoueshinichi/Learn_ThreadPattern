#ifndef THREAD_PATTERN_THREADPOOL_HPP
#define THREAD_PATTERN_THREADPOOL_HPP

#include "defs.hpp"

#include <iostream>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <functional>
#include <exception>
#include <stdexcept>

#include <vector>
#include <queue>

namespace Is
{
    using std::vector;
    using std::queue;
    using std::mutex;
    using std::unique_lock;
    using std::condition_variable;
    using std::thread;
    using std::packaged_task;
    using std::move;
    using std::future;
    using std::make_shared;
    using std::bind;
    using std::forward;
    using std::runtime_error;



#if (__cplusplus < 201704L)
#   define INVOKE_RESULT_TYPE(FUNC, ARGS) typename std::result_of<FUNC(ARGS...)>::type
#else
#   define INVOKE_RESULT_TYPE(FUNC, ARGS) typename std::invoke_result<FUNC, ARGS...>::type
#endif

    class ThreadPool
    {
        vector<thread> workers_;                  // スレッドプールに存在するワーカースレッド
        queue<packaged_task<void()>> queue_;      // タスクのキュー
        mutex mtx_;                               // キューのミューテックス
        condition_variable condition_;            // キューの条件変数
        bool stop_request_ {false};               // スレッドプールへのストップリクエスト

    public:
        /**
         * @brief Construct a new Thread Pool object
         * 
         * @param workers 
         */
        ThreadPool(size_t workers)
        {
            std::cout << "===== ThreadPool Constructor! =====" << std::endl;

            for (size_t i = 0; i < workers; ++i)
            {
                // スレッド内で処理される文脈
                auto done_on_thread = [this]() {
                    while (true)
                    {
                        // キューから受け取るタスク
                        packaged_task<void()> task;

                        // 排他領域
                        {
                            unique_lock<mutex> locker(mtx_);

                            // キューにタスクが入るまで待機
                            condition_.wait(locker, [this]() {
                                return stop_request_ || !queue_.empty(); // ストップリクエストON or キューにタスクが残っている場合、trueを出力
                            });

                            // ストップリクエストON and キューが空の場合、ThreadPoolを終了する
                            if (stop_request_ && queue_.empty())
                                return;

                            // タスクの受け取り
                            task = move(queue_.front());

                            // 中身の無い要素を削除
                            queue_.pop();

                            // タスク実行
                            task();
                        }
                    }
                };

                workers_.emplace_back(done_on_thread);
            } // for   
        }
     
        /**
         * @brief Destroy the Thread Pool object
         * 
         */
        virtual ~ThreadPool()
        {
            {
                unique_lock<mutex> locker(mtx_);
                stop_request_ = true;
            }

            condition_.notify_all();
            for (auto& worker : workers_)
            {
                worker.join();
            }

            std::cout << "===== ThreadPool Destructor! =====" << std::endl;
        }

        /**
         * @brief 
         * 
         * @tparam F 
         * @tparam Args 
         * @param func 
         * @param args 
         * @return future<INVOKE_RESULT_TYPE(F, Args)> 
         */
        template <typename F, typename... Args>
        auto enqueue(F&& func, Args&&... args) -> future<INVOKE_RESULT_TYPE(F, Args)>
        { // 引数付の関数オブジェクトor関数ポインタの戻り値の型を取得
            
            // キューにタスクを入れて、タスクの戻り値の型を取得する
            using return_type = INVOKE_RESULT_TYPE(F, Args);

            auto task = make_shared<packaged_task<return_type()>>(bind(forward<F>(func), forward<Args>(args)...));

            future<return_type> result = task->get_future();

            // 排他領域
            {
                unique_lock<mutex> locker_(mtx_);

                // ストップリクエストがONの場合、エンキューを停止＆例外出力
                if (stop_request_)
                {
                    throw runtime_error("Enqueue on stopped thread-pool.");
                }

                queue_.emplace([task]() {
                    (*task)();
                });
            }

            // ワーカースレッドのどれかに通知
            condition_.notify_one();

            return result;
        }
    };
}

/*
[std::queue]
(constructor)	コンストラクタ
empty			要素が空であるかを確認する
size			要素数を取得する
front			次の要素にアクセスする
back			最後の要素にアクセスする
push			要素を追加する
emplace			直接構築で要素を追加する
pop				次の要素を削除する
swap			他のstd::queueオブジェクトと値を入れ替える
*/

#endif // THREAD_PATTERN_THREADPOOL_HPP