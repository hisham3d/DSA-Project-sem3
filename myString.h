#include <iostream>
using namespace std;

class String
{
private:
    char* arr;
    int size;

public:

    String()
    {
        arr = NULL;
        size = 0;
    }

    // String from char*
    String(const char* str)
    {
        int index = 0;
        while (str[index] != '\0')
        {
            index++;
        }

        size = index + 1;
        arr = new char[size];

        for (int i = 0; i < index; i++)
        {
            arr[i] = str[i];
        }
        arr[index] = '\0';  // Properly terminate the string
    }


    // String from String
    String(const String& s)
    {
        size = s.size;
        arr = new char[size];
        for (int i = 0; i < size; i++)
        {
            arr[i] = s.arr[i];
        }
    }

    // String from string
    String(const string& s)
    {
        this->size = s.size();
        arr = new char[size];

        for (int i = 0; i < size; i++)
        {
            arr[i] = s[i];
        }

        arr[size] = '\0';
    }

    // initializes a string of predefined size
    String(int x)
    {
        size = x;
        arr = new char[size] {'\0'};
    }

    char* getdata()
    {
        return arr;
    }

    const char* c_str() const
    {
        return arr;
    }

    // returns the length of string
    int getsize()
    {
        int length = 0;
        while (this->arr[length] != '\0')
        {
            length++;
        }

        return length;
    }

    const char operator[](int i) const
    {
        if (i < 0)
        {
            i = size + i - 1;
        }

        if (i >= 0 && i < size)
        {
            return arr[i];
        }

        else
        {
            return '\0';
        }
    }

    String operator+(const String& str) const
    {
        int newSize = size + str.size - 1;
        char* resultArr = new char[newSize];

        for (int i = 0; i < size - 1; i++) {
            resultArr[i] = arr[i];
        }

        for (int j = 0; j < str.size; j++)
        {
            resultArr[size - 1 + j] = str.arr[j];
        }

        // Create and return a new String object
        return String(resultArr);
    }

    String operator+(const char& ch) const
    {
        //cout << "Working2"<< endl;
        int newSize = size + 1; // Add one extra space for the new character
        char* resultArr = new char[newSize];

        // Copy the original string
        for (int i = 0; i < size - 1; i++)
        {
            resultArr[i] = arr[i];
        }

        // Append the new character
        resultArr[size - 1] = ch;
        resultArr[newSize - 1] = '\0';

        // Create and return a new String object
        return String(resultArr);
    }


    // String + char*
    String operator+(const char* str) const
    {
        //cout << "Working"<< endl;
        int s1 = 0, s2 = 0;

        while (this->arr[s1] != '\0')
        {
            s1++;
        }

        while (str[s2] != '\0')
        {
            s2++;
        }

        char* result = new char[s1 + s2 + 1];

        for (int i = 0; i < s1; i++)
        {
            result[i] = this->arr[i];
        }

        for (int j = 0; j < s2; j++)
        {
            result[s1 + j] = str[j];
        }
        result[s1 + s2] = '\0';

        String newString(result);
        delete[] result;  // Free temporary memory
        return newString;
    }


    // char* + String
    friend String operator+(const char* lhs, const String& rhs)
    {
        //cout << "Working3"<< endl;
        int lhsSize = 0;
        while (lhs[lhsSize] != '\0')
        {
            lhsSize++;
        }

        int newSize = lhsSize + rhs.size;
        char* newArr = new char[newSize + 1];

        for (int i = 0; i < lhsSize; ++i)
        {
            newArr[i] = lhs[i];
        }

        for (int i = 0; i < rhs.size; ++i)
        {
            newArr[lhsSize - 1 + i] = rhs.arr[i];
        }
        newArr[newSize] = '\0';

        String result(newArr);
        delete[] newArr;
        return result;
    }

    // String + char*
    friend String operator+(const String& lhs, const char* rhs)
    {
        int rhsSize = 0;
        while (rhs[rhsSize] != '\0')
        {
            rhsSize++;
        }

        int newSize = lhs.size - 1 + rhsSize;
        char* newArr = new char[newSize + 1];

        for (int i = 0; i < lhs.size - 1; ++i)
        {
            newArr[i] = lhs.arr[i];
        }

        for (int i = 0; i < rhsSize; ++i)
        {
            newArr[lhs.size - 1 + i] = rhs[i];
        }

        newArr[newSize] = '\0';
        String result(newArr);
        delete[] newArr;

        return result;
    }

    // String += String
    String& operator+=(const String& other)
    {
        int newSize = size + other.size; // 11
        char* newArr = new char[newSize + 1]; // null

        for (int i = 0; i < size; ++i)  // abcde fghijk 
        {
            newArr[i] = arr[i];
        }

        for (int i = 0; i < other.size; ++i)
        {
            newArr[size + i - 1] = other.arr[i];
        }

        newArr[newSize - 1] = '\0';

        delete[] arr;
        arr = newArr;
        size = newSize;

        //cout <<"HERE" <<this->c_str()<<endl;

        return *this;
    }

    // String += char*
    String& operator+=(const char* str)
    {
        int strLen = 0;
        while (str[strLen] != '\0')
        {
            strLen++;
        }

        int newSize = size + strLen;
        char* newArr = new char[newSize + 1];

        for (int i = 0; i < size; ++i)
        {
            newArr[i] = arr[i];
        }

        for (int i = 0; i < strLen; ++i)
        {
            newArr[size + i - 1] = str[i];
        }

        newArr[newSize - 1] = '\0';

        delete[] arr;
        arr = newArr;
        size = newSize;

        return *this;
    }

    // String = String
    String& operator=(const String& str)
    {

        int index = 0;
        while (str.arr[index] != '\0')
        {
            index++;
        }

        char* result = new char[index + 1];

        for (int i = 0; i < index; i++)
        {
            result[i] = str.arr[i];
        }
        result[index] = '\0';

        delete[] this->arr;
        this->arr = result;

        return *this;
    }

    // String = string
    String& operator=(const string& str)
    {
        // Allocate new memory for the new string
        size = str.size();
        arr = new char[size + 1];
        std::copy(str.begin(), str.end(), arr);
        arr[size] = '\0';

        return *this;
    }

    // String = char*
    String& operator=(char* str)
    {
        int index = 0;
        while (str[index] != '\0')
        {
            index++;
        }

        char* result = new char[index + 1];

        for (int i = 0; i < index; i++)
        {
            result[i] = str[i];
        }
        result[index] = '\0';

        delete[] this->arr;
        this->arr = result;

        return *this;
    }

    // String == String
    bool operator==(const String& str) const
    {
        int index = 0;

        bool temp = true;
        while (this->arr[index] != '\0')
        {
            if (this->arr[index] == str.arr[index])
            {
                index++;
                continue;
            }

            else
            {
                temp = false;
                break;
            }
        }

        if (temp)
        {
            return true;
        }

        else
        {
            return false;
        }
    }

    // String == string
    bool operator==(const string& str) const
    {
        int index = 0;

        bool temp = true;
        while (this->arr[index] != '\0')
        {
            if (this->arr[index] == str[index])
            {
                index++;
                continue;
            }

            else
            {
                temp = false;
                break;
            }
        }

        if (temp)
        {
            return true;
        }

        else
        {
            return false;
        }
    }

    // String == char*
    bool operator==(const char* str) const
    {
        cout << "YESSIR" << endl;
        int index = 0;

        bool temp = true;
        while (this->arr[index] != '\0')
        {
            if (this->arr[index] == str[index])
            {
                index++;
                continue;
            }

            else
            {
                temp = false;
                break;
            }
        }

        if (temp)
        {
            return true;
        }

        else
        {
            return false;
        }
    }

    ~String()
    {
        delete[] arr;
    }

    int find(char ch, int start = 0) const
    {
        for (int i = start; i < size; i++)
        {
            if (arr[i] == ch)
            {
                return i; // Return the index of the first occurrence
            }
        }
    }

    int find(const String& target) const
    {
        for (int i = 0; i <= size - target.size; ++i)
        {
            bool match = true;

            for (int j = 0; j < target.size; ++j)
            {
                if (arr[i + j] != target.arr[j])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                return i;
            }
        }
    }

    // outputs the string
    friend ostream& operator<<(ostream& output, const String& str)
    {
        if (str.arr != NULL)
        {
            output << str.arr;
        }

        if (str.size > 0)
        {
            output << endl;
        }
        return output;
    }

    // inputs the string
    friend istream& operator>>(istream& input, String& str)
    {
        char* temp = new char[1000];
        input.getline(temp, 1000);
        str.arr = temp;
        return input;
    }

    //bool operator<(const String& other) const 
    //{
    //    return strcmp(this->arr, other.arr) < 0;
    //}

    //bool operator<=(const String& other) const 
    //{
    //    return strcmp(this->arr, other.arr) <= 0;
    //}

    //bool operator>(const String& other) const 
    //{
    //    return strcmp(this->arr, other.arr) > 0;
    //}

    //bool operator>=(const String& other) const 
    //{
    //    return strcmp(this->arr, other.arr) >= 0;
    //}
};

int stoi(String str)
{
    int result = 0;

    // Convert characters to integer
    for (int index = 0; index < str.getsize(); index++)
    {
        result = result * 10 + (str[index] - '0');
    }

    return result;
}

double stod(String& str)
{
    double result = 0.0;
    int index = 0;

    // Convert integer part
    while (index < str.getsize() && str[index] >= '0' && str[index] <= '9')
    {
        result = result * 10 + (str[index] - '0');
        index++;
    }

    // Handle the decimal point
    if (index < str.getsize() && str[index] == '.')
    {
        index++;
        double fraction = 0.0;
        int power = 1;

        while (index < str.getsize() && str[index] >= '0' && str[index] <= '9')
        {
            fraction = fraction * 10 + (str[index] - '0');
            power *= 10;
            index++;
        }

        result += fraction / power;
    }

    return result;
}

String tostring(int value)
{
    if (value == 0)
    {
        return String("0");
    }

    int temp = value;
    int length = 0;
    bool isNegative = value < 0;
    if (isNegative)
    {
        temp = -temp; // Make the number positive for now
    }

    // Calculate the length of the number
    int num = temp;
    while (num > 0)
    {
        num /= 10;
        length++;
    }

    // Adjust length for negative numbers
    if (isNegative)
    {
        length++;
    }

    // Create a character array to hold the digits
    char* buffer = new char[length + 1];
    buffer[length] = '\0'; // Null-terminate the string

    // Fill the buffer with the digits from the end
    for (int i = length - 1; i >= 0; --i)
    {
        if (isNegative)
        {
            buffer[i] = (value % 10) + '0';
            value /= 10;
        }

        else
        {
            buffer[i] = (temp % 10) + '0';
            temp /= 10;
        }
    }

    // Add a negative sign if necessary
    if (isNegative)
    {
        buffer[0] = '-';
    }

    String result(buffer);
    delete[] buffer; // Free the temporary buffer
    return result;
}

template <typename T>
std::string toString(const T& data) {
    std::ostringstream oss;
    oss << data;
    return oss.str();
}
