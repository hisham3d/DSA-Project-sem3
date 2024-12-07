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

String fileName = "healthcare_dataset.csv";
class AddressLocation
{
public:
    String filename;
    String linenumber;
    AddressLocation() {};
    AddressLocation(String f, String l)
    {
        filename = f;
        linenumber = l;
    }
};

bool alphaCheck(String& str)
{
    for (int i = 0; i < str.getsize(); ++i)
    {
        char s = str[i];
        if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z'))
        {
            if (!(s >= '0' && s <= '9') || !(s == '.'))
                return true;
        }
    }
    return false;
}

bool DecimalCheck(String& str)
{
    for (int i = 0; i < str.getsize(); ++i)
    {
        if (str[i] == '.')
            return true;
    }
    return false;
}

int countChar(String& str, char ch)
{
    int count = 0;
    for (int i = 0; i < str.getsize(); ++i)
    {
        if (str[i] == ch)
            ++count;
    }
    return count;
}

String toLower(String f)
{
    String out = "";
    for (int i = 0; i < f.getsize(); i++)
    {
        if (isupper(f[i]))
            out += tolower(f[i]);

        else
            out += f[i];
    }
    return out;
}

String toUpper(String f)
{
    String out = "";
    for (int i = 0; i < f.getsize(); i++)
    {
        if (islower(f[i]))
            out += toupper(f[i]);

        else
            out += f[i];
    }
    return out;
}

String DetermineDataType(String& str)
{
    if (alphaCheck(str))
        return "string";

    if (DecimalCheck(str))
        return "double";

    return "int";
}

int comparestring(String key, String val)
{
    if (key == val)
        return 0;

    if (DetermineDataType(key) == "string")
    {
        return strcmp(key.c_str(), val.c_str());
    }

    else if (DetermineDataType(key) == "int")
    {
        if (key.getsize() < val.getsize())
            return -1;

        else if (key.getsize() > val.getsize())
            return 1;

        for (int i = 0; i < key.getsize(); i++)
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

        for (int i = 0; i < key.getsize(); i++)
        {
            if (key[i] < val[i])
                return -1;
            if (key[i] > val[i])
                return 1;
        }
    }
}

CustomVector<String> split(String& str, char delimiter) {
    CustomVector<String> result;
    stringstream sstream(str.toStdString());
    String token;
    while (getline(sstream, token.toStdString(), delimiter)) {
        result.push_back(token);
    }
    return result;
}

String join(const CustomVector<String>& fields, char delimiter) {
    String result;
    for (int i = 0; i < fields.getSize(); i++) {
        if (i > 0) result += delimiter; // Add delimiter between fields
        result += fields[i];
    }
    return result;
}


void AllocateDataTypes(CustomVector<String>& DataType, CustomVector<String>& Fline)
{
    //CustomVector<String> out;
    for (int i = 0; i < Fline.getSize(); ++i)
    {
        String dataType = DetermineDataType(Fline[i]);
        DataType.push_back(dataType);
    }

    //return out;
}

int getFieldIndex(CustomVector<String>& fields, String fName)
{
    for (int i = 0; i < fields.getSize(); i++)
        if (toLower(fName) == toLower(fields[i]))
            return i;
    return -1;
}

void READLINE(fstream& file, CustomVector<String>& list)
{
    list.resize(0);
    String FirstLine, word = "";
    getline(file, FirstLine.toStdString());
    stringstream Fline(FirstLine.toStdString());

    while (getline(Fline, word.toStdString(), ','))
    {
        if (word.getsize() < 1)
            continue;
        if (countChar(word, '"') % 2 == 1)
        {
            String temp;
            getline(Fline, temp.toStdString(), '"');
            word += "," + temp + "\"";
        }
        list.push_back(word);
    }
}