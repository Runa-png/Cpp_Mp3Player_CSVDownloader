#include <string>
#include <iostream>
#include <filesystem>

#include "cppFunctions/checkFileExists/checkFileExists.h"

bool checkFileExists(std::string location) {
  if (std::filesystem::exists(location)) {
    return true;
  } else {
    return false;
  }
}

bool checkFileIsCSV(std::string location) {
  if (!checkFileExists(location)) {
    return false;
  }

  std::filesystem::path filePath(location);

  return filePath.extension() == ".csv";
}