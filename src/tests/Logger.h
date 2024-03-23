#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#define LOG_INFO(str) { std::cout << __FILE__ << ":" << __LINE__ << ": " << str << std::endl; }

class Logger {
public:
  static void LogMessage(std::string str);
};

#endif // LOGGER_H
