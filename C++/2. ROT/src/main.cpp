// ROT - encrypt or decrypt a plaintext file using Caesar shift
// Author: Jacob Niemeir <nniemeir@protonmail.com>

#include "../include/rot.h"

int outputResult(int printToFile, std::string postText, std::string outputFile,
                 int key) {
  if (printToFile == 1) {
    writeFile(postText, outputFile);
    return EXIT_SUCCESS;
  }

  std::cout << "Key: " << key << "\n";
  std::cout << "~~~" << "\n";
  std::cout << postText;
  std::cout << "~~~\n";
  return EXIT_SUCCESS;
}

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

void processArgs(int argc, char *argv[], int *shiftMode, std::string *inputFile,
                 std::string *outputFile, long *key, int *printToFile) {
  int c;
  int modeArgsGiven;
  modeArgsGiven = 0;
  std::string inputArg;

  while ((c = getopt(argc, argv, "e::d::h::i:k:o:")) != -1) {
    switch (c) {
    case 'e':
      modeArgsGiven++;
      *shiftMode = 1;
      break;
    case 'd':
      modeArgsGiven++;
      *shiftMode = 2;
      break;
    case 'h':
      printHelpMsg();
      exit(EXIT_SUCCESS);
      break;
    case 'i':
      if (optarg) {
        inputArg = optarg;
      } else {
        fprintf(stderr, "Option -i requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'k':
      if (optarg) {
        *key = convertInputToLong(optarg);
        if (*key <= 0) {
          std::cout << "Key must be an integer greater than zero.\n";
          exit(EXIT_FAILURE);
        }
      } else {
        fprintf(stderr, "Option -k requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'o':
      if (optarg) {
        *printToFile = 1;
        *outputFile = optarg;
      } else {
        fprintf(stderr, "Option -o requires an argument.\n");
        exit(EXIT_FAILURE);
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
      exit(EXIT_FAILURE);
    }
  }

  int index;
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
    *inputFile = inputArg;
  } else {
    std::cout << "Input directory not found.\n";
    exit(EXIT_FAILURE);
  }

  if (*shiftMode == 1) {
    if (key == 0) {
      std::cout << "Key required for encryption.\n";
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char *argv[]) {
  std::string inputFile;
  std::string outputFile;
  int printToFile = 0;
  int shiftMode = 0;
  int index;
  long key = 0;

  processArgs(argc, argv, &shiftMode, &inputFile, &outputFile, &key,
              &printToFile);

  std::string target = readFile(inputFile);
  std::string postText;

  if (shiftMode == 1) {
    postText = encrypt(target, key);
    return outputResult(printToFile, postText, outputFile, key);
  }

  // If key is not provided by user, frequency analysis is used to guess it
  if (key == 0) {
    key = findKey(target);
  }

  if (key == 0) {
    std::cout << "The text does not appear to be encrypted\n";
    return EXIT_SUCCESS;
  }

  postText = decrypt(target, key);
  return outputResult(printToFile, postText, outputFile, key);
}
