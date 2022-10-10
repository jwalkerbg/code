#include <iostream>
#include <chrono>

#include "util.h"

using namespace std;

int main()
{
  cout << "Hello World!" << endl;

  std::cout << std::chrono::microseconds(std::chrono::seconds(1)).count() << std::endl;

  std::cout << std::chrono::microseconds(2s).count() << std::endl;

  std::cout << func1(3,5) << std::endl;

  return 0;
}
