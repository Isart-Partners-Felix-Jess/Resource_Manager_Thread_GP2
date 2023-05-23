#pragma once

#ifndef ASSERT_H
#define ASSERT_H

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
#endif//Assert_h
