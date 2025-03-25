#include "../include/rot.h"

void printHelpMsg() {
  std::cout << "Usage: rot [options]\n";
  std::cout << "Options:\n";
  std::cout << "  -d                Decrypt text in input file\n";
  std::cout << "  -e                Encrypt text in input file\n";
  std::cout << "  -i <input_file>   Specify path to input file\n";
  std::cout << "  -k <key>          Specify cipher key (only required for "
               "encryption)\n";
  std::cout << "  -o <output_file>  Specify path to output file (output "
               "directed to stdout otherwise)\n";
  std::cout << "\n";
  std::cout << "Example usage:\n";
  std::cout << "  rot -e -i INPUT_FILE -o OUTPUT_FILE -k KEY\n";
  std::cout << "  rot -d -i INPUT_FILE\n";
  std::cout << "\n";
}