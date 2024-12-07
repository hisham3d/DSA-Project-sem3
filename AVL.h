#pragma once
#include <iostream>
#include "CustomVector.h"
#include"FileHandling.h"
#include "Commit_Log.h"
using namespace std;

String activeBranch2;

template <class T>
class AVLDataNode
{
public:
    T value;
    String index;
    AVLDataNode<T>* left;
    AVLDataNode<T>* right;

    AVLDataNode()
    {
        index = "";
        value = *(new T);
        left = NULL;
        right = NULL;
    }

    CustomVector<AddressLocation> AddressList;

    AVLDataNode(T v, String filename, String linenumber, String counter)
    {
        value = v;
        left = NULL;
        right = NULL;
        index = counter;
        AddressList.push_back(AddressLocation(filename, linenumber));
    }

    String getLeftchildAddress()
    {
        // last index stores the left child 
        return AddressList[AddressList.getSize() - 1].filename;
    }

    String getRightChildAddress()
    {
        // last index stores the right child
        return AddressList[AddressList.getSize() - 1].linenumber;
    }
};

template <class T>
class AVL
{
public:
    int counter;
    String fieldname;
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

    AVLDataNode<T>* insert(AVLDataNode<T>* r, T value, String val, String filename, String lineNumber)
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

    String GetNodeFilename(AVLDataNode<T>* R)
    {
        if (R == NULL)
            return "NULL";
        else
            return fieldname + "_AVLNODE_" + R->index;
    }

    void GetNodeInfo(fstream& f, AVLDataNode<T>* R)
    {
        f << R->value << endl;
        for (int i = 0; i < R->AddressList.getSize(); ++i)
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
        file.open("BRANCHES\\" + activeBranch2.toStdString() + "\\TREES\\AVL\\" + fieldname.toStdString() + "\\" + GetNodeFilename(R).toStdString() + ".txt", ios::out);
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

AVL<String>& stringCreateAvlTree(int index, String typee, String branch)
{
    activeBranch2 = branch;
    AVL<String>* Tree = new  AVL<String>();

    fstream file;
    file.open("FilesToREAD\\" + fileName.toStdString(), ios::in);
    CustomVector<String> lineREAD;
    READLINE(file, lineREAD);
    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;
        Tree->root = Tree->insert(Tree->root, lineREAD[index], lineREAD[index], fileName, tostring(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

AVL<int>& intCreateAvlTree(int index, String typee, String branch)
{
    activeBranch2 = branch;
    AVL<int>* Tree = new  AVL<int>();

    fstream file;
    file.open("FilesToREAD\\" + fileName.toStdString(), ios::in);
    CustomVector<String> lineREAD; //
    READLINE(file, lineREAD);
    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;
        Tree->root = Tree->insert(Tree->root, stoi(lineREAD[index]), lineREAD[index], fileName, tostring(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

AVL<double>& doubleCreateAvlTree(int index, String typee, String branch)
{
    activeBranch2 = branch;
    AVL<double>* Tree = new  AVL<double>();

    fstream file;
    file.open("FilesToREAD\\" + fileName.toStdString(), ios::in);
    CustomVector<String> lineREAD; //
    READLINE(file, lineREAD);
    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;
        Tree->root = Tree->insert(Tree->root, stod(lineREAD[index]), lineREAD[index], fileName, tostring(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

AVLDataNode<String>& ReadAvlNodeFromFile(String filename)
{
    AVLDataNode<String>* AVLNode = new AVLDataNode<String>();
    AVLNode->value = "NULL";
    if (filename.find("NULL") != string::npos)
        return *AVLNode;
    String line;
    fstream file;
    AVLNode->index = (filename.substr(filename.find_last_of('_') + 1));
    if (filename.find("txt") == string::npos)
        filename = filename + ".txt";
    file.open("BRANCHES\\" + activeBranch2.toStdString() + "\\TREES\\AVL\\" + filename.toStdString(), ios::in);
    CustomVector<String> vtLine;
    READLINE(file, vtLine);
    cout << vtLine[0] << endl;
    AVLNode->value = vtLine[0];
    while (!file.eof())
    {
        String f, l;
        getline(file, line.toStdString());
        f = line;
        getline(file, line.toStdString());
        l = line;
        AVLNode->AddressList.push_back(AddressLocation(f, l));
    }
    file.close();
    return *AVLNode;
}

CustomVector<String> GetTuples(AddressLocation& adr)
{
    fstream file;
    file.open("FilesToREAD\\" + adr.filename.toStdString());
    CustomVector<String> tuples;
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

void DisplayAllTuples(CustomVector<String>& Fields, AVLDataNode<String>* ptr)
{
    for (int i = 0; i < ptr->AddressList.getSize() - 1; i++)
    {
        CustomVector <String> lt = GetTuples(ptr->AddressList[i]);
        for (int i = 0; i < lt.getSize(); i++)
            cout << left << setw(30) << Fields[i] << ":  " << "                  " << lt[i] << endl;
        cout << "---------------------------------------------------------------------------------\n";
    }
}

void RangeSearch(AVLDataNode<String>* ptr, String start, String end, CustomVector<String>& fields, String fieldname)
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

AVLDataNode<String>* PointSearch(AVLDataNode<String>* ptr, String key, CustomVector<String>& fields, String fieldname)
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

String UpdatedTuple(String tup, String old, String newval)
{
    tup = toLower(tup);
    old = toLower(old);
    newval = toLower(newval);
    return tup.substr(0, tup.find(old)) + newval + tup.substr(tup.find(old) + old.getsize());
}

template<typename T>
void UpdateTupleInfile(AVLDataNode<T>* ptr, int index, String newVal, int fieldIndex) {
    //index--;

    // Get the file path from the node's AddressList
    String path = "FilesToREAD\\" + ptr->AddressList[0].filename;

    fstream file(path.toStdString(), ios::in);
    CustomVector<String> rows;
    String line;
    while (getline(file, line.toStdString())) {
        rows.push_back(line);
    }
    file.close();

    CustomVector<String> fields = split(rows[index], ','); // split row into field columns
    if (fieldIndex >= 0 && fieldIndex < fields.getSize()) {
        fields[fieldIndex] = newVal; // update the field
        rows[index] = join(fields, ','); // rejoin the updated fields into a row
    }

    file.open(path.toStdString(), ios::out | ios::trunc);
    for (const String& row : rows) {
        file << row << endl;
    }
    file.close();

    cout << "Tuple successfully updated." << endl;
}

void UpdateTuple(AVL<int>*& Avl, CustomVector<String> fields)
{
    int fieldIndex = getFieldIndex(fields, Avl->fieldname);

    String input = "";
    cin.ignore();
    cout << "\nEnter the update query (Serial #, Old value, New value): ";
    getline(cin, input.toStdString(), '\n');
    stringstream sstream(input.toStdString());

    CustomVector<String> tags;
    while (getline(sstream, input.toStdString(), ','))
    {
        if (input[0] == ' ')
            input.erase(0, 1);
        tags.push_back(input);
    }

    if (tags.getSize() < 3)
    {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    AVLDataNode<int>* toDelete = Avl->recursiveSearch(Avl->root, stoi(tags[1]));
    if (toDelete == NULL)
    {
        cout << "Node not found" << endl;
        return;
    }

    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[2], getFieldIndex(fields, Avl->fieldname));

    Avl = &intCreateAvlTree(getFieldIndex(fields, Avl->fieldname), "int", activeBranch2);
    Avl->fieldname = fields[fieldIndex];
    Avl->CreateTreeFile(Avl->root);
    cout << "Tree created again with updated data." << endl;

    CustomVector<String> LogMessage;
    LogMessage.push_back("AVL");

    String concatenatedTags;
    for (int i = 0; i < tags.getSize(); i++) {
        if (i > 0) {
            concatenatedTags += ", ";
        }
        concatenatedTags += tags[i];
    }

    LogMessage.push_back(concatenatedTags);
    addCommit1(activeBranch2, LogMessage);
}

void UpdateTuple(AVL<String>*& Avl, CustomVector<String> fields) {
    int fieldIndex = getFieldIndex(fields, Avl->fieldname);

    String input = "";
    cin.ignore();
    cout << "\nEnter the update query (Serial #, Old value, New value): ";
    getline(cin, input.toStdString(), '\n');
    stringstream sstream(input.toStdString());

    CustomVector<String> tags;
    while (getline(sstream, input.toStdString(), ',')) {
        if (input[0] == ' ')
            input.erase(0, 1);
        tags.push_back(input);
    }

    if (tags.getSize() < 3) {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    AVLDataNode<String>* toDelete = Avl->recursiveSearch(Avl->root, tags[1]);
    if (toDelete == NULL) {
        cout << "Node not found" << endl;
        return;
    }

    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[2], getFieldIndex(fields, Avl->fieldname));

    Avl = &stringCreateAvlTree(getFieldIndex(fields, Avl->fieldname), "string", activeBranch2);
    Avl->fieldname = fields[fieldIndex];
    Avl->CreateTreeFile(Avl->root);
    cout << "Tree created again with updated data." << endl;
}

void UpdateTuple(AVL<double>*& Avl, CustomVector<String> fields) {
    int fieldIndex = getFieldIndex(fields, Avl->fieldname);

    String input = "";
    cin.ignore();
    cout << "\nEnter the update query (Serial #, Old value, New value): ";
    getline(cin, input.toStdString(), '\n');
    stringstream sstream(input.toStdString());

    CustomVector<String> tags;
    while (getline(sstream, input.toStdString(), ',')) {
        if (input[0] == ' ')
            input.erase(0, 1);
        tags.push_back(input);
    }

    if (tags.getSize() < 3) {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    AVLDataNode<double>* toDelete = Avl->recursiveSearch(Avl->root, stod(tags[1]));
    if (toDelete == NULL) {
        cout << "Node not found" << endl;
        return;
    }

    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[2], getFieldIndex(fields, Avl->fieldname));

    Avl = &doubleCreateAvlTree(getFieldIndex(fields, Avl->fieldname), "double", activeBranch2);
    Avl->fieldname = fields[fieldIndex];
    Avl->CreateTreeFile(Avl->root);
    cout << "Tree created again with updated data." << endl;
}

template<typename T>
void RemoveTupleFromFile(AVLDataNode<T>* ptr)
{
    CustomVector<String> tuples;
    for (int i = 0; i < ptr->AddressList.getSize(); i++)
    {
        String  str = "";
        CustomVector<String> temp = GetTuples(ptr->AddressList[i]);
        for (int j = 0; j < temp.getSize(); j++)
            str += "," + temp[j];
        str.erase(0, 1);
        //cout << str << endl;
        tuples.push_back(str);
    }
    stringstream sstream;
    for (int i = 0; i < ptr->AddressList.getSize(); i++)
    {
        fstream file;
        String line = "";
        String path = "FilesToREAD\\" + ptr->AddressList[0].filename;
        file.open(path.toStdString(), ios::in);
        while (getline(file, line.toStdString(), '\n'))
        {
            int res = getFieldIndex(tuples, line);
            if (res == -1)
                sstream << line << endl;
        }
        file.close();
        file.open(path.toStdString(), ios::out);
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
    String RemovePath = "BRANCHES\\" + activeBranch2 + "\\TREES\\AVL\\" + Avl->fieldname + "\\" + Avl->GetNodeFilename(toDelete) + ".txt";
    Avl->root = Avl->deleteNode(Avl->root, val);
    if (hasPath(RemovePath))
        remove(RemovePath.c_str());

    cout << "Node successfully deleted and data deleted from file." << endl;
}