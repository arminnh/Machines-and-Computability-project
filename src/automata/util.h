#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <iostream>

template<typename T>
bool operator==(const std::vector<T> &v1, const std::vector<T> &v2)
{
  if (v1.size() != v2.size())
    return false;

  for (std::size_t i = 0; i < v1.size(); ++i)
    if (v1[i] != v2[i])
      return false;

  return true;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
  for (auto item : v)
    os << item << " ";
  return os;
}

template<typename T>
void printVector(const std::vector<T> &v)
{
  for (auto item : v)
    std::cout << std::to_string(item) << " ";
  std::cout << std::endl;
}

char i2c(int i)
{
  char c = static_cast<char>(((int)'0')+i);
  return c;
}

#ifndef DATADIR
#define DATADIR "./"
#endif

std::string datadir()
{
  return DATADIR + std::string("/");
}

#endif
