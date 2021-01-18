#ifndef THREAD_PATTERN_DEFS_HPP
#define THREAD_PATTERN_DEFS_HPP

#if defined(_MSC_VER)
#   if defined(thread_pattern_EXPORTS) || defined(thread_pattern_dbg_EXPORTS)
#       define TP_API __declspec(dllexport)
#   else
#       define TP_API __declspec(dllimport)
#   endif
#else
#   define TP_API
#endif

#endif // THREAD_PATTERN_DEFS_HPP