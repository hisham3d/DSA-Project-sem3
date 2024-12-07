#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "CustomVector.h"
using namespace std;

struct Commit_Log {
    int commitNumber;
    string message;
    string timestamp;

    Commit_Log() : commitNumber(0), message(""), timestamp("") {}

    Commit_Log(int num, const string& msg, const string& time)
        : commitNumber(num), message(msg), timestamp(time) {}
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
    while (getline(file, line)) {
        int commitNumber;
        string message, timestamp;
        size_t pos1 = line.find(':');
        size_t pos2 = line.find_last_of(':');

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            commitNumber = stoi(line.substr(0, pos1));
            message = line.substr(pos1 + 1, pos2 - pos1 - 1);
            timestamp = line.substr(pos2 + 1);
            history.emplace_back(commitNumber, message, timestamp);
        }
    }

    file.close();
    return history;
}

void displayCommitHistory1(const string& branchName) {
    CustomVector<Commit_Log> history = parseCommitHistory1(branchName);

    if (history.empty()) {
        cout << "No commits to display for branch '" << branchName << "'.\n";
        return;
    }

    cout << "Commit_Log History for '" << branchName << "':\n";
    for (const auto& commit : history) {
        cout << "- Commit_Log #" << commit.commitNumber << ": \"" << commit.message << "\"\n";
        cout << "  Timestamp: " << commit.timestamp << "\n";
    }
    cout << endl;
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
    string path = "BRANCHES\\" + branchName + "\\commit_log.txt";
    ofstream file(path, ios::app);

    string timestamp = getCurrentTimestamp1();

    CustomVector<Commit_Log> history = parseCommitHistory1(branchName);
    int nextCommitNumber = history.empty() ? 1 : history.back().commitNumber + 1;

    file << nextCommitNumber << "|" << LogMessage[0] << "|" << LogMessage[1] << "|" << timestamp << "\n";
    file.close();

    cout << "----- Commit log updated -----" << "\n";
}
