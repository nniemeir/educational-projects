/* 
OUI Lookup - Given a MAC address, match the OUI to a manufacturer's name using IEEE records

Author: Niemeir <nniemeir@protonmail.com>
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <optional>

const std::string FILE_NAME = "IEEE.csv";
const int MANUFACTURER_ID_LENGTH = 6;
const int MIN_MAC_LENGTH = 12;
const int MAX_MAC_LENGTH = 17;
const int SUFFIX_LENGTH = 2;

struct record {
	std::string manufacturerID;
	std::string manufacturerName;
};

// Search records for OUI that matches the one in MAC variable. Return corresponding name if match found, return nothing otherwise.
std::optional<record> searchRecords(std::ifstream& csvFile, std::string search_term)
{
	record targetRecord;
	bool found_record = false;
	std::string manufacturerIDField;
	std::string manufacturerNameField;
	// Search the first field of each line in the CSV file until a match is found or end of file
	while (std::getline(csvFile, manufacturerIDField, ',') && !found_record)
	{
		std::getline(csvFile, manufacturerNameField, '\n');
		if (manufacturerIDField == search_term)
		{
			// Assign the contents of each field in the line to the corresponding variable in targetRecord and exit the loop
			targetRecord.manufacturerID = manufacturerIDField;
			// Remove extra commas at end of second field
			targetRecord.manufacturerName = manufacturerNameField.substr(0, manufacturerNameField.size() - SUFFIX_LENGTH);
			return targetRecord;
		}
	}
	return std::nullopt;
}

bool validateInput(const std::string& mac) {
	return (mac.length() < MIN_MAC_LENGTH || mac.length() > MAX_MAC_LENGTH);
}

// Alter input string to match formatting of the CSV file
std::string formatMAC(const std::string& mac) {
	std::string searchTerm = mac;
	// Remove punctuation symbols from target string
	searchTerm.erase (remove_if(searchTerm.begin (), searchTerm.end (), ispunct), searchTerm.end ());
	searchTerm.erase (remove_if(searchTerm.begin (), searchTerm.end (), isspace), searchTerm.end ());

	// Truncate the target MAC address to 6 characters, the length of the manufacturer ID
	searchTerm.resize(MANUFACTURER_ID_LENGTH);
	// Capitalize all letters in target string so it will be in the same style as the CSV
	transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), toupper);
return searchTerm;
}

int main()
{
	// User input is taken for the target string
	std::ifstream csvFile;
	csvFile.open(FILE_NAME);
	if (!csvFile.is_open()) { 
		std::cout << "Unable to access " << FILE_NAME << "\n";
		return EXIT_FAILURE;
	}
	std::string MAC;
	std::cout << "Enter MAC Address: ";
	std::getline (std::cin,MAC);

	if (validateInput(MAC)) {
		std::cout << "Invalid MAC Address\n";
		return EXIT_FAILURE;
	}
	std::string searchTerm = formatMAC(MAC);
	// Our manufacturer ID table and the target string are passed to the recordSearch function
	std::optional<record> result = searchRecords(csvFile, searchTerm);
	csvFile.close();
	if (result.has_value()) {
	   std::cout << "This NIC was manufactured by: " << result.value().manufacturerName << "\n";
	} else {
		std::cout << "Manufacturer not found\n";
	}

	return EXIT_SUCCESS;
}

