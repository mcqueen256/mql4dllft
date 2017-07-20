#ifndef EXPORT_HPP
#define EXPORT_HPP

#include <iostream>
#include <fstream>
#include <functional>

#include "InstanceManager.hpp"
#include "BaseRobot.hpp"

#if defined(__BORLANDC__)
    typedef unsigned char uint8_t;
    typedef __int64 int64_t;
    typedef unsigned long uintptr_t;
#elif defined(_MSC_VER)
    typedef unsigned char uint8_t;
    typedef __int64 int64_t;
#else
    #include <stdint.h>
#endif

/* Macros ------------------------------------------------------------------- */
#ifdef NDEBUG
#define MT4_API __declspec(dllexport)
#else
#define MT4_API 
#endif

/* DLL Exported Functions --------------------------------------------------- */
extern "C" {
	/*
	 * Note: MQL calls are not available until after this function returns.
	 */
	MT4_API RobotReferenceType initialise(int param1);
	MT4_API void deinitialise(RobotReferenceType instance, const int reason);
	MT4_API void enableTrading(RobotReferenceType instance);
	MT4_API void disableTrading(RobotReferenceType instance);
	MT4_API int bar(RobotReferenceType instance, char* time, double open, double high, double low, double close, double volume);
	MT4_API int quote(RobotReferenceType instance, char* time, double open, double high, double low, double close, double volume);
}
#endif