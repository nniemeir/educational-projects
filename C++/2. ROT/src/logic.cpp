#include "../include/rot.h"

/*
Shift each letter in target by x, where x is key.
It is necessary to use A instead of a when the letter is upper-case because the
ASCII values of upper and lower-case letters differ
*/
std::string encrypt(const std::string &target, int key) {
  std::string cipherText;
  for (char i : target) {
    if (isalpha(i)) {
      if (isupper(i)) {
        i = (i - 'A' + key) % ALPHABET_LENGTH + 'A';
      } else {
        i = (i - 'a' + key) % ALPHABET_LENGTH + 'a';
      }
    }
    cipherText += i;
  }
  return cipherText;
}

// The same logic from the encrypt function applies here, but we shift backwards
// instead of forwards to decrypt
std::string decrypt(const std::string &target, int key) {
  std::string clearText;
  for (char i : target) {
    if (isalpha(i)) {
      if (isupper(i)) {
        i = (i - 'A' - key + ALPHABET_LENGTH) % ALPHABET_LENGTH + 'A';
      } else {
        i = (i - 'a' - key + ALPHABET_LENGTH) % ALPHABET_LENGTH + 'a';
      }
    }
    clearText += i;
  }
  return clearText;
}

/* Since the letter e is most common in English, the key is the absolute value
of the difference between the index value of the most common letter in target
and e in alphabet.
*/
int findKey(const std::string &target) {
  std::string caseInsensitiveTarget = target.tolower();
  const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
  std::vector<int> letterOccurrences;
  for (char i : alphabet) {
    std::string::size_type charFreq =
        std::count(caseInsensitiveTarget.begin(), caseInsensitiveTarget.end(), i);
    letterOccurrences.push_back(charFreq);
  }
  auto it =
      std::max_element(letterOccurrences.begin(), letterOccurrences.end());
  if (it != letterOccurrences.end()) {
    int mostFreqLetterIndex = std::distance(letterOccurrences.begin(), it);
    if (mostFreqLetterIndex != ASSUMED_MOST_FREQUENT) {
      int diff = abs(mostFreqLetterIndex - ASSUMED_MOST_FREQUENT);
      if (mostFreqLetterIndex < ASSUMED_MOST_FREQUENT) {
        diff = ALPHABET_LENGTH - diff;
      }
      return diff;
    }
  }
  return EXIT_SUCCESS;
}