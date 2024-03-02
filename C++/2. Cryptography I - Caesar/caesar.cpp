/* 
Caesar - Encrypt and decrypt text using Caesar ciphers, with the option to use frequency analysis to guess the key of an encrypted file.

Author: Nat Niemeir <nniemeir@protonmail.com>
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <optional>

const std::string FILE_NAME = "target.txt"; // The file to be encrypted/decrypted
const int ALPHABET_LENGTH = 26; // The number of letters in the English alphabet
const int ASSUMED_MOST_FREQUENT = 4; // The index of the letter e in the array of all English letters

std::string encrypt(const std::string& target, int key) {
	std::string cipherText;
	for (char i : target)
        {
            if (isalpha(i))
            {
                if (isupper(i))
                {
                    i = (i - 'A' + key) % ALPHABET_LENGTH + 'A';
                }
                else
                {
                    i = (i - 'a' + key) % ALPHABET_LENGTH + 'a';
                }
            }
            cipherText += i;
        }
        return cipherText;
}

std::string decrypt(const std::string& target, int key) {
	std::string clearText;
	for (char i : target)
        {
            if (isalpha(i))
            {
                if (isupper(i))
                {
                    i = (i - 'A' - key + ALPHABET_LENGTH) % ALPHABET_LENGTH + 'A';
                }
                else
                {
                    i = (i - 'a' - key + ALPHABET_LENGTH) % ALPHABET_LENGTH + 'a';
                }
            }
	    clearText += i;
        }
	return clearText;
}

int findKey(const std::string& target) {
const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
            std::vector<int> letterOccurrences;
            for (char i : alphabet)
            {
                std::string::difference_type charFreq = std::count(target.begin(), target.end(), i);
                letterOccurrences.push_back(charFreq);
            }
            auto it = std::max_element(letterOccurrences.begin(), letterOccurrences.end());
            if (it != letterOccurrences.end())
            {
                int mostFreqLetterIndex = std::distance(letterOccurrences.begin(), it);
                if (mostFreqLetterIndex != ASSUMED_MOST_FREQUENT)
                {
                    int diff = abs(mostFreqLetterIndex - ASSUMED_MOST_FREQUENT);
                    if (mostFreqLetterIndex < ASSUMED_MOST_FREQUENT)
                    {
                        diff = ALPHABET_LENGTH - diff;
                    }
		       return diff;  // Return the calculated difference
        }
    }

    return 0;
}

int main()
{
    std::ifstream targetFile(FILE_NAME); 
    if (!targetFile.is_open()) {
	    std::cout << "Unable to access " << FILE_NAME << "\n";
	    return EXIT_FAILURE;
    }
    // The number of chars in the file is determined by putting the pointer at the end and finding its position
    targetFile.seekg(0, std::ios::end);
    size_t targetSize = targetFile.tellg();
    // A string of the same size as targetFile is created
    std::string target(targetSize, ' ');
    targetFile.seekg(0);
    // The contents of targetFile are written into the target variable
    targetFile.read(&target[0], targetSize);
    int key;
    std::cout << "1. Encrypt File \n2. Decrypt File ";
    int choice;
    std::cin >> choice;
    if (choice == 1)
    {
        std::cout << "Enter a ROT key: ";
        std::cin >> key;
	std::string cipherText = encrypt(target, key);
	std::cout << cipherText << "\n~~~\n\n"; 
    }
    else if (choice == 2)
    {
        std::cout << "1. I Know The Key\n2. I Do Not Know The Key ";
        int mostFreqLetterIndex;
        int choice2;
        std::cin >> choice2;
        if (choice2 == 1)
        {
            std::cout << "\nEnter the key: ";
            std::cin >> key;
        }
        else if (choice2 == 2)
        {
	key = findKey(target);
	std::cout << key;
	if (key==0) {
		std::cout << "The text does not appear to be encrypted\n";
		return 0;
	}
	}
        std::cout << "Output:\n\n~~~" << "\n";
	std::string clearText = decrypt(target, key);
        std::cout << clearText;
	std::cout << "\n~~~\n\n";
    }
    else
    {
        std::cout << "Invalid Option, Please Try Again\n";
    }
    return EXIT_SUCCESS;
}
