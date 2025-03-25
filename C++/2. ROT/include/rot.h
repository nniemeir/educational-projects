#ifndef ROT_H_
#define ROT_H_

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <linux/limits.h>
#include <unistd.h>
#include <vector>

#define ALPHABET_LENGTH 26
#define ASSUMED_MOST_FREQUENT                                                  \
  4 // The index of the letter e in the array of all English letters

std::string encrypt(const std::string &target, int key);
std::string decrypt(const std::string &target, int key);
int findKey(const std::string &target);
std::string readFile(const std::string &fileName);
void printHelpMsg();
int writeFile(std::string &outputText, std::string &outputFile);

#endif