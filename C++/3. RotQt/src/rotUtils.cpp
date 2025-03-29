#include "../include/rot.h"

static constexpr int ALPHABET_LENGTH = 26;
static constexpr int ASSUMED_MOST_FREQUENT = 4; // 'e' in the alphabet

// Shift each letter in target by key
QString shiftText(const QString &target, int key, bool isEncrypting) {
  QString processedText;
  // Keep key inside range of alphabet
  key = key % ALPHABET_LENGTH;
  if (key < 0) {
    key += ALPHABET_LENGTH;
  }
  for (QChar i : target) {
    if (i.isLetter()) {
      int unicodeValue = i.unicode();
      QChar newChar;
      if (i.isUpper()) {
        if (isEncrypting) {
          newChar = QChar((unicodeValue - 'A' + key) % ALPHABET_LENGTH + 'A');
        } else {
          newChar = QChar((unicodeValue - 'A' - key + ALPHABET_LENGTH) %
                              ALPHABET_LENGTH +
                          'A');
        }
      } else {
        if (isEncrypting) {
          newChar = QChar((unicodeValue - 'a' + key) % ALPHABET_LENGTH + 'a');
        } else {
          newChar = QChar((unicodeValue - 'a' - key + ALPHABET_LENGTH) %
                              ALPHABET_LENGTH +
                          'a');
        }
      }
      processedText += newChar;
    } else {
      processedText += i; // Append non-letter characters unchanged
    }
  }
  return processedText;
}

/* Since the letter e is most common in English, the key is the absolute value
of the difference between the index value of the most common letter in target
and e in alphabet.
*/
int findKey(const QString &target) {
  const QString alphabet = "abcdefghijklmnopqrstuvwxyz";
  QVector<int> letterOccurrences(ALPHABET_LENGTH, 0);

  for (int i = 0; i < alphabet.length(); ++i) {
    QChar letter = alphabet[i];
    int count = target.count(letter);
    letterOccurrences[i] = count;
  }
  auto maxElementIt =
      std::max_element(letterOccurrences.begin(), letterOccurrences.end());
  if (maxElementIt != letterOccurrences.end()) {
    int mostFreqLetterIndex =
        std::distance(letterOccurrences.begin(), maxElementIt);
    if (mostFreqLetterIndex != ASSUMED_MOST_FREQUENT) {
      int diff = std::abs(mostFreqLetterIndex - ASSUMED_MOST_FREQUENT);
      if (mostFreqLetterIndex < ASSUMED_MOST_FREQUENT) {
        diff = ALPHABET_LENGTH - diff;
      }
      return diff;
    }
  }
  return 0;
}
