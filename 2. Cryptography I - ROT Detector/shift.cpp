#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
int main()
{
    std::cout << "Analyzing file...";
    std::ifstream target_File("target.txt");
    target_File.seekg(0, std::ios::end);
    size_t size = target_File.tellg();
    std::string target(size, ' ');
    target_File.seekg(0);
    target_File.read(&target[0], size);
    std::transform(target.begin(), target.end(), target.begin(), ::tolower);
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
        std::cout << "done! \nMost Common Letter: " << alphabet[index] << std::endl;
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
    return 0;
}
