#pragma once

#ifndef ASSERT_H
#define ASSERT_H

#include <stdexcept>
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
    DEBUG_ERROR(true, std::string("Assertion failed: " + std::string(message)).c_str()); \
    __debugbreak();}
#endif
#endif//Assert_h
