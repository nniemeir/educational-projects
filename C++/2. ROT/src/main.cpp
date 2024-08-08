// ROT - encrypt or decrypt a plaintext file using Caesar shift
// Author: Jacob Niemeir <nniemeir@protonmail.com>

#include "../include/rot.h"

long convertInputToLong(char *numericInput) {
  char *endptr;
  long selection = strtol(numericInput, &endptr, 10);
  if (endptr == numericInput || *endptr != '\0') {
    fprintf(stderr, "Error converting input to long\n");
    std::cout << "Invalid input.\n";
    return -1;
  }
  return selection;
}


int main(int argc, char *argv[]) {
  std::string inputFile;
  std::string outputFile;
  std::string inputArg;
  std::string outputArg;
  std::string postText;
  int printToFile;
  int shiftMode;
  int modeArgsGiven;
  int index;
  int c;
  long key;
  printToFile = 0;
  key = 0;
  opterr = 0;
  shiftMode = 0;
  modeArgsGiven = 0;
  while ((c = getopt(argc, argv, "e::d::h::i:k:o:")) != -1) {
    switch (c) {
    case 'e':
      modeArgsGiven++;
      shiftMode = 1;
      break;
    case 'd':
      modeArgsGiven++;
      shiftMode = 2;
      break;
    case 'h':
      helpMsg();
      return EXIT_SUCCESS;
      break;
    case 'i':
      if (optarg) {
        inputArg = optarg;
      } else {
        fprintf(stderr, "Option -i requires an argument.\n");
        return EXIT_FAILURE;
      }
      break;
    case 'k':
      if (optarg) {
        key = convertInputToLong(optarg);
        if (key <= 0) {
          std::cout << "Key must be an integer greater than zero.\n";
          return EXIT_FAILURE;
        }
      } else {
        fprintf(stderr, "Option -k requires an argument.\n");
        return EXIT_FAILURE;
      }
      break;
    case 'o':
      if (optarg) {
        printToFile = 1;
        outputArg = optarg;
      } else {
        fprintf(stderr, "Option -o requires an argument.\n");
        return EXIT_FAILURE;
      }
      break;
    case '?':
      if (optopt == 'i' || optopt == 'k' || optopt == 'o') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      } else if (isprint(optopt)) {
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      }
      return EXIT_FAILURE;
    default:
      abort();
    }
  }

  for (index = optind; index < argc; index++)
    std::cout << "Non-option argument %s\n", argv[index];

  if (shiftMode == 0) {
    std::cout << "Missing mode argument.\n";
    exit(EXIT_FAILURE);
  }

  if (modeArgsGiven == 2) {
    std::cout << "Conflicting mode arguments given.\n";
    exit(EXIT_FAILURE);
  }

  if (std::filesystem::exists(inputArg)) {
    inputFile = inputArg;
  } else {
    std::cout << "Input directory not found.\n";
    return EXIT_FAILURE;
  }

  outputFile = outputArg;
  std::string target = readFile(inputFile);

  if (shiftMode == 1) {
    if (key == 0) {
      std::cout << "Key required for encryption.\n";
      return EXIT_FAILURE;
    }
    postText = encrypt(target, key);
  } else if (shiftMode == 2) {
    if (key == 0) {
      key = findKey(target);
      if (key == 0) {
        std::cout << "The text does not appear to be encrypted\n";
        return EXIT_SUCCESS;
      }
    }
    postText = decrypt(target, key);
  }
  if (printToFile == 1) {
    writeFile(postText, outputFile);
  } else {
    std::cout << "Key: " << key << "\n";
    std::cout << "~~~" << "\n";
    std::cout << postText;
    std::cout << "~~~\n";
  }
  return EXIT_SUCCESS;
}
