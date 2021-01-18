cmake_minimum_required(VERSION 3.14.0)
project(thread_pattern LANGUAGES CXX VERSION 0.1.0)

# CMake Version
message("CMake Version: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}.${CMAKE_TWEAK_VERSION}")


###############################################################################
# 全ターゲットに影響を与えるオプション
###############################################################################
message(STATUS "========== [START] C++ All Target Options ==========")

# ========== Options ==========
option(BUILD_OPENMP "Enable/Disable OpenMP" ON)
option(BUILD_CPP_TEST "Enable/Disable Test" ON)
option(BUILD_SHARED_LIB "Enable/Disable DLL Libs" ON)

if (BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

# プラットフォーム毎のコンパイラオプション(マルチ・コンフィギュレーションに対応)
if (MSVC)
    message(STATUS "Compiler: MSVC in Windows")

    # Build Options
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # デフォルトの`/W[0-4]`を削除
    set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /GR /EHsc /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "/Ob0 /Od /MDd /Zi /RTC1") # /MDd or /Od が指定されている場合、#include <windows>内部で_DEBUGマクロが定義される
    set(CMAKE_CXX_FLAGS_RELEASE "/Ob2 /O2 /MD /DNDEBUG") # /DNDEBUGマクロはstatic_assert()で利用する
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /MD /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Od /MDd /Zi /RTC1")

    if(BUILD_OPENMP)
        set(CMAKE_CXX_FFAGS "${CMAKE_CXX_FFAGS} /openmp")
    endif()

    # ソースコードでUTF-8を使う(MSVC++)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
    # C++11以降を利用した場合にマクロ変数__cplusplusの値を利用中のC++バージョンに合わせる(つけないとC++98相当の値になる)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")


    # プリコンパイル済みヘッダーの使用(/Yu)を全体に設定
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yu")

elseif(APPLE)

message(STATUS "Compiler: Clang in macOS")    

    # Build Options(マルチコンフィギュレーション:visual studio対応)
    set(CMAKE_CXX_FLAGS "-O1")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

    # 警告
    string(APPEND CMAKE_CXX_FLAGS " -Wall")
    # string(APPEND CMAKE_CXX_FLAGS " -Werror")
    # string(APPEND CMAKE_CXX_FLAGS " -Weverything")
    # string(APPEND CMAKE_CXX_FLAGS " -Wno-c++98-compat")
    # string(APPEND CMAKE_CXX_FLAGS " -Wno-c++98-compat-pedanti")

    # ソースコードでUTF-8を使う(GCC++)
    add_compile_options($<$<CXX_COMPILER_ID:Clang>:/utf-8>)
    # C++11以降を利用した場合にマクロ変数__cplusplusの値を利用中のC++バージョンに合わせる(つけないとC++98相当の値になる)
    add_compile_options($<$<CXX_COMPILER_ID:Clang>:/Zc:__cplusplus>) 

else()
    message(STATUS "Compiler: GCC in Linux")

    # 詳細情報の表示
    set(CMAKE_VERBOSE_MAKEFILE ON)

    # Build Options(マルチコンフィギュレーション:visual studio対応)
    set(CMAKE_CXX_FLAGS "-O1")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

    # C++標準に準拠しない拡張機能を無効化
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --pedantic-errors")

    # https://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings/9862800#9862800
    string(APPEND CMAKE_CXX_FLAGS " -pedantic")
    string(APPEND CMAKE_CXX_FLAGS " -Wall")
    # string(APPEND CMAKE_CXX_FLAGS " -Wextra")
    # string(APPEND CMAKE_CXX_FLAGS " -Wcast-align")
    # string(APPEND CMAKE_CXX_FLAGS " -Wcast-qual")
    # string(APPEND CMAKE_CXX_FLAGS " -Wctor-dtor-privacy")
    # string(APPEND CMAKE_CXX_FLAGS " -Wdisabled-optimization")
    # string(APPEND CMAKE_CXX_FLAGS " -Wformat=2 ")
    # string(APPEND CMAKE_CXX_FLAGS " -Winit-self")
    # string(APPEND CMAKE_CXX_FLAGS " -Wlogical-op")
    # string(APPEND CMAKE_CXX_FLAGS " -Wmissing-declarations")
    # string(APPEND CMAKE_CXX_FLAGS " -Wmissing-include-dirs")
    # string(APPEND CMAKE_CXX_FLAGS " -Wnoexcept")
    # string(APPEND CMAKE_CXX_FLAGS " -Wold-style-cast")
    # string(APPEND CMAKE_CXX_FLAGS " -Woverloaded-virtual")
    # string(APPEND CMAKE_CXX_FLAGS " -Wredundant-decls")
    # string(APPEND CMAKE_CXX_FLAGS " -Wshadow")
    # string(APPEND CMAKE_CXX_FLAGS " -Wsign-conversion")
    # string(APPEND CMAKE_CXX_FLAGS " -Wsign-promo")
    # string(APPEND CMAKE_CXX_FLAGS " -Wstrict-null-sentinel")
    # string(APPEND CMAKE_CXX_FLAGS " -Wstrict-overflow=5")
    # string(APPEND CMAKE_CXX_FLAGS " -Wswitch-default")
    # string(APPEND CMAKE_CXX_FLAGS " -Wundef")
    # string(APPEND CMAKE_CXX_FLAGS " -Werror")
    # string(APPEND CMAKE_CXX_FLAGS " -Wno-unused")

    # ソースコードでUTF-8を使う(GCC++)
    add_compile_options($<$<CXX_COMPILER_ID:GCC>:/utf-8>)
    # C++11以降を利用した場合にマクロ変数__cplusplusの値を利用中のC++バージョンに合わせる(つけないとC++98相当の値になる)
    add_compile_options($<$<CXX_COMPILER_ID:GCC>:/Zc:__cplusplus>)

endif()

message("---------- Compiler Options for Multi Configuration ----------")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

message(STATUS "========== [END] C++ All Target Options ==========")

###############################################################################
# C++ core
###############################################################################
message(STATUS "========== [START] C++ Build Core ==========")

# ========== Library File ==========
add_subdirectory(lib)

# ========== Executable File ==========
add_subdirectory(thread_pool)


message(STATUS "========== [END] C++ Build Core ==========")

###############################################################################
# C++ Testing
###############################################################################
message(STATUS "========== [START] C++ Build Test ==========")
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

message(STATUS "========== [END] C++ Build Test ==========")