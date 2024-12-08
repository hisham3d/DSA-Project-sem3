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


// Helper function to convert a byte array to a hexadecimal string
string bytesToHex(const unsigned char* bytes, size_t length) {
    static const char* const hexChars = "0123456789abcdef";
    string hexStr;
    for (size_t i = 0; i < length; ++i) {
        hexStr += hexChars[(bytes[i] >> 4) & 0xF];
        hexStr += hexChars[bytes[i] & 0xF];
    }
    return hexStr;
}

// Custom SHA-256 Class
class SHA256 {
public:
    SHA256() { reset(); }

    // Main function to compute SHA-256 hash of input data
    string compute(const string& input) {
        reset();
        update(reinterpret_cast<const unsigned char*>(input.c_str()), input.size());
        finalize();
        return bytesToHex(digest.data(), digest.size());
    }

private:
    array<unsigned char, 64> buffer;  // 64-byte buffer for data being processed
    array<unsigned int, 8> state;     // State of the hash computation
    array<unsigned char, 32> digest;  // Final hash value
    size_t dataLength;                // Length of the data being processed

    // Constants for SHA-256
    static constexpr unsigned int K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0x9b05688c, 0x1f83d9ab, 0x5be0cd19, 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f,
        0x9b05688c, 0x1f83d9ab, 0x5be0cd19, 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f,
    };

    // Initial hash values
    static constexpr unsigned int H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // Helper functions for SHA-256
    void reset() {
        dataLength = 0;
        // Manually copy the values from H to state
        unsigned int H[8] = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
            0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
        };

        for (int i = 0; i < 8; ++i) {
            state[i] = H[i];
        }
        buffer.fill(0);
    }


    void update(const unsigned char* data, size_t length) {
        size_t i = 0;
        while (i < length) {
            buffer[dataLength % 64] = data[i++];
            dataLength++;

            if (dataLength % 64 == 0) {
                processBlock();
            }
        }
    }

    void finalize() {
        size_t index = dataLength % 64;
        buffer[index++] = 0x80;  // Add a single 1 bit (0x80 in hex)
        if (index > 56) {
            while (index < 64) buffer[index++] = 0x00;
            processBlock();
            index = 0;
        }
        while (index < 56) buffer[index++] = 0x00;

        unsigned long long bitLength = dataLength * 8;
        for (int i = 7; i >= 0; --i) {
            buffer[index++] = (bitLength >> (i * 8)) & 0xFF;
        }
        processBlock();
    }

    void processBlock() {
        array<unsigned int, 64> W;
        for (size_t i = 0; i < 16; ++i) {
            W[i] = (buffer[i * 4] << 24) | (buffer[i * 4 + 1] << 16) | (buffer[i * 4 + 2] << 8) | buffer[i * 4 + 3];
        }
        for (size_t i = 16; i < 64; ++i) {
            unsigned int s0 = (W[i - 15] >> 7) | (W[i - 15] << (32 - 7));
            s0 ^= (W[i - 15] >> 18) | (W[i - 15] << (32 - 18));
            s0 ^= (W[i - 15] >> 3);
            unsigned int s1 = (W[i - 2] >> 17) | (W[i - 2] << (32 - 17));
            s1 ^= (W[i - 2] >> 19) | (W[i - 2] << (32 - 19));
            s1 ^= (W[i - 2] >> 10);
            W[i] = W[i - 16] + s0 + W[i - 7] + s1;
        }

        unsigned int a = state[0], b = state[1], c = state[2], d = state[3], e = state[4], f = state[5], g = state[6], h = state[7];

        for (size_t i = 0; i < 64; ++i) {
            unsigned int S1 = (e >> 6) | (e << (32 - 6));
            S1 ^= (e >> 11) | (e << (32 - 11));
            S1 ^= (e >> 25) | (e << (32 - 25));
            unsigned int ch = (e & f) ^ (~e & g);
            unsigned int temp1 = h + S1 + ch + K[i] + W[i];
            unsigned int S0 = (a >> 2) | (a << (32 - 2));
            S0 ^= (a >> 13) | (a << (32 - 13));
            S0 ^= (a >> 22) | (a << (32 - 22));
            unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
            unsigned int temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
        state[5] += f;
        state[6] += g;
        state[7] += h;
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
        std::string valueAsString = toString(data); // Convert data to string
        hash = sha256.compute(valueAsString);       // Compute hash
    }

    // Constructor for creating a parent node by combining the hashes of two child nodes
    MerkleNode(MerkleNode* left, MerkleNode* right, SHA256& sha256) : left(left), right(right) {
        hash = sha256.compute(left->hash + right->hash);
    } // Combine child hashes for parent
    MerkleNode() : left(nullptr), right(nullptr) {}
};


// Template Class representing a Merkle Tree
template <typename T>
class MerkleTree {
public:
    string fieldname;
    MerkleNode<T>* root;
    SHA256 sha256;

public:
    MerkleTree() {
        vector<MerkleNode<T>*> nodes;

        }
    MerkleTree(const CustomVector<T>& data) {
        CustomVector<MerkleNode<T>*> nodes;
        // Create leaf nodes
        for (const auto& datum : data) {
            nodes.push_back(new MerkleNode<T>(datum, sha256));  // Create leaf nodes from data
        }

        // Build the tree by pairing nodes and creating parent nodes
        while (nodes.getSize() > 1) {
            CustomVector<MerkleNode<T>*> nextLevel;
            for (size_t i = 0; i < nodes.getSize(); i += 2) {
                if (i + 1 < nodes.getSize()) {
                    nextLevel.push_back(new MerkleNode<T>(nodes[i], nodes[i + 1], sha256));
                }
                else {
                    // If there is an odd number of nodes, the last one is carried over
                    nextLevel.push_back(nodes[i]);
                }
            }
            nodes = nextLevel;
        }

        root = nodes[0];  // Set the root of the tree
    }

    // Function to get the Merkle root hash
    string getRootHash() const {
        return root ? root->hash : "";
    }

    // Function to print the tree (for debugging purposes)
    void printTree(MerkleNode<T>* node, int level = 0) const {
        if (node == nullptr) return;
        printTree(node->right, level + 1);
        cout << string(level * 4, ' ') << node->hash << endl;
        printTree(node->left, level + 1);
    }

    void printTree() const {
        printTree(root);  // Print the tree starting from the root
    }

    template <typename T>
    void CreateTreeFile(MerkleNode<T>* node, const string& activeBranch2, const string& fieldname) {
        if (node == nullptr) {
            return;
        }

        // Traverse the left subtree
        CreateTreeFile(node->left, activeBranch2, fieldname);

        // Write the current node to the file
        WriteNodeToFile(node, activeBranch2, fieldname);

        // Traverse the right subtree
        CreateTreeFile(node->right, activeBranch2, fieldname);
    }

    template <typename T>
    void GetNodeInfo(fstream& file, MerkleNode<T>* node) {
        if (!node) {
            return;
        }

        // For leaf nodes, print the data and hash
        if (node->left == nullptr && node->right == nullptr) {
            file << "Leaf Data: " << node->data << "\n";
            file << "Leaf Hash: " << node->hash << "\n";

            // Write AddressList for leaf nodes
            for (const auto& address : node->AddressList) {
                file << "Filename: " << address.filename << "\n";
                file << "LineNumber: " << address.linenumber << "\n";
            }
        }
        else {
            // For internal nodes, print the hash and child hashes
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
        // Use the node's hash as the filename for simplicity
        return node->hash; // Customize if needed
    }

    template <typename T>
    void WriteNodeToFile(MerkleNode<T>* node, const string& activeBranch22, const string& fieldname) {
        if (!node) {
            return;
        }
        // Open a file to write node data
        fstream file("BRANCHES\\"+ activeBranch22 +"\\TREES\\MERKLETREE\\" + fieldname + "\\" + GetNodeFilename(node) + ".txt", ios::out);

        // Write the node's hash and child information
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

        // Write AddressList if it exists (for leaf nodes)
        if (!node->left && !node->right) { // This is a leaf node
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
        SHA256 sha256; // Instantiate the SHA256 class
        string valueAsString;
        if constexpr (std::is_same<T, string>::value) {
            valueAsString = value; // If T is already a string
        }
        else {
            valueAsString = std::to_string(value); // Convert numeric values to string
        }
        return sha256.compute(valueAsString); // Convert T to string and compute its SHA-256 hash
    }

    template <typename T>
    void buildTree(const CustomVector<T>& data) {
        vector<string> currentLevel; // Use a local variable for the leaves

        cout << "Building tree with data: ";
        for (const auto& val : data) {
            cout << val << " ";
        }
        cout << endl;

        // Convert data to leaf nodes
        for (const auto& val : data) {
            string hash = hashFunction(val);
            cout << "Hash for " << val << ": " << hash << endl;
            currentLevel.push_back(hash);
        }

        // Build tree levels
        while (currentLevel.size() > 1) {
            vector<string> newLevel;
            for (size_t i = 0; i < currentLevel.size(); i += 2) {
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
    CustomVector<string> data;

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

        data.push_back(lineREAD[index]); // Add relevant column data
        ++lineNumber;
    }
    file.close();

    // Debug: Check the data vector
    cout << "Data extracted for Merkle tree: ";
    for (const auto& val : data) {
        cout << val << " ";
    }
    cout << endl;


    MerkleTree<string>* tree = new MerkleTree<string>();
    tree->buildTree(data);
    return *tree;
}

// Function to create a Merkle tree from a CSV file of integers
MerkleTree<int>& createIntMerkleTreeFromCSV(int index, const string& branch) {
    activeBranch5 = branch;
    CustomVector<int> data;

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);

    CustomVector<string> lineREAD;
    READLINE(file, lineREAD); // Read header line
    int lineNumber = 1;

    while (!file.eof()) {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;

        data.push_back(stoi(lineREAD[index])); // Add relevant column data
        
        ++lineNumber;
    }
    file.close();

    // Debug: Check the data vector
    cout << "Data extracted for Merkle tree: ";
    for (const auto& val : data) {
        cout << val << " ";
    }
    cout << endl;

    MerkleTree<int>* tree = new MerkleTree<int>(data);
    return *tree;
}


// Function to create a Merkle tree from a CSV file of doubles
MerkleTree<double>& createDoubleMerkleTreeFromCSV(int index, const string& branch) {
    activeBranch5 = branch;
    CustomVector<double> data;

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    CustomVector<string> lineREAD;
    READLINE(file, lineREAD);
    int lineNumber = 1;

    // Read the CSV file and extract the column data for the Merkle tree
    while (!file.eof()) {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;

        data.push_back(stod(lineREAD[index])); // Add the relevant data from the CSV to the vector
    }
    file.close();

    // Create a Merkle tree from the extracted data
    MerkleTree<double>* tree = new MerkleTree<double>(data);
    return *tree;
}

MerkleNode<string>&ReadMerkleNodeFromFile(const string & filename, SHA256 & sha256) {
    // Create a new MerkleNode
    MerkleNode<string>* merkleNode = new MerkleNode<string>("NULL", sha256);

    string fullFilename = filename;
    if (filename.find("txt") == string::npos) {
        fullFilename += ".txt";
    }

    fstream file("BRANCHES\\TREES\\MERKLETREE\\" + fullFilename, ios::in);

    // Read the file and populate the MerkleNode
    string line;
    CustomVector<string> vtLine;
    READLINE(file, vtLine);

    // Set the root value
    if (!vtLine.empty()) {
        merkleNode->hash = sha256.compute(vtLine[0]);
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


// Function to get tuples from a file based on AddressLocation
CustomVector<string> GetTuples(const AddressLocation& adr) {
    CustomVector<string> tuples;
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
        for (size_t i = 0; i < tuples.size() && i < Fields.size(); i++) {
            cout << left << setw(30) << Fields[i] << ":  " << tuples[i] << endl;
        }
        cout << "---------------------------------------------------------------------------------\n";
    }
}

