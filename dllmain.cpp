#include "export.h"
void dllmain() {
	std::string mt4_folder = "C:\\Users\\nicholas\\AppData\\Roaming\\";
	std::ofstream* fout = new std::ofstream(mt4_folder + "NBRotot_std_log.txt", std::ios::out);
	std::ofstream* ferr = new std::ofstream(mt4_folder + "NBRotot_err_log.txt", std::ios::out);
	std::cout.rdbuf(fout->rdbuf());
	std::cerr.rdbuf(ferr->rdbuf());
}