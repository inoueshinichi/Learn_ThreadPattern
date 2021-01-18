cmake_minimum_required(VERSION 3.14.0)
message(STATUS "========== [START] thread_pool ==========")

add_executable(thread_pool_exe
    main_thread_pool.cpp
)

target_link_libraries(thread_pool_exe PRIVATE
    thread_pattern
)

message(STATUS "========== [END] thread_pool ==========")