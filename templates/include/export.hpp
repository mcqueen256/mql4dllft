#ifndef EXPORT_HPP
#define EXPORT_HPP

#include "RobotReferenceType.hpp"

/* Macros ------------------------------------------------------------------- */
#ifdef PRODUCTION
#define MT4_API __declspec(dllexport)
#else
#define MT4_API 
#endif

/* DLL Exported Functions --------------------------------------------------- */
extern "C" {
	/*
	 * Note: MQL calls are not available until after this function returns.
	 */
	MT4_API RobotReferenceType initialise(int robot_name_ref);
	MT4_API void deinitialise(RobotReferenceType instance, const int reason);
    MT4_API int createStringReference();
    MT4_API void stringAddChar(int ref, char c);
}
#endif