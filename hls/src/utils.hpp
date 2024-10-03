/* Copyright 2019 Columbia University, SLD Group */

#include "systemc.h"
#include "tlm.h"
using namespace tlm;

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

/* Macros to report information to the users. */
#define STRING_(x) #x
#define STRING(x) STRING_(x)
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)

#include <sstream>

template <typename T> std::string __to_str(T t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <typename T1, typename T2> std::string __to_str(T1 t1, T2 t2) {
    std::stringstream ss;
    ss << t1 << t2;
    return ss.str();
}

template <typename T1, typename T2, typename T3> std::string __to_str(T1 t1, T2 t2, T3 t3) {
    std::stringstream ss;
    ss << t1 << t2 << t3;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4> std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6 << t7;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8,
          typename T9>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8,
          typename T9, typename T10>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8,
          typename T9, typename T10, typename T11>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 << t11;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8,
          typename T9, typename T10, typename T11, typename T12>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 << t11 << t12;
    return ss.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8,
          typename T9, typename T10, typename T11, typename T12, typename T13>
std::string __to_str(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12,
                     T13 t13) {
    std::stringstream ss;
    ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 << t11 << t12 << t13;
    return ss.str();
}

#define STR(...) __to_str(__VA_ARGS__).c_str()

#define REPORT_INFO(...)                                                                                               \
    {                                                                                                                  \
        printf("Info: %s: ", sc_object::basename());                                                                   \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n\n");                                                                                                \
    }

#define REPORT_ERROR(...)                                                                                              \
    {                                                                                                                  \
        printf("Error: %s: ", sc_object::basename());                                                                  \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n\n");                                                                                                \
    }

#define REPORT_TIME(time, ...)                                                                                         \
    {                                                                                                                  \
        std::stringstream _ss;                                                                                         \
        _ss << time;                                                                                                   \
        std::string _s = _ss.str();                                                                                    \
        const char *_time = _s.c_str();                                                                                \
        printf("Info: %s: ", sc_object::basename());                                                                   \
        printf("@%s ", _time);                                                                                         \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n\n");                                                                                                \
    }

#if defined(DEBUG)

/* Print the following info only if debug is selected. */

#define REPORT_DEBUG(...)                                                                                              \
    {                                                                                                                  \
        printf("Debug: %s: ", sc_object::basename());                                                                  \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n\n");                                                                                                \
    }

#else /* !DEBUG */

#define REPORT_DEBUG(...)

#endif /* DEBUG */

#endif /* __UTILS_HPP__ */
