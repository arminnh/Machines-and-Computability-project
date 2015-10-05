#ifndef COMPARE_FILES_H
#define COMPARE_FILES_H

#include <gtest/gtest.h>
#include <fstream>

bool compareFiles(const std::string &filename1, const std::string &filename2)
{
  std::ifstream ifs1(filename1.c_str());
  std::ifstream ifs2(filename2.c_str());

  std::string line1, line2;
  while (std::getline(ifs1, line1)) {
    std::getline(ifs2, line2);
    if (line1 != line2)
      return false;
  }

  return true;
}

#endif
