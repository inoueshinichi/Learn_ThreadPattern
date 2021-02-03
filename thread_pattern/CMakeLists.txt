cmake_minimum_required(VERSION 3.14.0)
message(STATUS "========== [START] thread_pattern ==========")

add_executable(thread_pool
    main_thread_pool.cpp
)
target_link_libraries(thread_pool PRIVATE
    threadlib
)

add_executable(thread_async
    main_thread_async.cpp
)

add_executable(thread_safe_ostream
    main_thread_safe_ostream.cpp
)

add_executable(thread_lock_guard
    main_thread_lock_guard.cpp
)

add_executable(thread_unique_lock1
    main_thread_unique_lock1.cpp
)

add_executable(thread_unique_try_lock
    main_thread_unique_try_lock.cpp
)

add_executable(thread_recursive_mutex
    main_thread_recursive_mutex.cpp
)


message(STATUS "========== [END] thread_pattern ==========")