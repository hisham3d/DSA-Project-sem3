#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include"FileHandling.h"
#include "CustomVector.h"
#include "utilities.h"


using namespace std;
string activeBranch5;


class SHA256 {
public:
    string compute(const string& input) {
        unsigned long long hashValue = 0;

        for (char ch : input) {
            hashValue += static_cast<unsigned long long>(ch);
            hashValue *= 31;
            hashValue %= 1000000007;
        }

        stringstream ss;
        ss << hex << setw(16) << setfill('0') << hashValue;
        return ss.str();
    }
};


template <typename T>
struct MerkleNode {
    T data;
    std::string hash;
    MerkleNode* left;
    MerkleNode* right;
    vector<AddressLocation> AddressList; // Ensure this is included

    // Constructor for creating a leaf node (data of type T)
    MerkleNode(const T& data, SHA256& sha256) : left(nullptr), right(nullptr) {
        string valueAsString;
        if constexpr (std::is_same<T, string>::value) {
            valueAsString = data; // If T is already a string
        }
        else {
            valueAsString = std::to_string(data); // Convert numeric values to string
        }
        hash = sha256.compute(valueAsString);       // Compute hash
    }

    // Constructor for creating a parent node by combining the hashes of two child nodes
    MerkleNode(MerkleNode* left, MerkleNode* right, SHA256& sha256) : left(left), right(right) {
        hash = sha256.compute(left->hash + right->hash);
    } // Combine child hashes for parent
    MerkleNode() : left(nullptr), right(nullptr) {}
};


template <typename T>
class MerkleTree {
public:
    string fieldname;
    MerkleNode<T>* root;
    SHA256 sha256;
    vector<list<MerkleNode<T>*>> hashTable; // Hash table for chaining

public:
    MerkleTree() {
        vector<MerkleNode<T>*> nodes;

    }

    template <typename T>
    MerkleTree(const vector<T>& data) {
        vector<MerkleNode<T>*> nodes;

        // Initialize hash table with a fixed number of buckets (e.g., 10 buckets)
        hashTable.resize(10);

        // Creating leaf nodes and adding them to the hash table
        for (int i = 0; i < data.size(); ++i) {
            string valueAsString;
            if constexpr (std::is_same<T, std::string>::value) {
                valueAsString = data[i];
            }
            else {
                valueAsString = std::to_string(data[i]);
            }

            string nodeHash = sha256.compute(valueAsString); // Compute the hash for the data
            MerkleNode<T>* node = new MerkleNode<T>(data[i], sha256); // Create the node

            // Add the node to the hash table (using hash as the key)
            int bucketIndex = getHashBucketIndex(nodeHash);
            hashTable[bucketIndex].push_back(node);

            nodes.push_back(node);  // Add node to the list for tree construction
        }

        // Build the tree by combining nodes
        while (nodes.size() > 1) {
            vector<MerkleNode<T>*> nextLevel;
            for (int i = 0; i < nodes.size(); i += 2) {
                if (i + 1 < nodes.size()) {
                    nextLevel.push_back(new MerkleNode<T>(nodes[i], nodes[i + 1], sha256));
                }
                else {
                    nextLevel.push_back(nodes[i]);
                }
            }
            nodes = nextLevel;
        }

        root = nodes[0];  // The root of the tree
    }

    int getHashBucketIndex(const string& nodeHash) const {
        int hashValue = 0;
        for (char ch : nodeHash) {
            hashValue = (hashValue * 31 + ch) % hashTable.size();
        }
        return hashValue;
    }

    void addToHashTable(MerkleNode<T>* node) {
        int index = std::hash<string>{}(node->hash) % hashTable.size();
        hashTable[index].push_back(node); // Use chaining to handle hash collisions
    }

    MerkleNode<T>* getFromHashTable(const string& hashValue) {
        int index = std::hash<string>{}(hashValue) % hashTable.size();
        for (MerkleNode<T>* node : hashTable[index]) {
            if (node->hash == hashValue) {
                return node;
            }
        }
        return nullptr; // Return nullptr if the node is not found
    }

    void printHashTable() const {
        for (int i = 0; i < hashTable.size(); ++i) {
            const auto& bucket = hashTable[i];
            if (!bucket.empty()) {
                cout << "Hash Chain " << i << ": ";
                for (const MerkleNode<T>* node : bucket) {
                    cout << node->hash << " ";
                }
                cout << endl;
            }
        }
    }

    string getRootHash() const {
        return root ? root->hash : "";
    }

    void printTree(MerkleNode<T>* node, int level = 0) const {
        if (node == nullptr) return;
        printTree(node->right, level + 1);
        cout << string(level * 4, ' ') << node->hash << endl;
        printTree(node->left, level + 1);
    }

    void printTree() const {
        printTree(root);
    }

    template <typename T>
    void CreateTreeFile(MerkleNode<T>* node, const string& activeBranch2, const string& fieldname) {
        if (node == nullptr) {
            return;
        }

        CreateTreeFile(node->left, activeBranch2, fieldname);
        WriteNodeToFile(node, activeBranch2, fieldname);
        CreateTreeFile(node->right, activeBranch2, fieldname);
    }

    template <typename T>
    void GetNodeInfo(fstream& file, MerkleNode<T>* node) {
        if (!node) {
            return;
        }

        if (node->left == nullptr && node->right == nullptr) {
            file << "Leaf Data: " << node->data << "\n";
            file << "Leaf Hash: " << node->hash << "\n";

            for (const auto& address : node->AddressList) {
                file << "Filename: " << address.filename << "\n";
                file << "LineNumber: " << address.linenumber << "\n";
            }
        }
        else {
            file << "Node Hash: " << node->hash << "\n";
            if (node->left) {
                file << "Left Child Hash: " << node->left->hash << "\n";
            }
            else {
                file << "Left Child Hash: NULL\n";
            }

            if (node->right) {
                file << "Right Child Hash: " << node->right->hash << "\n";
            }
            else {
                file << "Right Child Hash: NULL\n";
            }
        }
    }
    template <typename T>
    string GetNodeFilename(MerkleNode<T>* node) {
        return node->hash;
    }

    template <typename T>
    void WriteNodeToFile(MerkleNode<T>* node, const string& activeBranch22, const string& fieldname) {
        if (!node) {
            return;
        }
        fstream file("BRANCHES\\" + activeBranch22 + "\\TREES\\MERKLETREE\\" + fieldname + "\\" + GetNodeFilename(node) + ".txt", ios::out);
        if (!file.is_open()) {
            cerr << "Error opening file to write Merkle Node data." << endl;
            return;
        }

        file << "NodeHash: " << node->hash << "\n";
        if (node->left) {
            file << "LeftChildHash: " << node->left->hash << "\n";
        }
        else {
            file << "LeftChildHash: NULL\n";
        }
        if (node->right) {
            file << "RightChildHash: " << node->right->hash << "\n";
        }
        else {
            file << "RightChildHash: NULL\n";
        }

        if (!node->left && !node->right) {
            for (const auto& address : node->AddressList) {
                file << "Filename: " << address.filename << "\n";
                file << "LineNumber: " << address.linenumber << "\n";
            }
        }

        file << "----\n";
        file.close();
    }

    template <typename T>
    string hashFunction(const T& value) {
        SHA256 sha256;
        string valueAsString;
        if constexpr (std::is_same<T, string>::value) {
            valueAsString = value;
        }
        else {
            valueAsString = std::to_string(value);
        }
        return sha256.compute(valueAsString);
    }

    template <typename T>
    void buildTree(const vector<T>& data) {
        vector<string> currentLevel;

        cout << "Building tree with data: ";
        for (const auto& val : data) {
            cout << val << " ";
        }
        cout << endl;

        for (const auto& val : data) {
            string hash = hashFunction(val);
            cout << "Hash for " << val << ": " << hash << endl;
            currentLevel.push_back(hash);
        }

        while (currentLevel.size() > 1) {
            vector<string> newLevel;
            for (int i = 0; i < currentLevel.size(); i += 2) {
                string combinedHash = (i + 1 < currentLevel.size())
                    ? hashFunction(currentLevel[i] + currentLevel[i + 1])
                    : hashFunction(currentLevel[i]);
                cout << "Combined hash: " << combinedHash << endl;
                newLevel.push_back(combinedHash);
            }
            currentLevel = newLevel;
        }

        string root = currentLevel.empty() ? "" : currentLevel.front();
        cout << "Merkle root: " << root << endl;
    }

};


MerkleTree<string>& createStringMerkleTreeFromCSV(int index, const string& branch) {
    activeBranch5 = branch;
    vector<string> data;

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    if (!file.is_open()) {
        cerr << "Error: File could not be opened!" << endl;
        throw runtime_error("File not found");
    }

    CustomVector<string> lineREAD;
    READLINE(file, lineREAD);
    int lineNumber = 1;

    while (!file.eof()) {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;

        if (index < 0 || index >= lineREAD.getSize()) {
            cerr << "Error: Index out of bounds on line " << lineNumber << endl;
            continue;
        }

        try {
            data.push_back(std::to_string(std::stoi(lineREAD[index])));
        }
        catch (const invalid_argument&) {
            cerr << "Invalid integer value on line " << lineNumber << ": " << lineREAD[index] << endl;
        }
        catch (const out_of_range&) {
            cerr << "Integer value out of range on line " << lineNumber << ": " << lineREAD[index] << endl;
        }
        ++lineNumber;
    }
    file.close();

    //// Debug: Check the data vector
    //cout << "Data extracted for Merkle tree: ";
    //for (const auto& val : data) {
    //    cout << val << " ";
    //}
    //cout << endl;

    //if (data.empty()) {
    //    cerr << "Error: No data available to create the Merkle Tree" << endl;
    //    throw runtime_error("No data for Merkle Tree");
    //}

    MerkleTree<string>* tree = new MerkleTree<string>();
    tree->buildTree(data);
    return *tree;
}

MerkleTree<int>& createIntMerkleTreeFromCSV(int index, const string& branch) {
    activeBranch5 = branch;
    vector<int> data;

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);

    CustomVector<string> lineREAD;
    READLINE(file, lineREAD);
    int lineNumber = 1;

    while (!file.eof()) {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;

        if (index < 0 || index >= lineREAD.getSize()) {
            cerr << "Error: Index out of bounds on line " << lineNumber << endl;
            continue;
        }

        try {
            data.push_back(stoi(lineREAD[index]));
        }
        catch (const invalid_argument&) {
            cerr << "Invalid integer value on line " << lineNumber << ": " << lineREAD[index] << endl;
        }
        catch (const out_of_range&) {
            cerr << "Integer value out of range on line " << lineNumber << ": " << lineREAD[index] << endl;
        }
        ++lineNumber;
    }
    file.close();

    cout << "Data extracted for Merkle tree: ";
    for (const auto& val : data) {
        cout << val << " ";
    }
    cout << endl;

    if (data.empty()) {
        cerr << "Error: No data available to create the Merkle Tree" << endl;
        throw runtime_error("No data for Merkle Tree");
    }

    MerkleTree<int>* tree = new MerkleTree<int>(data);
    return *tree;
}


MerkleTree<double>& createDoubleMerkleTreeFromCSV(int index, const string& branch) {
    activeBranch5 = branch;
    vector<double> data;

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    if (!file.is_open()) {
        cerr << "Error: File could not be opened!" << endl;
        throw runtime_error("File not found");
    }

    CustomVector<string> lineREAD;
    READLINE(file, lineREAD); // Read header line
    int lineNumber = 1;

    while (!file.eof()) {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;

        if (index < 0 || index >= lineREAD.getSize()) {
            cerr << "Error: Index out of bounds on line " << lineNumber << endl;
            continue;
        }

        try {
            data.push_back(stoi(lineREAD[index])); // Add relevant column data
        }
        catch (const invalid_argument&) {
            cerr << "Invalid integer value on line " << lineNumber << ": " << lineREAD[index] << endl;
        }
        catch (const out_of_range&) {
            cerr << "Integer value out of range on line " << lineNumber << ": " << lineREAD[index] << endl;
        }
        ++lineNumber;
    }
    file.close();

    cout << "Data extracted for Merkle tree: ";
    for (const auto& val : data) {
        cout << val << " ";
    }
    cout << endl;

    if (data.empty()) {
        cerr << "Error: No data available to create the Merkle Tree" << endl;
        throw runtime_error("No data for Merkle Tree");
    }

    MerkleTree<double>* tree = new MerkleTree<double>(data);
    return *tree;
}

MerkleNode<string>& ReadMerkleNodeFromFile(const string& filename, SHA256& sha256) {
    // Create a new MerkleNode
    MerkleNode<string>* merkleNode = new MerkleNode<string>("NULL", sha256);

    // Handle case where the filename contains "NULL"
    if (filename.find("NULL") != string::npos)
        return *merkleNode;

    string fullFilename = filename;
    if (filename.find("txt") == string::npos) {
        fullFilename += ".txt";  // Append extension if not present
    }

    fstream file("BRANCHES\\TREES\\MERKLETREE\\" + fullFilename, ios::in);
    if (!file.is_open()) {
        cerr << "Error opening file: " << fullFilename << endl;
        return *merkleNode;
    }

    // Read the file and populate the MerkleNode
    string line;
    CustomVector<string> vtLine;
    READLINE(file, vtLine);

    // Set the root value
    if (!vtLine.empty()) {
        merkleNode->hash = sha256.compute(vtLine[0]);
    }
    else {
        cerr << "File is empty or invalid format: " << fullFilename << endl;
        return *merkleNode;
    }

    // Populate the AddressList
    while (!file.eof()) {
        string fileAddress, lineAddress;

        // Read file address and line number
        if (getline(file, fileAddress) && getline(file, lineAddress)) {
            merkleNode->AddressList.emplace_back(fileAddress, lineAddress);
        }
    }

    file.close();
    return *merkleNode;
}

vector<string> GetTuples(const AddressLocation& adr) {
    vector<string> tuples;
    fstream file("FilesToREAD\\" + adr.filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << adr.filename << endl;
        return tuples;
    }

    int lineNumber = 0;
    int key = stoi(adr.linenumber);
    string line;
    while (getline(file, line)) {
        if (lineNumber == key) {
            tuples.push_back(line);
            break;
        }
        lineNumber++;
    }
    file.close();

    return tuples;
}

template <typename T>
void DisplayAllTuples(const vector<string>& Fields, const MerkleNode<T>& node) {
    for (const auto& address : node.AddressList) {
        vector<string> tuples = GetTuples(address);
        for (int i = 0; i < tuples.size() && i < Fields.size(); i++) {
            cout << left << setw(30) << Fields[i] << ":  " << tuples[i] << endl;
        }
        cout << "---------------------------------------------------------------------------------\n";
    }
}

