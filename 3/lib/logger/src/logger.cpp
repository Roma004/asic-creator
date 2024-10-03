#include "logger/logger.hpp"
#include <fstream>

std::ofstream f_debug_log("debug.log");

Logger debug("\e[33;1;4mDEBUG\e[0m", f_debug_log);
Logger error("\e[31;1;4mERROR\e[0m", std::cerr);
Logger info("\e[32;1;4mINFO\e[0m", std::cout);

