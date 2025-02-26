#ifndef UTITILY_H
#define UTILITY_H

#include <iostream>

// Error handling functions
inline void error(std::string str) __attribute__((noreturn));
inline void error(std::string str)
{
  std::cout << "Error: " << str << std::endl;
  exit(1);
}

#endif

