#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
int main()
{
    system("clear");
    std::ifstream target_File("target.txt");
    target_File.seekg(0, std::ios::end);
    size_t size = target_File.tellg();
    std::string target(size, ' ');
    target_File.seekg(0);
    target_File.read(&target[0], size);
    int key;
    std::cout << "1. Encrypt File \n2. Decrypt File ";
    int choice;
    std::cin >> choice;
    if (choice == 1)
    {
        std::cout << "Enter a ROT key: ";
        std::cin >> key;
        for (char i : target)
        {
            int test = isalpha(i);
            if (test)
            {
                if (isupper(i))
                {
                    i = (i - 'A' + key) % 26 + 'A';
                }
                else
                {
                    i = (i - 'a' + key) % 26 + 'a';
                }
            }
            std::cout << i;
        }
        std::cout << std::endl;
    }
    else if (choice == 2)
    {
        std::cout << "1. I Know The Key\n2. I Do Not Know The Key ";
        int index;
        int choice2;
        std::cin >> choice2;
        system("clear");
        if (choice2 == 1)
        {
            std::cout << "\nEnter the key" << std::endl;
            std::cin >> key;
        }
        else if (choice2 == 2)
        {
            const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
            std::vector<int> occurrences;
            for (char i : alphabet)
            {
                std::string::difference_type charFreq = std::count(target.begin(), target.end(), i);
                occurrences.push_back(charFreq);
            }
            auto it = std::max_element(occurrences.begin(), occurrences.end());
            if (it != occurrences.end())
            {
                int index = std::distance(occurrences.begin(), it);
                std::cout << "Most Common Letter: " << alphabet[index] << std::endl;
                if (index != 4)
                {
                    int diff = abs(index - 4);
                    if (index < 4)
                    {
                        diff = 26 - diff;
                    }
                    std::cout << "Most Likely Cipher: ROT" << diff << std::endl;
                }
                else
                {
                    std::cout << "The text does not appear to have been shifted" << std::endl;
                }
            }
        }
        std::cout << "Output:\n\n~~~" << std::endl;
        for (char i : target)
        {
            int test = isalpha(i);
            if (test)
            {
                if (isupper(i))
                {
                    i = (i - 'A' - key + 26) % 26 + 'A';
                }
                else
                {
                    i = (i - 'a' - key + 26) % 26 + 'a';
                }
            }
                std::cout << i;
        }
        std::cout << "\n~~~\n" << std::endl;
    }
    else
    {
        std::cout << "Invalid Option, Please Try Again" << std::endl;
    }
    return 0;
}
