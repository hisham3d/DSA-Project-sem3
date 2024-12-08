#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "CustomVector.h"
using namespace std;

class Commit_Log {
public:
    int commitNumber;
    string user;
    string query;
    string timestamp;
    Commit_Log() : commitNumber(0), user(""), query(""), timestamp("") {}

    Commit_Log(int commitNum, const string& usr, const string& qry, const string& ts)
        : commitNumber(commitNum), user(usr), query(qry), timestamp(ts) {}
};

CustomVector<Commit_Log> parseCommitHistory1(const string& branchName) {
    CustomVector<Commit_Log> history;
    string path = "BRANCHES\\" + branchName + "\\commit_log.txt";
    ifstream file(path);

    if (!file.is_open()) {
        cout << "No commit history found for branch '" << branchName << "'.\n";
        return history;
    }

    string line;
    for (int i = 0; i < 7 && getline(file, line); ++i) {
    }

    while (getline(file, line)) {
        if (line.empty() || line[0] != '|') {
            continue;
        }

        try {
            int pos1 = line.find('<');
            int pos2 = line.find('>');
            int pos3 = line.find('|', pos2 + 1);
            int pos4 = line.find('|', pos3 + 1);
            int pos5 = line.find('|', pos4 + 1);
            int pos6 = line.rfind('|');

            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos &&
                pos4 != string::npos && pos5 != string::npos && pos6 != string::npos) {

                int commitNumber = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));

                string user = line.substr(pos3 + 1, pos4 - pos3 - 1);
                user.erase(user.find_last_not_of(' ') + 1);

                string query = line.substr(pos5 + 1, pos6 - pos5 - 1);
                query.erase(query.find_last_not_of(' ') + 1);

                string timestamp = line.substr(pos6 + 1);
                timestamp.erase(timestamp.find_last_not_of(' ') + 1);

                history.emplace_back(commitNumber, user, query, timestamp);
            }
        }
        catch (const std::exception& e) {
            cout << "Error parsing commit log entry: " << e.what() << "\n";
        }
    }

    file.close();
    return history;
}

void displayCommitChanges(const string& branchName) {
    // File path for the commit log
    string path = "BRANCHES\\" + branchName + "\\commit_log.txt";

    // Check if the file exists
    if (!std::filesystem::exists(path)) {
        cout << "No commit log found for branch '" << branchName << "'.\n";
        return;
    }

    // Open the file in read mode
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Failed to access commit log for branch '" << branchName << "'.\n";
        return;
    }

    // Read and display the contents of the file line by line
    string line;
    cout << "\nDisplaying Commit Log for '" << branchName << "':\n";
    while (getline(file, line)) {
        cout << line << "\n";
    }

    file.close();
    cout << "\n----- End of Commit Log -----\n";
}


string getCurrentTimestamp1() {

    auto now = chrono::system_clock::now();
    time_t timeT = chrono::system_clock::to_time_t(now);

    struct tm localTime;
    localtime_s(&localTime, &timeT);

    stringstream ss;
    ss << put_time(&localTime, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS
    return ss.str();
}
void addCommit1(const string& branchName, const CustomVector<string>& LogMessage) {

    string user;
    cin.ignore();
    cout << "\nEnter username: ";
    getline(cin, user, '\n');
    cout << endl;
    string path = "BRANCHES\\" + branchName + "\\commit_log.txt";

    bool fileExists = std::filesystem::exists(path);

    ofstream file(path, ios::app);


    if (!fileExists) {
        file << "                                    _________________\n";
        file << "                                   |                 |\n";
        file << "                                   |  < COMMIT LOG > |\n";
        file << "                                   |_________________|\n";
        file << " ___________________________________________________________________________________________________________\n";
        file << "|ID  |        USER         | TREE  |           QUERY               |          TIMESTMP            | VERSION |\n";
        file << "|____|_____________________|_______|_______________________________|______________________________|_________|\n";
    }

    string timestamp = getCurrentTimestamp1();

    CustomVector<Commit_Log> history = parseCommitHistory1(branchName);
    int nextCommitNumber = history.empty() ? 1 : history.back().commitNumber + 1;
    string combinedQuery = LogMessage[1] + " -> " + LogMessage[2];

    file << "|" << left << setw(4) << ("<" + to_string(nextCommitNumber) + ">")
        << "| " << setw(20) << user
        << "| " << setw(6) << LogMessage[0]
        << "| " << setw(30) << combinedQuery
        << "| " << setw(28) << timestamp
        << " | " << setw(7) << to_string(getLatestVersion()) << " |\n";
    

    file.close();

    cout << "----- Commit log updated -----\n";
}