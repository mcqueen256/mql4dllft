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
	MT4_API int initialise(int number) {
		static bool init = false;
		if (!init) {
			dllmain();
			init = true;
		}
		
		try {
			NBRobot* robot = new NBRobot(number);
			Instance::push(robot);
			return (int)robot;
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			return 0;
		}
	}
}
#endif