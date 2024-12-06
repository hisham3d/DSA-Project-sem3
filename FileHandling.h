#pragma once
#include"utilities.h"
#include <filesystem>
using namespace std;
namespace fs = filesystem;

bool hasPath(const string& path)
{
    struct stat buffer {};
    return (stat(path.c_str(), &buffer) == 0);
}

void createFolder(const string& path)
{
    if (hasPath(path))
        return;

    if (_mkdir(path.c_str()) == -1)
    {
        cout << path << endl;
    }
}

void createFile(const string& path)
{
    fstream output(path, ios::out);
    output.close();
}

void displayBranches()
{
    namespace fs = filesystem;

    fs::path branchesDir = fs::current_path() / "BRANCHES";

    cout << "Existing branches:\n";
    vector<fs::directory_entry> entries;

    // Collect entries using an index-based loop
    for (auto it = fs::directory_iterator(branchesDir); it != fs::directory_iterator(); ++it)
    {
        entries.push_back(*it);
    }

    for (size_t i = 0; i < entries.size(); ++i)
    {
        if (fs::is_directory(entries[i]))
        {
            cout << "- " << entries[i].path().filename().string() << '\n';
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

void CREATEBRANCHESFOLDERS(string name)
{
    createFolder("BRANCHES\\" + name);
    CREATETREEFOLDERS(name);
}

void copyDirectory(const string& source, const string& destination)
{
    namespace fs = filesystem;

    vector<fs::directory_entry> entries;
    for (auto it = fs::recursive_directory_iterator(source); it != fs::recursive_directory_iterator(); ++it)
    {
        entries.push_back(*it);
    }

    for (size_t i = 0; i < entries.size(); ++i)
    {
        const auto& path = entries[i].path();
        auto relativePathStr = fs::relative(path, source).string();
        if (entries[i].is_directory())
        {
            fs::create_directories(destination + "\\" + relativePathStr);
        }

        else if (entries[i].is_regular_file())
        {
            fs::copy_file(path, destination + "\\" + relativePathStr, fs::copy_options::overwrite_existing);
        }
    }
}

void deleteBranch()
{
    namespace fs = filesystem;


   

    cout << "\nEnter the name of the branch to delete: ";
    string branchName;
    cin >> branchName;

    fs::path branchesDir = fs::current_path() / "BRANCHES";
    fs::path branchPath = branchesDir / branchName;

    if (fs::exists(branchPath) && fs::is_directory(branchPath))
    {
        try
        {
            fs::remove_all(branchPath);
            cout << "Branch '" << branchName << "' has been successfully deleted.\n";
        }

        catch (const exception& e)
        {
            cerr << "Error: Could not delete branch '" << branchName << "'. Reason: " << e.what() << '\n';
        }
    }

    else
    {
        cout << "Branch '" << branchName << "' does not exist.\n";
    }
}

void mergeBranches(const string& sourceBranch, const string& targetBranch)
{
    namespace fs = filesystem;

    fs::path branchesDir = fs::current_path() / "BRANCHES";
    fs::path sourcePath = branchesDir / sourceBranch;
    fs::path targetPath = branchesDir / targetBranch;

    if (!fs::exists(sourcePath) || !fs::is_directory(sourcePath))
    {
        cout << "Error: Source branch '" << sourceBranch << "' does not exist.\n";
        return;
    }

    if (!fs::exists(targetPath) || !fs::is_directory(targetPath))
    {
        cout << "Error: Target branch '" << targetBranch << "' does not exist.\n";
        return;
    }

    for (fs::recursive_directory_iterator it(sourcePath), end; it != end; ++it)
    {
        fs::directory_entry entry = *it;

        fs::path relativePath = fs::relative(entry.path(), sourcePath);
        fs::path targetFilePath = targetPath / relativePath;

        if (entry.is_directory())
        {
            if (!fs::exists(targetFilePath))
            {
                fs::create_directories(targetFilePath);
            }
        }

        else if (entry.is_regular_file())
        {
            fs::copy_file(entry.path(), targetFilePath, fs::copy_options::overwrite_existing);
        }
    }

    cout << "Merged '" << sourceBranch << "' into '" << targetBranch << "' successfully.\n";

    if (fs::exists(sourcePath) && fs::is_directory(sourcePath))
    {
        if (fs::remove_all(sourcePath) > 0)
        {
            cout << "Source branch '" << sourceBranch << "' deleted successfully.\n";
        }

        else
        {
            cout << "Error: Could not delete source branch '" << sourceBranch << "'.\n";
        }
    }

    else
    {
        cout << "Error: Source branch '" << sourceBranch << "' does not exist or is not a directory.\n";
    }
}

string branchSelection()
{
    
    int branchChoice;
    string branchName; // default branch
    cout << "Select Choice:" << endl;
    cout << "[1] Main Branch" << endl;
    cout << "[2] Create/Use Another Branch" << endl;
    cout << "[3] Delete Branch" << endl;
    cout << "[4] Merge Branches" << endl;
    cout << "[?] Merge Branches" << endl;
    cout << "Enter your choice: ";
    cin >> branchChoice;
    while ((branchChoice < 5) && (branchChoice > 0)) {

        if (branchChoice == 1) {

            branchName = "main";
            CREATEBRANCHESFOLDERS(branchName);
        }

        if (branchChoice == 2)
        {
            cout << "Enter branch name: ";
            cin.ignore();
            getline(cin, branchName);

            string branchPath = "BRANCHES\\" + branchName;
            if (!hasPath(branchPath))
            {
                cout << "Creating branch: " << branchName << endl;
                fs::create_directories(branchPath);

                CREATEBRANCHESFOLDERS(branchName);

                copyDirectory("BRANCHES\\main", branchPath);
            }

            else
            {
                cout << "Switching to existing branch: " << branchName << endl;
            }
        }
        if (branchChoice == 3) {
            deleteBranch();

        }
        if (branchChoice == 4) {

            string sourceBranch, targetBranch;

            cout << "Enter the source branch to merge from: ";
            cin >> sourceBranch;

            cout << "Enter the target branch to merge into: ";
            cin >> targetBranch;

            mergeBranches(sourceBranch, targetBranch);


        }
        cout << endl;

        cout << "Working in branch: " << branchName << endl;

        cout << endl;
        displayBranches();
        cout << endl;

        cout << "Select Choice:" << endl;
        cout << "[1] Main Branch" << endl;
        cout << "[2] Create/Use Another Branch" << endl;
        cout << "[3] Delete Branch" << endl;
        cout << "[4] Merge Branches" << endl;
        cout << "[?] Enter other number to quit" << endl;
        cout << "Enter your choice: ";
        cin >> branchChoice;
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