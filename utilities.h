#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include "myString.h"
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>
#include <stdio.h>
#include <set>
#include <iomanip>
using namespace std;

string fileName = "healthcare_dataset.csv";
class AddressLocation
{
public:
	string filename;
	string linenumber;
	AddressLocation() {};
	AddressLocation(string f, string l)
	{
		filename = f;
		linenumber = l;
	}
};

void AllocateDataTypes(CustomVector<string>& DataType, CustomVector<string>& Fline)
{
	//CustomVector<string> out;
	for (size_t i = 0; i < Fline.getSize(); ++i)
	{
		string dataType = DetermineDataType(Fline[i]);
		DataType.push_back(dataType);
	}

	//return out;
}

int getFieldIndex(CustomVector<string>& fields, string fName)
{
	for (int i = 0; i < fields.getSize(); i++)
		if (toLower(fName) == toLower(fields[i]))
			return i;
	return -1;
}

void READLINE(fstream& file, CustomVector<string>& list)
{
	list.resize(0);
	string FirstLine, word = "";
	getline(file, FirstLine);
	stringstream Fline(FirstLine);

	while (getline(Fline, word, ','))
	{
		if (word.size() < 1)
			continue;
		if (countChar(word, '"') % 2 == 1)
		{
			string temp;
			getline(Fline, temp, '"');
			word += "," + temp + "\"";
		}
		list.push_back(word);
	}
}