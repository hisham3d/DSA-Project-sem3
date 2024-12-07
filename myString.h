//#pragma once
//﻿#include <iostream>
//using namespace std;

class String
{
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



    int getsize();
    // Conversion Operator
    //operator int() const; // returns the length of string
    ~String(); // destructor
    friend std::ostream& operator<<(std::ostream& output, const String& str); // outputs the string
    friend std::istream& operator>>(std::istream& input, String& str); // inputs the string

    // btree
    const char* c_str() const {
        return arr;
    }

    bool operator<(const String& other) const {
        return strcmp(this->arr, other.arr) < 0;
    }

    bool operator<=(const String& other) const {
        return strcmp(this->arr, other.arr) <= 0;
    }

    bool operator>(const String& other) const {
        return strcmp(this->arr, other.arr) > 0;
    }

    bool operator>=(const String& other) const {
        return strcmp(this->arr, other.arr) >= 0;
    }

    String& operator=(string& str) {
        //if (this == &str) return *this; // Check for self-assignment

        // Clean up existing data
        delete[] arr;

        // Allocate new memory for the new string
        size = str.size();
        arr = new char[size + 1];
        std::copy(str.begin(), str.end(), arr);
        arr[size] = '\0';

        return *this;
    }

    int find(char ch, int start = 0) const {
        if (start < 0 || start >= size) {
            return -1; // Invalid start position
        }
        for (int i = start; i < size; i++) {
            if (arr[i] == ch) {
                return i; // Return the index of the first occurrence
            }
        }
        return -1; // Character not found
    }

    int find(const String& target) const {
        if (target.size > size || target.size == 0) {
            return -1;
        }

        for (size_t i = 0; i <= size - target.size; ++i) {
            bool match = true;

            for (size_t j = 0; j < target.size; ++j) {
                if (arr[i + j] != target.arr[j]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                return static_cast<int>(i);
            }
        }

        return -1;
    }

    bool operator==(const string& str) const { // returns true if string nd String are equal
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
    };

    String substr(int start, int length) {
        if (start < 0 || start >= size || length < 0 || start + length > size) {
            throw std::out_of_range("Invalid start or length for substring");
        }

        char* subStr = new char[length + 1];
        for (int i = 0; i < length; ++i) {
            subStr[i] = arr[start + i];
        }
        subStr[length] = '\0';

        String substring(subStr);
        delete[] subStr;
        return substring;
    }

    String substr(int start) {
        if (start < 0 || start >= size) {
            throw std::out_of_range("Invalid start position for substring");
        }

        int length = size - start;
        char* subStr = new char[length + 1];
        for (int i = 0; i < length; ++i) {
            subStr[i] = arr[start + i];
        }
        subStr[length] = '\0';

        String substring(subStr);
        delete[] subStr;
        return substring;
    }

    friend String operator+(const char* lhs, const String& rhs) {
        int lhsSize = 0;
        while (lhs[lhsSize] != '\0') {
            lhsSize++;
        }

        int newSize = lhsSize + rhs.size;
        char* newArr = new char[newSize + 1];

        for (int i = 0; i < lhsSize; ++i) {
            newArr[i] = lhs[i];
        }
        for (int i = 0; i < rhs.size; ++i) {
            newArr[lhsSize + i] = rhs.arr[i];
        }
        newArr[newSize] = '\0';

        String result(newArr);
        delete[] newArr;
        return result;
    }

    // Overload + operator for concatenating a String and a C-string
    friend String operator+(const String& lhs, const char* rhs) {
        int rhsSize = 0;
        while (rhs[rhsSize] != '\0') {
            rhsSize++;
        }

        int newSize = lhs.size + rhsSize;
        char* newArr = new char[newSize + 1];

        for (int i = 0; i < lhs.size; ++i) {
            newArr[i] = lhs.arr[i];
        }
        for (int i = 0; i < rhsSize; ++i) {
            newArr[lhs.size + i] = rhs[i];
        }
        newArr[newSize] = '\0';

        String result(newArr);
        delete[] newArr;
        return result;
    }

    string& toStdString()
    {
        static string res(arr); // Static to extend lifetime

        return res;
    }

    // Operator overloading for += with another String
    String& operator+=(const String& other) {
        int newSize = size + other.size;
        char* newArr = new char[newSize + 1];

        // Copy current string to newArr
        for (int i = 0; i < size; ++i) {
            newArr[i] = arr[i];
        }

        // Copy other string to newArr
        for (int i = 0; i < other.size; ++i) {
            newArr[size + i] = other.arr[i];
        }

        newArr[newSize] = '\0';

        // Clean up the old array and assign the new one
        delete[] arr;
        arr = newArr;
        size = newSize;

        return *this;
    }

    String operator+(const string& str) {
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

    // Operator overloading for += with a const char* (C-string)
    String& operator+=(const char* str) {
        int strLen = 0;
        while (str[strLen] != '\0') {
            strLen++;
        }

        int newSize = size + strLen;
        char* newArr = new char[newSize + 1];

        // Copy current string to newArr
        for (int i = 0; i < size; ++i) {
            newArr[i] = arr[i];
        }

        // Copy C-string to newArr
        for (int i = 0; i < strLen; ++i) {
            newArr[size + i] = str[i];
        }

        newArr[newSize] = '\0';

        // Clean up the old array and assign the new one
        delete[] arr;
        arr = newArr;
        size = newSize;

        return *this;
    }

    // Method to find the last occurrence of a character
    int find_last_of(char ch) {
        for (int i = size - 1; i >= 0; --i) {
            if (arr[i] == ch) {
                return i;
            }
        }
        return -1; // Return -1 if the character is not found
    }

    // btree
    void erase(int pos, int count = 1) {
        if (pos < 0 || pos >= size || count <= 0) {
            return; // Invalid position or count, do nothing
        }
        if (pos + count >= size) {
            size = pos; // Erase to the end
        }
        else {
            for (int i = pos; i + count < size; i++) {
                arr[i] = arr[i + count];
            }
            size -= count;
        }
        arr[size] = '\0'; // Null-terminate the string
    }
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

//#include <string>

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

int String::getsize() { // returns the length of string
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
    if (size > 0)
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

int stoi(String str) {
    int result = 0;
    int sign = 1;
    size_t index = 0;

    // Handle optional sign at the beginning
    if (str[index] == '-') {
        sign = -1;
        index++;
    }
    else if (str[index] == '+') {
        index++;
    }

    // Convert characters to integer
    for (; index < str.getsize(); index++) {
        if (str[index] < '0' || str[index] > '9') {
            throw std::invalid_argument("Invalid character in input string");
        }
        result = result * 10 + (str[index] - '0');
    }

    return sign * result;
}

double stod(String& str) {
    double result = 0.0;
    size_t index = 0;
    int sign = 1;

    // Handle optional sign at the beginning
    if (str[index] == '-') {
        sign = -1;
        index++;
    }
    else if (str[index] == '+') {
        index++;
    }

    // Convert integer part
    while (index < str.getsize() && str[index] >= '0' && str[index] <= '9') {
        result = result * 10 + (str[index] - '0');
        index++;
    }

    // Handle the decimal point
    if (index < str.getsize() && str[index] == '.') {
        index++;
        double fraction = 0.0;
        int power = 1;

        while (index < str.getsize() && str[index] >= '0' && str[index] <= '9') {
            fraction = fraction * 10 + (str[index] - '0');
            power *= 10;
            index++;
        }

        result += fraction / power;
    }

    return sign * result;
}

String tostring(int value) {
    // Handle the edge case of 0 directly
    if (value == 0) {
        return String("0");
    }

    int temp = value;
    int length = 0;
    bool isNegative = value < 0;
    if (isNegative) {
        temp = -temp; // Make the number positive for now
    }

    // Calculate the length of the number
    int num = temp;
    while (num > 0) {
        num /= 10;
        length++;
    }

    // Adjust length for negative numbers
    if (isNegative) {
        length++;
    }

    // Create a character array to hold the digits
    char* buffer = new char[length + 1];
    buffer[length] = '\0'; // Null-terminate the string

    // Fill the buffer with the digits from the end
    for (int i = length - 1; i >= 0; --i) {
        if (isNegative) {
            buffer[i] = (value % 10) + '0';
            value /= 10;
        }
        else {
            buffer[i] = (temp % 10) + '0';
            temp /= 10;
        }
    }

    // Add a negative sign if necessary
    if (isNegative) {
        buffer[0] = '-';
    }

    String result(buffer);
    delete[] buffer; // Free the temporary buffer
    return result;
}