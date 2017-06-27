#ifndef EXPORT_H
#define EXPORT_H

#include <iostream>
#include <fstream>

/* Macros ------------------------------------------------------------------- */
#ifdef NDEBUG
#define MT4_API __declspec(dllexport)
#else
#define MT4_API 
#endif

/* DLL Exported Functions --------------------------------------------------- */
extern "C" {
	MT4_API int initialise(int number) {
		return 0;
	}
}
#endif