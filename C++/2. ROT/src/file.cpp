#include "../include/rot.h"

std::string readFile(const std::string &fileName) {
  std::ifstream targetFile(fileName);
  // The program has no data to operate on if it cannot read the file
  if (!targetFile.is_open()) {
    std::cout << "Unable to access " << fileName << "\n";
    exit(EXIT_FAILURE);
  }
  // The number of chars in the file is determined by putting the pointer at the
  // end and finding its position
  targetFile.seekg(0, std::ios::end);
  size_t targetSize = targetFile.tellg();
  // A string of the same size as targetFile is created
  std::string target(targetSize, ' ');
  targetFile.seekg(0);
  // The contents of targetFile are written into the target variable
  targetFile.read(&target[0], targetSize);
  return target;
}

int writeFile(std::string &outputText, std::string &outputFile) {
  std::ofstream output(outputFile, std::ofstream::out);
  output << outputText;
  return EXIT_SUCCESS;
}