//
// pch.h
//

// Hisham Abdullah
// 23I-0693
// Assignment 2

#pragma once

#include <iostream>
using namespace std;

class String {
private:
    char* arr;
    int size;
public:
    // provide definitions of following functions
    String(); // default constructor
    String(const char* str); // initializes the string with constant c-string
    String(const String&); // copy constructor to initialize the string from the existing string
    String(int x); // initializes a string of predefined size
    char* getdata(); //returns the string inside the object
    // Binary Operators //
    // Sub script operators
    const char operator[](int i) const; // returns the character at index [x]
    //NOTE: in above [] operator functions if i=negative int value, print ith character from end 
    //of string e.g. in case of �LOOP� if i=-1 OR i=3, it should return �P� similarly i = -4 OR i = 0, //return �L�
    // Arithmetic Operators
    String operator+(const String& str); // appends a String at the end of the String
    String operator+(const char& str); // appends a char at the end of the String
    String operator+(const char* str); // appends a String at the end of the String
    String operator-(const String& substr); //removes the substr from the String
    String operator-(const char& str); //removes all occurrences of char from the String
    String operator-(const char* str); //removes the str from the String
    // Assignment Operators
    String& operator=(const String&); // copies one String to another
    String& operator=(char*); // copies one c-string to another
    // Logical Operators
    bool operator==(const String&) const; // returns true if two Strings are equal
    bool operator==(const char*) const; // returns true if the c-string is equal to the String
    // Unary Operators //
    // Boolean Not Operator
    bool operator!(); // returns true if the String is empty
    // Function-Call Operators
    //If something is not found then return -1
    int operator()(char) const; // returns the first index of the character being searched
    int operator()(const String&) const; // returns the first index of the String being searched
    int operator()(const char*) const; // returns the index of the c-string being searched


    // Conversion Operator
    operator int() const; // returns the length of string
    ~String(); // destructor
    friend std::ostream& operator<<(std::ostream& output, const String& str); // outputs the string
    friend std::istream& operator>>(std::istream& input, String& str); // inputs the string
};



String::String() { // default constructor
    arr = NULL;
    size = 0;
};

String::String(const char* str) { // initializes the string with constant c-string
    int index = 0;
    while (str[index] != '\0') {
        index++;
    }
    size = index + 1;
    arr = new char[size] {'\0'};
    for (int i = 0; i < size; i++) {
        arr[i] = str[i];
    }
};

String::String(const String& s) { // copy constructor to initialize the string from the existing string
    this->size = s.size;
    arr = new char[size] {'\0'};
    for (int i = 0; i < size; i++) {
        arr[i] = s[i];
    }
};

String::String(int x) { // initializes a string of predefined size
    size = x;
    arr = new char[size] {'\0'};
};

char* String::getdata() {
    return arr;
};

const char String::operator[](int i) const {
    if (i < 0) {
        i = size + i - 1;
    }

    if (i >= 0 && i < size) {
        return arr[i];
    }
    else {
        return '\0';
    }
}

String String::operator+(const String& str) {
    int s1 = 0, s2 = 0;
    while (this->arr[s1] != '\0') {
        s1++;
    }
    while (str.arr[s2] != '\0') {
        s2++;
    }

    char* result = new char[s1 + s2 + 1];

    for (int i = 0; i < s1; i++) {
        result[i] = this->arr[i];
    }
    for (int j = 0; j < s2; j++) {
        result[s1 + j] = str.arr[j];
    }
    result[s1 + s2] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

String String::operator+(const char& str) {
    int s1 = 0;
    while (this->arr[s1] != '\0') {
        s1++;
    }

    char* result = new char[s1 + 2];

    for (int i = 0; i < s1; i++) {
        result[i] = this->arr[i];
    }
    result[s1] = str;
    result[s1 + 1] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

String String::operator+(const char* str) {
    int s1 = 0, s2 = 0;
    while (this->arr[s1] != '\0') {
        s1++;
    }
    while (str[s2] != '\0') {
        s2++;
    }

    char* result = new char[s1 + s2 + 1];

    for (int i = 0; i < s1; i++) {
        result[i] = this->arr[i];
    }
    for (int j = 0; j < s2; j++) {
        result[s1 + j] = str[j];
    }
    result[s1 + s2] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

#include <string>

String String::operator-(const String& substr) {
    int s1 = 0, s2 = 0;
    while (this->arr[s1] != '\0') {
        s1++;
    }
    while (substr.arr[s2] != '\0') {
        s2++;
    }

    char* result = new char[s1 + 1];

    int i = 0; // result.
    int j = 0; // this->arr.

    while (this->arr[j] != '\0') {
        bool temp = true;
        for (int k = 0; k < s2; k++) {
            if (this->arr[j + k] == substr.arr[k]) {
                continue;
            }
            else {
                temp = false;
                break;
            }
        }
        if (temp) {
            j += s2; // skip the substring indexes.
        }
        else {
            result[i++] = this->arr[j++];
        }
    }
    result[i] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

String String::operator-(const char& str) {
    int s1 = 0;
    while (this->arr[s1] != '\0') {
        s1++;
    }

    char* result = new char[s1 + 1];

    int i = 0; // result.
    int j = 0; // this->arr.

    while (this->arr[j] != '\0') {
        if (this->arr[j] == str) {
            j++; // skip the character index.
        }
        else {
            result[i++] = this->arr[j++];
        }
    }
    result[i] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

String String::operator-(const char* str) {
    int s1 = 0;
    while (this->arr[s1] != '\0') {
        s1++;
    }
    int s2 = 0;
    while (str[s2] != '\0') {
        s2++;
    }

    char* result = new char[s1 + 1];

    int i = 0; // result.
    int j = 0; // this->arr.

    while (this->arr[j] != '\0') {
        bool temp = true;
        for (int k = 0; k < s2; k++) {
            if (this->arr[j + k] == str[k]) {
                continue;
            }
            else {
                temp = false;
                break;
            }
        }
        if (temp) {
            j += s2; // skip the substring indexes.
        }
        else {
            result[i++] = this->arr[j++];
        }
    }
    result[i] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

int String::operator()(char ch) const {
    int index = 0;
    while (this->arr[index] != '\0') {
        if (this->arr[index] == ch)
            return index;
        index++;
    }

    return -1;
}

int String::operator()(const String& str) const {
    int index = 0;
    int strIndex = 0;
    while (this->arr[index] != '\0') {
        if (this->arr[index] == str.arr[strIndex]) {
            int temp = index;
            while (this->arr[temp] == str.arr[strIndex] && str.arr[strIndex] != '\0') {
                temp++;
                strIndex++;
            }
            if (str.arr[strIndex] == '\0')
                return index;
            else
                strIndex = 0;
        }
        index++;
    }

    return -1;
}

int String::operator()(const char* str) const {
    int index = 0;
    int strIndex = 0;
    while (this->arr[index] != '\0') {
        if (this->arr[index] == str[strIndex]) {
            int temp = index;
            while (this->arr[temp] == str[strIndex] && str[strIndex] != '\0') {
                temp++;
                strIndex++;
            }
            if (str[strIndex] == '\0')
                return index;
            else
                strIndex = 0;
        }
        index++;
    }

    return -1;
}

String::operator int() const { // returns the length of string
    int length = 0;
    while (this->arr[length] != '\0') {
        length++;
    }

    return length;
};

String& String::operator=(const String& str) { // copies one String to another
    int index = 0;
    while (str.arr[index] != '\0') {
        index++;
    }

    char* result = new char[index + 1];

    for (int i = 0; i < index; i++) {
        result[i] = str.arr[i];
    }
    result[index] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

String& String::operator=(char* str) { // copies one c-string to another
    int index = 0;
    while (str[index] != '\0') {
        index++;
    }

    char* result = new char[index + 1];

    for (int i = 0; i < index; i++) {
        result[i] = str[i];
    }
    result[index] = '\0';

    delete[] this->arr;
    this->arr = result;

    return *this;
}

bool String::operator!() { // returns true if the String is empty
    int index = 0;
    if (this->arr[index] == '\0') {
        return true;
    }
    else {
        return false;
    }
};

bool String::operator==(const String& str) const { // returns true if two Strings are equal
    int index = 0;

    bool temp = true;
    while (this->arr[index] != '\0') {
        if (this->arr[index] == str.arr[index]) {
            index++;
            continue;
        }
        else {
            temp = false;
            break;
        }
    }

    if (temp) {
        return true;
    }
    else {
        return false;
    }
};

bool String::operator==(const char* str) const { // returns true if the c-string is equal to the String
    int index = 0;

    bool temp = true;
    while (this->arr[index] != '\0') {
        if (this->arr[index] == str[index]) {
            index++;
            continue;
        }
        else {
            temp = false;
            break;
        }
    }

    if (temp) {
        return true;
    }
    else {
        return false;
    }
}


String::~String() {
    delete[] arr;
};

std::ostream& operator<<(std::ostream& output, const String& str) {  // outputs the string
    if (str.arr != NULL) {
        output << str.arr;
    }
    if (str.size > 0) {
        output << endl;
    }
    return output;
};

std::istream& operator>>(std::istream& input, String& str) { // inputs the string
    char* temp = new char[1000];
    input.getline(temp, 1000);
    str.arr = temp;
    return input;
};

