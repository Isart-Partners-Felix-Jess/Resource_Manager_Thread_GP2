#pragma once
#include <iostream>
#include <intrin.h>
#include <Log.hpp>

#ifdef NDEBUG
#define Assert(condition, message) \
if (!(condition)) { \
throw std::runtime_error(message); \
}
#else
#define Assert(condition, message) \
if (!(condition)) { \
    std::cerr << "Assertion failed: " << message << std::endl; \
    __debugbreak();}
#endif
