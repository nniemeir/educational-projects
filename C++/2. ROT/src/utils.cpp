#include "../include/rot.h"

void helpMsg() {
  std::cout << "Usage: rot [options]\n";
  std::cout << "Options:\n";
  std::cout << "  -b               Prepend pattern to basenames\n";
  std::cout << "  -d <directory>   Specify directory\n";
  std::cout << "  -e               Append pattern to basenames\n";
  std::cout << "  -h               Display this help message\n";
  std::cout << "  -p <pattern>     Specify pattern to prepend/append\n";
  std::cout << "  -x <extension>   Specify extension of files to perform "
               "renaming on\n";
  std::cout << "\n";
  std::cout << "Example usage:\n";
  std::cout << "  presuff -b -d /path/to/directory -p pat_ -x txt\n";
  std::cout << "  presuff -e -p _pat\n";
  std::cout << "\n";
}