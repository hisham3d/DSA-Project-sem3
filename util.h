#pragma once
#include<fstream>
#include<iostream>
#include<sstream>
#include<list>
#include<vector>
#include "myString.h"
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include<direct.h>
#include<stdio.h>
#include<set>
#include <iomanip>
using namespace std;

vector<string> arrFileNames = { "healthcare_dataset.csv" };
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
bool alphaCheck(const string& str)
{
	for (char s : str)
		if (s >= 'a' && s <= 'z' || s >= 'A' && s <= 'Z' && (!(s >= '0' && s <= '9') || !(s == '.')))
			return true;
	return false;
}
bool DecimalCheck(const string& str)
{
	for (char s : str)
		if (s == '.')
			return true;
	return false;
}
string DetermineDataType(const string& str)
{
	if (alphaCheck(str))
		return "string";
	if (DecimalCheck(str))
		return "double";
	return "int";
}
vector<string> AllocateDataTypes(vector<string>& Fline)
{
	vector<string>  out;
	for (auto s : Fline) {
		string dataType = DetermineDataType(s);
		out.push_back(dataType);
		//cout << "Storing: " << dataType << endl;
	}
	return out;
}
int countChar(string& str, char ch)
{
	int count = 0;
	for (auto s : str)
		count += ch == s;
	return count;
}
string toLower(string f)
{
	string out = "";
	for (int i = 0; i < f.size(); i++)
	{
		if (isupper(f[i]))
			out += tolower(f[i]);
		else
			out += f[i];
	}
	return out;
}
int getFieldIndex(vector<string>& fields, string fName)
{
	for (int i = 0; i < fields.size(); i++)
		if (toLower(fName) == toLower(fields[i]))
			return i;
	return -1;
}
int comparestring(string key, string val)
{
	if (key == val)
		return 0;
	if (DetermineDataType(key) == "string")
	{
		return strcmp(key.c_str(), val.c_str());
	}
	else if (DetermineDataType(key) == "int")
	{
		if (key.size() < val.size())
			return -1;
		else if (key.size() > val.size())
			return 1;
		for (int i = 0; i < key.size(); i++)
		{
			if (key[i] < val[i])
				return -1;
			if (key[i] > val[i])
				return 1;
		}
	}
	else
	{
		if (key.find('.') > val.find('.'))
			return 1;
		if (key.find('.') < val.find('.'))
			return -1;
		for (int i = 0; i < key.size(); i++)
		{
			if (key[i] < val[i])
				return -1;
			if (key[i] > val[i])
				return 1;
		}
	}
}
void READLINE(fstream& file, vector<string>& list)
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

void copyString(string s1, string s2) {

}

