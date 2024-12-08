#pragma once
#include <iostream>
using namespace std;
#include"AVL.h"

string activeBranch3;

template <class T>
class RBDataNode
{
public:
    T value;
    string index;
    int color;
    RBDataNode* parent;
    RBDataNode* left;
    RBDataNode* right;

    RBDataNode()
    {
        index = "";
        value = *(new T);
        left = NULL;
        right = NULL;
        parent = NULL;
    }

    CustomVector <AddressLocation> AddressList;

    RBDataNode(T v, string filename, string linenumber, string counter)
    {
        value = v;
        left = NULL;
        right = NULL;
        parent = NULL;
        index = counter;
        AddressList.push_back(AddressLocation(filename, linenumber));
    }

    string getLeftchildAddress()
    {
        // last index stores the left child 
        return AddressList[AddressList.getSize() - 1].filename;
    }

    string getRightChildAddress()
    {
        // last index stores the right child
        return AddressList[AddressList.getSize() - 1].linenumber;
    }
};

template <typename T>
class RBSUBNODE
{
public:
    T val;
    CustomVector <AddressLocation> AddressList;
    void print()
    {
        for (int i = 0; i < AddressList.getSize(); i++)
            cout << AddressList[i].filename << "  " << AddressList[i].linenumber << endl;
    }

    void printinffile(fstream& file)
    {
        for (int i = 0; i < AddressList.getSize(); i++)
            file << AddressList[i].filename << "\n" << AddressList[i].linenumber << endl;
    }
};

template<typename T>
void DisplayAllTuples(CustomVector<string>& Fields, RBSUBNODE <T>& ptr)
{
    for (int i = 0; i < ptr.AddressList.getSize(); i++)
    {
        CustomVector<string> lt = GetTuples(ptr.AddressList[i]);

        for (int i = 0; i < lt.getSize(); i++)
            cout << left << setw(30) << Fields[i] << ":  " << "                  " << lt[i] << endl;

        cout << "---------------------------------------------------------------------------------\n";
    }
}

template <class T>
class RedBlackTree
{
public:
    string fieldname;
    RBDataNode<T>* root;
    RBDataNode<T>* end;

    string GetNodeFilename(RBDataNode<T>* R)
    {
        if (R == NULL || R == end)
            return "NULL";
        else
            return fieldname + "_RBNODE_" + R->index;
    }

    void GetNodeInfo(fstream& f, RBDataNode<T>* R)
    {
        f << R->value << endl << R->color << endl;
        for (int i = 0; i < R->AddressList.getSize(); i++)
            f << R->AddressList[i].filename << endl << R->AddressList[i].linenumber << endl;
        f << GetNodeFilename(R->left) << endl;
        f << GetNodeFilename(R->right);
    }

    void WriteNodeinFile(RBDataNode<T>* R)
    {
        fstream file;
        file.open("BRANCHES\\" + activeBranch3 + "\\TREES\\RB\\" + fieldname + "\\" + GetNodeFilename(R) + ".txt", ios::out);
        GetNodeInfo(file, R);
        file.close();
    }

    void CreateTreeFile(RBDataNode<T>* R)
    {
        if (R == end)
            return;
        CreateTreeFile(R->left);
        WriteNodeinFile(R);
        CreateTreeFile(R->right);
    }

    // For preorder notation
    void preorder(RBDataNode<T>* n)
    {
        if (n != end)
        {
            cout << n->value << " ";
            preorder(n->left);
            preorder(n->right);
        }
    }

    // For Inorder notation
    void inorder(RBDataNode<T>* n)
    {
        if (n != end)
        {
            inorder(n->left);
            cout << n->value << " ";
            //for (auto S : n->AddressList)
            //    cout << setw(50) << S.filename << "   " << S.linenumber << endl;
            inorder(n->right);
        }
    }

    //  For Postorder notation
    void postorder(RBDataNode<T>* n)
    {
        if (n != end)
        {
            postorder(n->left);
            postorder(n->right);
            cout << n->value << " ";
        }
    }

    RBDataNode<T>* search(RBDataNode<T>* n, T val)
    {
        if (n == end || val == n->value)
        {
            return n;
        }

        if (val < n->value)
        {
            return search(n->left, val);
        }

        return search(n->right, val);
    }

    // For balancing the tree after deletion
    void balance(RBDataNode<T>* n)
    {
        RBDataNode<T>* temp;

        while (n != root && n->color == 0)
        {
            if (n == n->parent->left)
            {
                temp = n->parent->right;

                if (temp->color == 1)
                {
                    temp->color = 0;
                    n->parent->color = 1;
                    leftrot(n->parent);
                    temp = n->parent->right;
                }

                if (temp->left->color == 0 && temp->right->color == 0)
                {
                    temp->color = 1;
                    n = n->parent;
                }

                else
                {
                    if (temp->right->color == 0)
                    {
                        temp->left->color = 0;
                        temp->color = 1;
                        rightrot(temp);
                        temp = n->parent->right;
                    }

                    temp->color = n->parent->color;
                    n->parent->color = 0;
                    temp->right->color = 0;
                    leftrot(n->parent);
                    n = root;
                }
            }

            else
            {
                temp = n->parent->left;

                if (temp->color == 1)
                {
                    temp->color = 0;
                    n->parent->color = 1;
                    rightrot(n->parent);
                    temp = n->parent->left;
                }

                if (temp->right->color == 0 && temp->right->color == 0)
                {
                    temp->color = 1;
                    n = n->parent;
                }

                else
                {
                    if (temp->left->color == 0)
                    {
                        temp->right->color = 0;
                        temp->color = 1;
                        leftrot(temp);
                        temp = n->parent->left;
                    }

                    temp->color = n->parent->color;
                    n->parent->color = 0;
                    temp->left->color = 0;
                    rightrot(n->parent);
                    n = root;
                }
            }
        }

        n->color = 0;
    }

    void rb(RBDataNode<T>* n, RBDataNode<T>* n1)
    {
        if (n->parent == NULL)
        {
            root = n1;
        }

        else if (n == n->parent->left)
        {
            n->parent->left = n1;
        }

        else
        {
            n->parent->right = n1;
        }

        n1->parent = n->parent;
    }

    void deleteRBDataNode(T val)
    {
        RBDataNode<T>* z = end;
        RBDataNode<T>* x;
        RBDataNode<T>* y;
        RBDataNode<T>* n = this->root;

        while (n != end)
        {
            if (n->value == val)
            {
                z = n;
            }

            if (n->value <= val)
            {
                n = n->right;
            }

            else
            {
                n = n->left;
            }
        }

        if (z == end)
        {
            cout << "NOT FOUND" << endl;
            return;
        }

        y = z;
        int y_orgcolor = y->color;

        if (z->left == end)
        {
            x = z->right;
            rb(z, z->right);
        }

        else if (z->right == end)
        {
            x = z->left;
            rb(z, z->left);
        }

        else
        {
            y = min(z->right);
            y_orgcolor = y->color;
            x = y->right;

            if (y->parent == z)
            {
                x->parent = y;
            }

            else
            {
                rb(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rb(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;

        }

        delete z;

        if (y_orgcolor == 0)
        {
            balance(x);
        }

    }

    // For balancing the tree after insertion
    void insertrot(RBDataNode<T>* n)
    {
        RBDataNode<T>* temp;

        while (n->parent->color == 1)
        {
            if (n->parent == n->parent->parent->right)
            {
                temp = n->parent->parent->left;

                if (temp->color == 1)
                {
                    temp->color = 0;
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    n = n->parent->parent;
                }

                else
                {
                    if (n == n->parent->left)
                    {
                        n = n->parent;
                        rightrot(n);
                    }


                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    leftrot(n->parent->parent);
                }

            }

            else
            {
                temp = n->parent->parent->right;

                if (temp->color == 1)
                {
                    temp->color = 0;
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    n = n->parent->parent;
                }

                else
                {
                    if (n == n->parent->right)
                    {
                        n = n->parent;
                        leftrot(n);
                    }

                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    rightrot(n->parent->parent);
                }
            }

            if (n == root)
            {
                break;
            }

        }
        root->color = 0;

    }

    void print(RBDataNode<T>* root, string indent, bool last)
    {
        if (root != end)
        {
            cout << indent;
            if (last)
            {
                cout << "R----";
                indent += "   ";
            }

            else
            {
                cout << "L----";
                indent += "|  ";
            }

            string sColor = root->color ? "RED" : "BLACK";
            cout << root->value << "(" << sColor << ")" << endl;
            print(root->left, indent, false);
            print(root->right, indent, true);
        }
    }

    RedBlackTree()
    {
        end = new RBDataNode<T>;
        end->color = 0;
        end->left = nullptr;
        end->right = nullptr;
        root = end;
        fieldname = "";
    }

    RBDataNode<T>* min(RBDataNode<T>* n)
    {
        while (n->left != end)
        {
            n = n->left;
        }

        return n;
    }

    RBDataNode<T>* max(RBDataNode<T>* n)
    {
        while (n->right != end)
        {
            n = n->right;
        }

        return n;
    }

    RBDataNode<T>* successor(RBDataNode<T>* n)
    {
        if (n->right != end)
        {
            return min(n->right);
        }

        RBDataNode* temp = n->parent;

        while (temp != end && n == temp->right)
        {
            n = temp;
            temp = temp->parent;
        }

        return temp;
    }

    RBDataNode<T>* predecessor(RBDataNode<T>* n)
    {
        if (n->left != end)
        {
            return max(n->left);
        }

        RBDataNode* temp = n->parent;

        while (temp != end && n == temp->left)
        {
            n = temp;
            temp = temp->parent;
        }

        return temp;
    }

    void leftrot(RBDataNode<T>* n)
    {
        RBDataNode<T>* temp = n->right;
        n->right = temp->left;

        if (temp->left != end)
        {
            temp->left->parent = n;
        }

        temp->parent = n->parent;

        if (n->parent == NULL)
        {
            this->root = temp;
        }

        else if (n == n->parent->left)
        {
            n->parent->left = temp;
        }

        else
        {
            n->parent->right = temp;
        }

        temp->left = n;
        n->parent = temp;
    }

    void rightrot(RBDataNode<T>* n)
    {
        RBDataNode<T>* temp = n->left;
        n->left = temp->right;

        if (temp->right != end)
        {
            temp->right->parent = n;
        }

        temp->parent = n->parent;

        if (n->parent == NULL)
        {
            this->root = temp;
        }

        else if (n == n->parent->right)
        {
            n->parent->right = temp;
        }

        else
        {
            n->parent->left = temp;
        }

        temp->right = n;
        n->parent = temp;
    }

    // Inserting a RBDataNode
    void insert(T val, string vals, string filename, string lineNumber)
    {
        RBDataNode<T>* cur = search(root, val);
        if (cur != end && cur != NULL)
        {
            cur->AddressList.push_back(AddressLocation(filename, lineNumber));
            return;
        }

        RBDataNode<T>* n = new RBDataNode<T>(val, filename, lineNumber, vals);

        n->parent = NULL;
        n->value = val;
        n->left = end;
        n->right = end;
        n->color = 1;

        RBDataNode<T>* temp = NULL;
        RBDataNode<T>* temp1 = this->root;

        while (temp1 != end)
        {
            temp = temp1;

            if (n->value < temp1->value)
            {
                temp1 = temp1->left;
            }

            else
            {
                temp1 = temp1->right;
            }
        }

        n->parent = temp;

        if (temp == NULL)
        {
            root = n;
        }

        else if (n->value < temp->value)
        {
            temp->left = n;
        }

        else
        {
            temp->right = n;
        }

        if (n->parent == NULL)
        {
            n->color = 0;
            return;
        }

        if (n->parent->parent == NULL)
        {
            return;
        }
        insertrot(n);
    }

    RBDataNode<T>* getRoot()
    {
        return this->root;
    }

    void printTree()
    {
        if (root)
        {
            print(this->root, "", true);
        }
    }
};

RedBlackTree<string>& stringCreateRBTree(int index, string typee, string branch)
{
    activeBranch3 = branch;
    RedBlackTree<string>* Tree = new RedBlackTree<string>();
    CustomVector<string> lineREAD;

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    READLINE(file, lineREAD);

    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;
        Tree->insert(lineREAD[index], lineREAD[index], fileName, to_string(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

RedBlackTree<int>& intCreateRBTree(int index, string typee, string branch)
{
    activeBranch3 = branch;
    RedBlackTree<int>* Tree = new  RedBlackTree<int>();

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    CustomVector<string> lineREAD;
    READLINE(file, lineREAD);

    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);
        if (lineREAD.getSize() <= 1)
            break;
        Tree->insert(stoi(lineREAD[index]), lineREAD[index], fileName, to_string(lineNumber++));
    }
    file.close();

    return *Tree;
}

RedBlackTree<double>& doubleCreateRBTree(int index, string typee, string branch)
{
    activeBranch3 = branch;
    RedBlackTree<double>* Tree = new  RedBlackTree<double>();

    fstream file;
    file.open("FilesToREAD\\" + fileName, ios::in);
    CustomVector<string> lineREAD;
    READLINE(file, lineREAD);

    int lineNumber = 1;
    while (!file.eof())
    {
        lineREAD.resize(0);
        READLINE(file, lineREAD);

        if (lineREAD.getSize() <= 1)
            break;

        Tree->insert(stod(lineREAD[index]), lineREAD[index], fileName, to_string(lineNumber++));
    }
    //Tree->printInorder(Tree->root);
    file.close();

    return *Tree;
}

RBDataNode<string>& ReadRBNodeFromFile(string filename)
{
    RBDataNode<string>* RBNode = new RBDataNode<string>();
    RBNode->value = "NULL";

    if (filename.find("NULL") != string::npos)
        return *RBNode;

    string line;
    fstream file;
    RBNode->index = (filename.substr(filename.find_last_of('_') + 1));

    if (filename.find("txt") == string::npos)
        filename = filename + ".txt";

    file.open("BRANCHES\\" + activeBranch3 + "\\TREES\\RB\\" + filename, ios::in);
    CustomVector<string> vtLine;
    READLINE(file, vtLine);
    //cout << vtLine[0] << endl;
    RBNode->value = vtLine[0];
    READLINE(file, vtLine);
    RBNode->color = stoi(vtLine[0]);

    while (!file.eof())
    {
        string f, l;
        getline(file, line);
        f = line;
        getline(file, line);
        l = line;
        RBNode->AddressList.push_back(AddressLocation(f, l));
    }

    file.close();
    return *RBNode;
}

void DisplayAllTuples(CustomVector<string>& Fields, RBDataNode<string>* ptr)
{
    for (int i = 0; i < ptr->AddressList.getSize() - 1; i++)
    {
        CustomVector<string> lt = GetTuples(ptr->AddressList[i]);

        for (int i = 0; i < lt.getSize(); i++)
            cout << left << setw(30) << Fields[i] << ":  " << "                  " << lt[i] << endl;

        cout << "---------------------------------------------------------------------------------\n";
    }
}

RBDataNode<string>* PointSearch(RBDataNode<string>* ptr, string key, CustomVector<string>& fields, string fieldname)
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
        return PointSearch(&ReadRBNodeFromFile(fieldname + "\\" + ptr->getLeftchildAddress()), key, fields, fieldname);

    else
        return PointSearch(&ReadRBNodeFromFile(fieldname + "\\" + ptr->getRightChildAddress()), key, fields, fieldname);
}

void RangeSearch(RBDataNode<string>* ptr, string start, string end, CustomVector<string>& fields, string fieldname)
{
    if (ptr->value == "NULL")
        return;

    if ((comparestring(start, ptr->value) < 0))
        RangeSearch(&ReadRBNodeFromFile(fieldname + "\\" + ptr->getLeftchildAddress()), start, end, fields, fieldname);

    if ((comparestring(start, ptr->value) <= 0) && (comparestring(end, ptr->value) >= 0))
        DisplayAllTuples(fields, ptr);

    if ((comparestring(end, ptr->value) > 0))
        RangeSearch(&ReadRBNodeFromFile(fieldname + "\\" + ptr->getRightChildAddress()), start, end, fields, fieldname);
}

template<typename T>
void RemoveTupleFromFile(RBDataNode<T>* ptr)
{
    CustomVector<string> tuples;
    for (int i = 0; i < ptr->AddressList.getSize(); i++)
    {
        string str = "";
        CustomVector<string> temp = GetTuples(ptr->AddressList[i]);

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

template<typename T>
void DeleteTuple(RedBlackTree<T>* RB, int index, T val)
{

    RBDataNode <T>* toDelete = RB->search(RB->root, val);

    if (toDelete == NULL || toDelete == RB->end) {
        cout << "ERROR: Value not found" << endl;
        return;
    }

    RemoveTupleFromFile<T>(toDelete);
    string RemovePath = "BRANCHES\\" + activeBranch3 + "\\TREES\\RB\\" + RB->fieldname + "\\" + RB->GetNodeFilename(toDelete) + ".txt";
    RB->deleteRBDataNode(val);

    if (hasPath(RemovePath))
        remove(RemovePath.c_str());

    cout << "Node successfully deleted and data deleted from file." << endl;
    string valueAsString;
    if constexpr (is_same<T, string>::value) {
        valueAsString = val; // If T is already a string
    }
    else {
        valueAsString = to_string(val); // Convert numeric values to string
    }
    CustomVector<string> LogMessage;
    LogMessage.push_back("AVL");
    LogMessage.push_back(valueAsString);
    LogMessage.push_back("Deleted");
    addCommit1(activeBranch3, LogMessage);
}

template<typename T>
void UpdateTupleInfile(RBDataNode<T>* ptr, int index, string newVal, int fieldIndex) {
    string path = "FilesToREAD\\" + ptr->AddressList[0].filename;

    // read all rows into vector
    fstream file(path, ios::in);
    CustomVector<string> rows;
    string line;
    while (getline(file, line)) {
        rows.push_back(line);
    }
    file.close();

    CustomVector<string> fields = split(rows[index], ','); // split row into field columns
    if (fieldIndex >= 0 && fieldIndex < fields.getSize()) {
        fields[fieldIndex] = newVal; // Update the field value
        rows[index] = join(fields, ','); // Rejoin fields into the updated row
    }

    file.open(path, ios::out | ios::trunc);
    for (const string& row : rows) {
        file << row << endl;
    }
    file.close();

    cout << "Tuple successfully updated." << endl;
}

void UpdateTuple(RedBlackTree<int>*& RB, CustomVector<string> fields) {
    int fieldIndex = getFieldIndex(fields, RB->fieldname);

    string input = "";
    cin.ignore();
    cout << "\nEnter the update query (Serial #, Old value, New value): ";
    getline(cin, input, '\n');
    stringstream sstream(input);

    CustomVector<string> tags;
    while (getline(sstream, input, ',')) {
        if (input[0] == ' ') {
            input.erase(0, 1);
        }
        tags.push_back(input);
    }

    if (tags.getSize() < 3) {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    RBDataNode<int>* toDelete = RB->search(RB->root, stoi(tags[1]));
    if (toDelete == NULL) {
        cout << "Node not found" << endl;
        return;
    }

    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[2], fieldIndex);

    RB = &intCreateRBTree(getFieldIndex(fields, RB->fieldname), "int", activeBranch3);
    RB->fieldname = fields[fieldIndex];
    RB->CreateTreeFile(RB->root);
    cout << "Tree created again with updated data." << endl;

    CustomVector<string> LogMessage;
    LogMessage.push_back("RB");

    string concatenatedTags;
    for (int i = 0; i < tags.getSize(); i++) {
        if (i > 0) {
            concatenatedTags += ", ";
        }
        concatenatedTags += tags[i];
    }

    LogMessage.push_back(concatenatedTags);
    LogMessage.push_back("UPDATED");

    addCommit1(activeBranch3, LogMessage);
}

void UpdateTuple(RedBlackTree<string>*& RB, CustomVector<string> fields) {
    int fieldIndex = getFieldIndex(fields, RB->fieldname);

    string input = "";
    cin.ignore();
    cout << "\nEnter the update query (Serial #, Old value, New value): ";
    getline(cin, input, '\n');
    stringstream sstream(input);

    CustomVector<string> tags;
    while (getline(sstream, input, ',')) {
        if (input[0] == ' ') {
            input.erase(0, 1);
        }
        tags.push_back(input);
    }

    if (tags.getSize() < 3) {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    RBDataNode<string>* toDelete = RB->search(RB->root, tags[1]);
    if (toDelete == NULL) {
        cout << "Node not found" << endl;
        return;
    }

    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[2], fieldIndex);

    RB = &stringCreateRBTree(getFieldIndex(fields, RB->fieldname), "string", activeBranch3);
    RB->fieldname = fields[fieldIndex];
    RB->CreateTreeFile(RB->root);
    cout << "Tree created again with updated data." << endl;

    CustomVector<string> LogMessage;
    LogMessage.push_back("RB");

    string concatenatedTags;
    for (int i = 0; i < tags.getSize(); i++) {
        if (i > 0) {
            concatenatedTags += ", ";
        }
        concatenatedTags += tags[i];
    }

    LogMessage.push_back(concatenatedTags);
    LogMessage.push_back("UPDATED");

    addCommit1(activeBranch3, LogMessage);
}


void UpdateTuple(RedBlackTree<double>*& RB, CustomVector<string> fields) {
    int fieldIndex = getFieldIndex(fields, RB->fieldname);

    string input = "";
    cin.ignore();
    cout << "\nEnter the update query (Serial #, Old value, New value): ";
    getline(cin, input, '\n');
    stringstream sstream(input);

    CustomVector<string> tags;
    while (getline(sstream, input, ',')) {
        if (input[0] == ' ') {
            input.erase(0, 1);
        }
        tags.push_back(input);
    }

    if (tags.getSize() < 3) {
        cout << "INCORRECT QUERY" << endl;
        return;
    }

    RBDataNode<double>* toDelete = RB->search(RB->root, stod(tags[1]));
    if (toDelete == NULL) {
        cout << "Node not found" << endl;
        return;
    }

    UpdateTupleInfile(toDelete, stoi(tags[0]), tags[2], fieldIndex);

    RB = &doubleCreateRBTree(getFieldIndex(fields, RB->fieldname), "double", activeBranch3);
    RB->fieldname = fields[fieldIndex];
    RB->CreateTreeFile(RB->root);
    cout << "Tree created again with updated data." << endl;

    CustomVector<string> LogMessage;
    LogMessage.push_back("RB");

    string concatenatedTags;
    for (int i = 0; i < tags.getSize(); i++) {
        if (i > 0) {
            concatenatedTags += ", ";
        }
        concatenatedTags += tags[i];
    }

    LogMessage.push_back(concatenatedTags);
    LogMessage.push_back("UPDATED");

    addCommit1(activeBranch3, LogMessage);
}

void display_commit_log1() {

    displayCommitChanges(activeBranch3);

}