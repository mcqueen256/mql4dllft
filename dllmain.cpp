#include "export.h"
void dllmain() {
	std::string mt4_folder = "C:\\Users\\nicholas\\AppData\\Roaming\\MetaQuotes\\Terminal\\3294B546D50FEEDA6BF3CFC7CF858DB7\\";
	std::ofstream* fout = new std::ofstream(mt4_folder + "MQL4\\Logs\\mql4dllft_std_log.txt", std::ios::out);
	std::ofstream* ferr = new std::ofstream(mt4_folder + "MQL4\\Logs\\mql4dllft_err_log.txt", std::ios::out);
	std::cout.rdbuf(fout->rdbuf());
	std::cerr.rdbuf(ferr->rdbuf());
	std::cout << "it begins" << std::endl << std::flush;
}

