#pragma once
#include"util.h"
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

bool hasPath(const string& path) {
    struct stat buffer {};
    return (stat(path.c_str(), &buffer) == 0);
}
void createFolder(const string& path) {
    if (hasPath(path))
        return;
    if (_mkdir(path.c_str()) == -1)
    {
        cout << path << endl;
    }
}
void createFile(const string& path) {
    fstream output(path, ios::out);
    output.close();
}

void displayBranches() {
    namespace fs = std::filesystem;

    fs::path branchesDir = fs::current_path() / "BRANCHES";

    std::cout << "Existing branches:\n";
    for (const auto& entry : fs::directory_iterator(branchesDir)) {
        if (fs::is_directory(entry)) {
            std::cout << "- " << entry.path().filename().string() << '\n';
        }
    }
}

void CREATETREEFOLDERS(string branch)
{
    //cout << "creating tree folders" << endl;
    createFolder("BRANCHES\\" + branch + "\\TREES");
    createFolder("BRANCHES\\" + branch + "\\TREES\\AVL");
    createFolder("BRANCHES\\" + branch + "\\TREES\\RB");
    createFolder("BRANCHES\\" + branch + "\\TREES\\BTREE");
}

void CREATEBRANCHESFOLDERS(string name) {
    createFolder("BRANCHES\\" + name);
    CREATETREEFOLDERS(name);
}

void copyDirectory(const string& source, const string& destination) {
    for (const auto& entry : fs::recursive_directory_iterator(source)) {
        const auto& path = entry.path();
        auto relativePathStr = fs::relative(path, source).string();
        if (entry.is_directory()) {
            fs::create_directories(destination + "\\" + relativePathStr);
        }
        else if (entry.is_regular_file()) {
            fs::copy_file(path, destination + "\\" + relativePathStr, fs::copy_options::overwrite_existing);
        }
    }
}

void deleteBranch() {
    namespace fs = std::filesystem;

    displayBranches();

    char ch;
    cout << "Do you want to delete a branch? (Y/N): ";
    cin >> ch;
    if (ch == 'N' || ch == 'n') {
        return;
    }

    cout << "\nEnter the name of the branch to delete: ";
    string branchName;
    cin >> branchName;

    fs::path branchesDir = fs::current_path() / "BRANCHES";
    fs::path branchPath = branchesDir / branchName;

    if (fs::exists(branchPath) && fs::is_directory(branchPath)) {
        try {
            fs::remove_all(branchPath);
            std::cout << "Branch '" << branchName << "' has been successfully deleted.\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error: Could not delete branch '" << branchName << "'. Reason: " << e.what() << '\n';
        }
    }
    else {
        std::cout << "Branch '" << branchName << "' does not exist.\n";
    }
}

void mergeBranches(const string& sourceBranch, const string& targetBranch) {
    namespace fs = std::filesystem;

    fs::path branchesDir = fs::current_path() / "BRANCHES";
    fs::path sourcePath = branchesDir / sourceBranch;
    fs::path targetPath = branchesDir / targetBranch;

    if (!fs::exists(sourcePath) || !fs::is_directory(sourcePath)) {
        cout << "Error: Source branch '" << sourceBranch << "' does not exist.\n";
        return;
    }

    if (!fs::exists(targetPath) || !fs::is_directory(targetPath)) {
        cout << "Error: Target branch '" << targetBranch << "' does not exist.\n";
        return;
    }

    for (fs::recursive_directory_iterator it(sourcePath), end; it != end; ++it) {
        fs::directory_entry entry = *it;

        fs::path relativePath = fs::relative(entry.path(), sourcePath);
        fs::path targetFilePath = targetPath / relativePath;

        if (entry.is_directory()) {
            if (!fs::exists(targetFilePath)) {
                fs::create_directories(targetFilePath);
            }
        }
        else if (entry.is_regular_file()) {
            fs::copy_file(entry.path(), targetFilePath, fs::copy_options::overwrite_existing);
        }
    }

    cout << "Merged '" << sourceBranch << "' into '" << targetBranch << "' successfully.\n";

    if (fs::exists(sourcePath) && fs::is_directory(sourcePath)) {
        if (fs::remove_all(sourcePath) > 0) {
            cout << "Source branch '" << sourceBranch << "' deleted successfully.\n";
        }
        else {
            cout << "Error: Could not delete source branch '" << sourceBranch << "'.\n";
        }
    }
    else {
        cout << "Error: Source branch '" << sourceBranch << "' does not exist or is not a directory.\n";
    }
}

string branchSelection() {
    int branchChoice;
    string branchName = "main"; // default branch
    cout << "Select Branch:" << endl;
    cout << "[1] Main Branch" << endl;
    cout << "[2] Create/Use Another Branch" << endl;
    cout << "Enter your choice: ";
    cin >> branchChoice;

    if (branchChoice == 2) {
        cout << "Enter branch name: ";
        cin.ignore();
        getline(cin, branchName);

        string branchPath = "BRANCHES\\" + branchName;
        if (!hasPath(branchPath)) {
            cout << "Creating branch: " << branchName << endl;
            fs::create_directories(branchPath);

            CREATEBRANCHESFOLDERS(branchName);

            copyDirectory("BRANCHES\\main", branchPath);
        }
        else {
            cout << "Switching to existing branch: " << branchName << endl;
        }
    }
    else {
        branchName = "main";
        CREATEBRANCHESFOLDERS(branchName);
    }

    return branchName;
}

void GetFields(string Filename, vector<string>& LINE1, vector<string>& Entries)
{
    fstream file;
    //file.open("FilesToREAD\\" + filename);
    file.open("FilesToREAD\\" + Filename);
    READLINE(file, LINE1);
    READLINE(file, Entries);
    file.close();
}