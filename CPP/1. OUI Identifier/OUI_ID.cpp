#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
vector<string> readRecordFromFile(string file_name, string search_term);
int main()
{

	string MAC;
	cout << "Enter MAC Address: ";
	cin >> MAC;
	MAC.erase(remove(MAC.begin(), MAC.end(), ':'), MAC.end());
	MAC.erase(remove(MAC.begin(), MAC.end(), '-'), MAC.end());
	MAC.resize(6);
	transform(MAC.begin(), MAC.end(), MAC.begin(), ::toupper);
	vector<string> data = readRecordFromFile("IEEE.csv", MAC);
	return 0;
}

vector<string> readRecordFromFile(string file_name, string search_term)
{
	vector<string> record;
	ifstream file;
	file.open(file_name);
	bool found_record = false;
	string field_one;
	string field_two;
	while (getline(file, field_one, ',') && !found_record)
	{
		getline(file, field_two, '\n');
		if (field_one == search_term || field_one == "EOF")
		{
			found_record = true;
			record.push_back(field_one);
			record.push_back(field_two);
		}
	}
	record[1].resize(record[1].size() - 2);
	cout << "This NIC was manufactured by: " << record[1] << '\n';
	return record;
}
