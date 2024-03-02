#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <optional>

const std::string FILE_NAME = "target.txt";
const int ALPHABET_LENGTH = 26;
const int ASSUMED_MOST_FREQUENT = 4;

std::string encrypt(const std::string& target, int key) {
	std::string cipherText;
	for (char i : target)
        {
            int test = isalpha(i);
            if (test != 0)
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
            int test = isalpha(i);
            if (test)
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
int diff = 0;
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
                }
	    }
return diff;
}

int main()
{
    std::ifstream targetFile("target.txt");
    targetFile.seekg(0, std::ios::end);
    size_t size = targetFile.tellg();
    std::string target(size, ' ');
    targetFile.seekg(0);
    targetFile.read(&target[0], size);
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
	int key = findKey(target);
	if (key==0) {
		std::cout << "The text does not appear to be encrypted\n";
	}
	else {
        std::cout << "Output:\n\n~~~" << "\n";
	std::string clearText = decrypt(target, key);
        std::cout << clearText;
	std::cout << "\n~~~\n\n";
	}
    }
    }
    else
    {
        std::cout << "Invalid Option, Please Try Again\n";
    }
    return 0;
}
