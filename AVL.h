#pragma once
#include <iostream>
#include"FileHandling.h"
using namespace std;

string activeBranch2;
template <class T>
class AVLDataNode 
{
public:
    T value;
    string index;
    AVLDataNode<T>* left;
    AVLDataNode<T>* right;

    AVLDataNode() 
    {
        index = "";
        value = *(new T);
        left = NULL;
        right = NULL;
    }

    vector <AddressLocation> AddressList;
    
    AVLDataNode(T v, string filename, string linenumber, string counter) 
    {
        value = v;
        left = NULL;
        right = NULL;
        index = counter;
        AddressList.push_back(AddressLocation(filename, linenumber));
    }

    string getLeftchildAddress()
    {
        // last index stores the left child 
        return AddressList[AddressList.size() - 1].filename;
    }

    string getRightChildAddress()
    {
        // last index stores the right child
        return AddressList[AddressList.size() - 1].linenumber;
    }
};

template <class T>
class AVL 
{
public:
    int counter;
    string fieldname;
    AVLDataNode<T>* root;

    AVL() 
    {
        root = NULL;
        counter = 0;
    }

    bool isTreeEmpty() 
    {
        return root == NULL;
    }

    int getBalanceFactor(AVLDataNode<T>* n) 
    {
        if (n == NULL)
            return -1;
        return height(n->left) - height(n->right);
    }

    AVLDataNode<T>* rightRotate(AVLDataNode<T>* y) 
    {
        AVLDataNode<T>* x = y->left;
        AVLDataNode<T>* T2 = x->right;
        x->right = y;
        y->left = T2;
        return x;
    }

    AVLDataNode<T>* leftRotate(AVLDataNode<T>* x) 
    {
        AVLDataNode<T>* y = x->right;
        AVLDataNode<T>* T2 = y->left;
        y->left = x;
        x->right = T2;
        return y;
    }

    AVLDataNode<T>* insert(AVLDataNode<T>* r, T value, string val, string filename, string lineNumber) 
    {
        if (r == NULL) 
        {
            r = new AVLDataNode<T>(value, filename, lineNumber, val);
            return r;
        }

        if (value < r->value) 
        {
            r->left = insert(r->left, value, val, filename, lineNumber);
        }
        
        else if (value > r->value) 
        {
            r->right = insert(r->right, value, val, filename, lineNumber);
        }
        
        else 
        {
            r->AddressList.push_back(AddressLocation(filename, lineNumber));
            return r;
        }
        
        int bf = getBalanceFactor(r);

        if (bf > 1 && value < r->left->value)
            return rightRotate(r);
        
        if (bf < -1 && value > r->right->value)
            return leftRotate(r);
        
        if (bf > 1 && value > r->left->value) 
        {
            r->left = leftRotate(r->left);
            return rightRotate(r);
        }

        if (bf < -1 && value < r->right->value) 
        {
            r->right = rightRotate(r->right);
            return leftRotate(r);
        }
        return r;
    }

    AVLDataNode<T>* minValueNode(AVLDataNode<T>* node) 
    {
        AVLDataNode<T>* current = node;
        while (current->left != NULL) 
        {
            current = current->left;
        }
        return current;
    }

    AVLDataNode<T>* deleteNode(AVLDataNode<T>* r, T v) 
    {
        if (r == NULL) 
        {
            return NULL;
        }
        
        else if (v < r->value) 
        {
            r->left = deleteNode(r->left, v);
        }
        
        else if (v > r->value) 
        {
            r->right = deleteNode(r->right, v);
        }
        
        else 
        {
            if (r->left == NULL) 
            {
                AVLDataNode<T>* temp = r->right;
                delete r;
                return temp;
            }

            else if (r->right == NULL) 
            {
                AVLDataNode<T>* temp = r->left;
                delete r;
                return temp;
            }
            
            else 
            {
                AVLDataNode<T>* temp = minValueNode(r->right);
                r->value = temp->value;
                r->right = deleteNode(r->right, temp->value);
            }
        }

        int bf = getBalanceFactor(r);
        
        if (bf == 2 && getBalanceFactor(r->left) >= 0)
            return rightRotate(r);
        
        else if (bf == 2 && getBalanceFactor(r->left) == -1) 
        {
            r->left = leftRotate(r->left);
            return rightRotate(r);
        }
        
        else if (bf == -2 && getBalanceFactor(r->right) <= -0)
            return leftRotate(r);
        
        else if (bf == -2 && getBalanceFactor(r->right) == 1) 
        {
            r->right = rightRotate(r->right);
            return leftRotate(r);
        }
        return r;
    }

    void printInorder(AVLDataNode<T>* r) //  (Left, current node, Right)
    {
        if (r == NULL) 
        {
            return;
        }
        printInorder(r->left);
        cout << r->value << " ";
        printInorder(r->right);
    }

    int height(AVLDataNode<T>* r) 
    {
        if (r == NULL)
            return -1;
        else 
        {
            int lheight = height(r->left);
            int rheight = height(r->right);
            if (lheight > rheight)
                return (lheight + 1);
            else return (rheight + 1);
        }
    }

    string GetNodeFilename(AVLDataNode<T>* R)
    {
        if (R == NULL)
            return "NULL";
        else
            return fieldname + "_AVLNODE_" + R->index;
    }

    void GetNodeInfo(fstream& f, AVLDataNode<T>* R)
    {
        f << R->value << endl;
        for (int i = 0; i < R->AddressList.size(); ++i)
        {
            f << R->AddressList[i].filename << endl;
            f << R->AddressList[i].linenumber << endl;
        }
        f << GetNodeFilename(R->left) << endl;
        f << GetNodeFilename(R->right);
    }

    void WriteNodeinFile(AVLDataNode<T>* R)
    {
        fstream file;
        file.open("BRANCHES\\" + activeBranch2 + "\\TREES\\AVL\\" + fieldname + "\\" + GetNodeFilename(R) + ".txt", ios::out);
        GetNodeInfo(file, R);
        file.close();
    }

    void CreateTreeFile(AVLDataNode<T>* R)
    {
        if (R == NULL)
            return;
        CreateTreeFile(R->left);
        WriteNodeinFile(R);
        CreateTreeFile(R->right);
    }

    AVLDataNode<T>* recursiveSearch(AVLDataNode<T>* r, T val) 
    {
        if (r == NULL || r->value == val)
            return r;
        else if (val < r->value)
            return recursiveSearch(r->left, val);
        else
            return recursiveSearch(r->right, val);
    }
};

AVL<string>& stringCreateAvlTree(int index, string typee, string branch)
{
    activeBranch2 = branch;
    AVL<string>* Tree = new  AVL<string>();

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    vector<string> lineREAD;
    READLINE(file, lineREAD);
    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.size() <= 1)
            break;
        Tree->root = Tree->insert(Tree->root, lineREAD[index], lineREAD[index], fileName, to_string(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

AVL<int>& intCreateAvlTree(int index, string typee, string branch)
{
    activeBranch2 = branch;
    AVL<int>* Tree = new  AVL<int>();

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    vector<string> lineREAD; //
    READLINE(file, lineREAD);
    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.size() <= 1)
            break;
        Tree->root = Tree->insert(Tree->root, stoi(lineREAD[index]), lineREAD[index], fileName, to_string(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

AVL<double>& doubleCreateAvlTree(int index, string typee, string branch)
{
    activeBranch2 = branch;
    AVL<double>* Tree = new  AVL<double>();

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    vector<string> lineREAD; //
    READLINE(file, lineREAD);
    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.size() <= 1)
            break;
        Tree->root = Tree->insert(Tree->root, stod(lineREAD[index]), lineREAD[index], fileName, to_string(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

AVLDataNode<string>& ReadAvlNodeFromFile(string filename)
{
    AVLDataNode<string>* AVLNode = new AVLDataNode<string>();
    AVLNode->value = "NULL";
    if (filename.find("NULL") != string::npos)
        return *AVLNode;
    string line;
    fstream file;
    AVLNode->index = (filename.substr(filename.find_last_of('_') + 1));
    if (filename.find("txt") == string::npos)
        filename = filename + ".txt";
    file.open("BRANCHES\\" + activeBranch2 + "\\TREES\\AVL\\" + filename, ios::in);
    vector<string> vtLine;
    READLINE(file, vtLine);
    //cout << vtLine[0] << endl;
    AVLNode->value = vtLine[0];
    while (!file.eof())
    {
        string f, l;
        getline(file, line);
        f = line;
        getline(file, line);
        l = line;
        AVLNode->AddressList.push_back(AddressLocation(f, l));
    }
    file.close();
    return *AVLNode;
}

vector<string> GetTuples(AddressLocation& adr)
{
    fstream file;
    file.open("FilesToREAD\\" + adr.filename);
    vector<string> tuples;
    if (file.is_open())
    {
        int linenumber = 0;
        int key = stoi(adr.linenumber);
        while (key >= linenumber && !file.eof())
        {
            READLINE(file, tuples);
            linenumber++;
        }
    }
    return tuples;
}

void DisplayAllTuples(vector<string>& Fields, AVLDataNode<string>* ptr)
{
    for (int i = 0; i < ptr->AddressList.size() - 1; i++)
    {
        vector <string > lt = GetTuples(ptr->AddressList[i]);
        for (int i = 0; i < lt.size(); i++)
            cout << left << setw(30) << Fields[i] << ":  " << "                  " << lt[i] << endl;
        cout << "---------------------------------------------------------------------------------\n";
    }
}

void RangeSearch(AVLDataNode<string>* ptr, string start, string end, vector<string>& fields, string fieldname)
{
    if (ptr->value == "NULL")
        return;
    if ((comparestring(start, ptr->value) < 0))
        RangeSearch(&ReadAvlNodeFromFile(fieldname + "\\" + ptr->getLeftchildAddress()), start, end, fields, fieldname);
    if ((comparestring(start, ptr->value) <= 0) && (comparestring(end, ptr->value) >= 0))
        DisplayAllTuples(fields, ptr);
    if ((comparestring(end, ptr->value) > 0))
        RangeSearch(&ReadAvlNodeFromFile(fieldname + "\\" + ptr->getRightChildAddress()), start, end, fields, fieldname);
}

AVLDataNode<string>* PointSearch(AVLDataNode<string>* ptr, string key, vector<string>& fields, string fieldname)
{
    if (ptr->value == "NULL") 
    {
        cout << "\nNot found\n";
        return NULL;
    }
    
    if (key == ptr->value) 
    {
        DisplayAllTuples(fields, ptr);
        return ptr;
    }
    
    if (comparestring(key, ptr->value) < 0)
        return PointSearch(&ReadAvlNodeFromFile(fieldname + "\\" + ptr->getLeftchildAddress()), key, fields, fieldname);

    else
        return PointSearch(&ReadAvlNodeFromFile(fieldname + "\\" + ptr->getRightChildAddress()), key, fields, fieldname);
}

string UpdatedTuple(string tup, string old, string newval)
{
    tup = toLower(tup);
    old = toLower(old);
    newval = toLower(newval);
    return tup.substr(0, tup.find(old)) + newval + tup.substr(tup.find(old) + old.size());
}

template<typename T>
void UpdateTupleInfile(AVLDataNode<T>* ptr, int index, string old, string newVal)
{
    vector<string> tuples;
    for (size_t i = 0; i < ptr->AddressList.size(); ++i)
    {
        string str = "";
        vector<string> temp = GetTuples(ptr->AddressList[i]);
        cout << "CHECK" << endl;
        if (toLower(temp[index]) != toLower(old))
            continue;

        for (size_t j = 0; j < temp.size(); ++j)
            str += "," + temp[j];

        str.erase(0, 1);
        tuples.push_back(str);
    }
    cout << "DONE" << endl;

    stringstream sstream;
    for (int i = 0; i < ptr->AddressList.size(); ++i)
    {
        fstream file;
        string line = "";
        string path = "FilesToREAD\\" + ptr->AddressList[0].filename;
        file.open(path, ios::in);
        while (getline(file, line, '\n'))
        {
            int res = getFieldIndex(tuples, line);
            if (res != i) 
            {
                sstream << line << endl;
            }
            
            else 
            {
                sstream << UpdatedTuple(tuples[i], old, newVal) << endl;
                cout << "Tuple updated" << endl;
            }
        }
        file.close();
        file.open(path, ios::out);
        file << sstream.rdbuf() << endl;
        file.close();
    }
}

void UpdateTuple(AVL<string>*& Avl, vector<string> fields)
{
    string input = "";
    cin.ignore();
    cout << "\nEnter the update query: ";
    getline(cin, input, '\n');
    stringstream sstream(input);

    vector<string> tags;
    while (getline(sstream, input, ','))
    {
        if (input[0] == ' ')
            input.erase(0, 1);
        tags.push_back(input);
    }

    if (tags.size() < 3)
    {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    AVLDataNode<string>* toDelete = Avl->recursiveSearch(Avl->root, tags[1]);
    if (toDelete == NULL) 
    {
        return;
    }
    cout << toDelete->value;
    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[1], tags[2]);
    if (toLower(tags[1]) == toLower(Avl->fieldname))
    {
        cout << "IF CONDITION" << endl;
        Avl = &stringCreateAvlTree(getFieldIndex(fields, Avl->fieldname), "string", activeBranch2);
        Avl->CreateTreeFile(Avl->root);
    }
}

void UpdateTuple(AVL<int>*& Avl, vector<string> fields)
{
    string input = "";
    cin.ignore();
    cout << "\nEnter the update query: ";
    getline(cin, input, '\n');
    stringstream sstream(input);

    vector<string> tags;
    while (getline(sstream, input, ','))
    {
        if (input[0] == ' ')
            input.erase(0, 1);
        tags.push_back(input);
    }

    if (tags.size() < 3)
    {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    AVLDataNode<int>* toDelete = Avl->recursiveSearch(Avl->root, stoi(tags[1]));
    if (toDelete == NULL) 
    {
        cout << "Tuple not found" << endl;
        return;
    }

    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[1], tags[2]);
    if (toLower(tags[1]) == toLower(Avl->fieldname))
    {
        Avl = &intCreateAvlTree(getFieldIndex(fields, Avl->fieldname), "int", activeBranch2);
        Avl->CreateTreeFile(Avl->root);
        cout << "Tree created again" << endl;
    }
}

void UpdateTuple(AVL<double>*& Avl, vector<string> fields)
{
    string input = "";
    cin.ignore();
    cout << "\nEnter the update query: ";
    getline(cin, input, '\n');
    stringstream sstream(input);

    vector<string> tags;
    while (getline(sstream, input, ','))
    {
        if (input[0] == ' ')
            input.erase(0, 1);
        tags.push_back(input);
    }

    if (tags.size() < 4)
    {
        cout << "INCORRECT QUERY" << endl;
        return;
    }
    AVLDataNode<double>* toDelete = Avl->recursiveSearch(Avl->root, stod(tags[0]));
    cout << toDelete->value;
    if (toDelete == NULL)
        return;
    UpdateTupleInfile(toDelete, getFieldIndex(fields, tags[1]), tags[2], tags[3]);
    if (toLower(tags[1]) == toLower(Avl->fieldname))
    {
        Avl = &doubleCreateAvlTree(getFieldIndex(fields, Avl->fieldname), "double", activeBranch2);
        Avl->CreateTreeFile(Avl->root);
    }
}

template<typename T>
void RemoveTupleFromFile(AVLDataNode<T>* ptr)
{
    vector<string> tuples;
    for (size_t i = 0; i < ptr->AddressList.size(); i++)
    {
        string  str = "";
        vector<string> temp = GetTuples(ptr->AddressList[i]);
        for (size_t j = 0; j < temp.size(); j++)
            str += "," + temp[j];
        str.erase(0, 1);
        //cout << str << endl;
        tuples.push_back(str);
    }
    stringstream sstream;
    for (size_t i = 0; i < ptr->AddressList.size(); i++)
    {
        fstream file;
        string line = "";
        string path = "FilesToREAD\\" + ptr->AddressList[0].filename;
        file.open(path, ios::in);
        while (getline(file, line, '\n'))
        {
            int res = getFieldIndex(tuples, line);
            if (res == -1)
                sstream << line << endl;
        }
        file.close();
        file.open(path, ios::out);
        file << sstream.rdbuf();
        file.close();
    }
}

template <typename T>
void DeleteTuple(AVL<T>* Avl, int index, T val)
{
    AVLDataNode<T>* toDelete = Avl->recursiveSearch(Avl->root, val);
    if (toDelete == NULL)
        return;
    RemoveTupleFromFile<T>(toDelete);
    string RemovePath = "BRANCHES\\" + activeBranch2 + "\\TREES\\AVL\\" + Avl->fieldname + "\\" + Avl->GetNodeFilename(toDelete) + ".txt";
    Avl->root = Avl->deleteNode(Avl->root, val);
    if (hasPath(RemovePath))
        remove(RemovePath.c_str());
}