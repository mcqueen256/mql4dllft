#ifndef EXPORT_H
#define EXPORT_H

#include <iostream>
#include <fstream>

#include "Instance.hpp"
#include "Robot.hpp"

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

void dllmain() {
	std::string mt4_folder = "C:\\Users\\nicholas\\AppData\\Roaming\\MetaQuotes\\Terminal\\3294B546D50FEEDA6BF3CFC7CF858DB7\\";
	std::ofstream* fout = new std::ofstream(mt4_folder + "MQL4\\Logs\\mql4dllft_std_log.txt", std::ios::out);
	std::ofstream* ferr = new std::ofstream(mt4_folder + "MQL4\\Logs\\mql4dllft_err_log.txt", std::ios::out);
	std::cout.rdbuf(fout->rdbuf());
	std::cerr.rdbuf(ferr->rdbuf());
	std::cout << "it begins" << std::endl << std::flush;
}

/* DLL Exported Functions --------------------------------------------------- */
extern "C" {
	/*
	 * Note: MQL calls are not available until after this function returns.
	 */
	MT4_API int initialise(int param1) {
		static bool init = false;
		if (!init) {
			dllmain();
			init = true;
		}
		
		try {
			Robot* robot = new Robot(param1);
			Instance::push(robot);
			return reinterpret_cast<int>(robot);
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			return 0;
		}
	}

	MT4_API void deinitialise(int instance, const int reason) {
		/* Dealocate the robot memory. */
		delete Instance::at(instance);
		/* Remove robot from existing instances. */
		Instance::erase(instance);
	}

	MT4_API void enableTrading(int instance) {

	}

	MT4_API void disableTrading(int instance) {

	}

	MT4_API int bar(int instance, char* time, double open, double high, double low, double close, double volume) {
		return 0;
	}

	MT4_API int quote(int instance, char* time, double open, double high, double low, double close, double volume) {
		return 0;
	}
}
#endif