#include "../include/mainwindow.h"
#include "../include/rot.h"
#include "./ui_mainwindow.h"
#include <QChar>
#include <algorithm>

#define ALPHABET_LENGTH 26
#define ASSUMED_MOST_FREQUENT 4 // 'e' in the alphabet

// Shift each letter in target by key, where key is an integer
QString encrypt(const QString &target, int key) {
    QString cipherText;
    for (QChar i : target) {
        if (i.isLetter()) {
            int unicodeValue = i.unicode();
            QChar newChar;
            if (i.isUpper()) {
                newChar = QChar((unicodeValue - 'A' + key) % ALPHABET_LENGTH + 'A');
            } else {
                newChar = QChar((unicodeValue - 'a' + key) % ALPHABET_LENGTH + 'a');
            }
            cipherText += newChar;
        } else {
            cipherText += i; // Append non-letter characters unchanged
        }
    }
    return cipherText;
}

// Decrypt the text by shifting letters backwards
QString decrypt(const QString &target, int key) {
    QString clearText;
    for (QChar i : target) {
        if (i.isLetter()) {
            int unicodeValue = i.unicode();
            QChar newChar;
            if (i.isUpper()) {
                newChar = QChar((unicodeValue - 'A' - key + ALPHABET_LENGTH) % ALPHABET_LENGTH + 'A');
            } else {
                newChar = QChar((unicodeValue - 'a' - key + ALPHABET_LENGTH) % ALPHABET_LENGTH + 'a');
            }
            clearText += newChar;
        } else {
            clearText += i; // Append non-letter characters unchanged
        }
    }
    return clearText;
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
    auto maxElementIt = std::max_element(letterOccurrences.begin(), letterOccurrences.end());
    if (maxElementIt != letterOccurrences.end()) {
        int mostFreqLetterIndex = std::distance(letterOccurrences.begin(), maxElementIt);
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

