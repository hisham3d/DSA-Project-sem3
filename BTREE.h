#pragma once
#include<iostream>
#include"RB.h"
using namespace std;

string activeBranch4;

template <typename T>
class BTREEDataNode
{
public:
	BTREEDataNode<T>** child;	// Array of pointers to children.
	RBSUBNODE<T>* key;				// Array of keys
	int counter;
	int  size;		// Number of keys.
	bool leaf;			// Whether the node is a leaf.
};

template <typename T>
class BTree
{
public:
	BTREEDataNode<T>* root;
	int counter;
	string fieldname;
	BTree(int);
	~BTree<T>();
	void insert(T, string filename, string linenumber);
	T remove(T);
	std::pair<BTREEDataNode<T>*, int> search(T);
	void print();

	void INRANGEROOT(BTREEDataNode<T>* node, CustomVector<string> fields, T start, T end)
	{
		for (int i = 0; i < node->size; i++)
		{
			if (node->key[i].val >= start && node->key[i].val <= end)
				DisplayAllTuples<T>(fields, node->key[i]);
		}

		if (!node->leaf)
		{
			for (int i = 0; i <= node->size; i++)
			{
				INRANGEROOT(node->child[i], fields, start, end);
			}
		}
	}

	string GetFileName(BTREEDataNode<T>* node)
	{
		return fieldname + "_BTREENODE_" + to_string(node->counter);
	}

	void printNodeinfile(BTREEDataNode<T>* node)
	{
		fstream file;
		file.open("BRANCHES\\" + activeBranch4 + "\\TREES\\BTREE\\" + fieldname + "\\" + GetFileName(node) + ".txt", ios::out);
		for (int i = 0; i < node->size; i++)
		{
			file << node->key[i].val << ",";
		}

		file << endl;

		for (int i = 0; i < node->size; i++)
		{
			node->key[i].printinffile(file);
		}

		if (!node->leaf)
		{
			for (int i = 0; i <= node->size; i++)
			{
				file << GetFileName(node->child[i]) << endl;
			}
		}

		file.close();

		if (!node->leaf)
		{
			for (int i = 0; i <= node->size; i++)
			{
				printNodeinfile(node->child[i]);
			}
		}
	}

	void CreateTreeFile()
	{
		if (root != 0 && root != NULL)
		{
			printNodeinfile(root);
		}
	}

private:
	// Used for initializing nodes.
	void initializeNode(BTREEDataNode<T>*);

	// Recursive function called by destructor.
	void freeNode(BTREEDataNode<T>*);

	// Finds the index of a key in a node.
	int  findIndex(BTREEDataNode<T>*, T);

	// Inserts a key into a node.
	int  nodeInsert(BTREEDataNode<T>*, T, CustomVector<AddressLocation>&);

	// Deletes the key at a given index from a node.
	T nodeDelete(BTREEDataNode<T>*, int);

	// Function for splitting nodes that are too full.
	void splitChild(BTREEDataNode<T>*, int);

	// Merges two children of a node at a given index into one child.
	char mergeChildren(BTREEDataNode<T>*, int);

	// Makes sure the child of a node at a specified index has >= minDegree items.
	char fixChildSize(BTREEDataNode<T>*, int);

	// Recursively prints a subtree.
	void printNode(BTREEDataNode<T>*, int);

	// Minimum degree of the tree.
	int  minDegree;
};

// Constructor for b tree.
// t is the minimum degree of the tree.
// compare is the comparison function used for managing elements within the tree.
// printK is a function that prints keys.
template <typename T>
BTree<T>::BTree(int  t)
{
	counter = 0;
	minDegree = t;
	root = new BTREEDataNode<T>;
	initializeNode(root);
	root->leaf = true;
	root->counter = counter++;
}

template <typename T>
BTree<T>::~BTree<T>()
{
	freeNode(root);
}

template <typename T>
void BTree<T>::insert(T k, string filename, string linenumber)
{
	pair<BTREEDataNode<T>*, int > res = search(k);
	if (res.first != 0 && res.first != NULL)
	{
		res.first->key[res.second].AddressList.push_back(AddressLocation(filename, linenumber));
		return;
	}

	// Grow upwards if the root is full.
	if (root->size == 2 * minDegree - 1)
	{
		BTREEDataNode<T>* newRoot = new BTREEDataNode<T>();
		initializeNode(newRoot);
		newRoot->leaf = false;
		newRoot->child[0] = root;
		root = newRoot;
		root->counter = counter++;
		splitChild(newRoot, 0);
	}

	// Work down the tree.
	BTREEDataNode<T>* curr = root;
	while (!curr->leaf)
	{

		// Find the proper child to go to.
		int index = curr->size - 1;
		while (index >= 0 && k < curr->key[index].val)
		{
			index--;
		}
		index++;

		// Split child if full.
		if (curr->child[index]->size == 2 * minDegree - 1)
		{
			splitChild(curr, index);
			if ((curr->key[index].val < k))
			{
				index++;
			}
		}
		curr = curr->child[index];
	}
	CustomVector<AddressLocation> adr;
	adr.push_back(AddressLocation(filename, linenumber));
	nodeInsert(curr, k, adr);
}

template <typename T>
T BTree<T>::remove(T k)
{
	BTREEDataNode<T>* curr = root;
	while (true)
	{
		int  i = findIndex(curr, k);
		if (i < curr->size && !((curr->key[i].val < k) || (k < curr->key[i].val)))
		{
			T toReturn = curr->key[i].val;
			if (curr->leaf)
			{
				nodeDelete(curr, i);
			}

			else
			{
				BTREEDataNode<T>* leftKid = curr->child[i];
				BTREEDataNode<T>* rightKid = curr->child[i + 1];
				if (leftKid->size >= minDegree)
				{
					while (!(leftKid->leaf))
					{
						fixChildSize(leftKid, leftKid->size);
						leftKid = leftKid->child[leftKid->size];
					}
					curr->key[i].val = nodeDelete(leftKid, leftKid->size - 1);
				}

				else if (rightKid->size >= minDegree)
				{
					while (!(rightKid->leaf))
					{
						fixChildSize(rightKid, 0);
						rightKid = rightKid->child[0];
					}
					curr->key[i].val = nodeDelete(rightKid, 0);
				}

				else
				{
					mergeChildren(curr, i);
					curr = leftKid;
					continue;
				}
			}
			return toReturn;
		}

		else
		{
			if (curr->leaf)
			{
				return *(new T);
			}

			char result = fixChildSize(curr, i);

			if (result == 2)
			{
				curr = root;
			}

			else
			{
				curr = curr->child[findIndex(curr, k)];
			}
		}
	}
}

template <typename T>
pair<BTREEDataNode<T>*, int > BTree<T>::search(T k)
{
	BTREEDataNode<T>* x = root;
	while (true)
	{
		int  i = findIndex(x, k);

		if (i < x->size && !((k < x->key[i].val) || (x->key[i].val < k)))
		{
			return pair<BTREEDataNode<T>*, int >(x, i);
		}

		else if (x->leaf)
		{
			return pair<BTREEDataNode<T>*, int >(0, 1);
		}

		else
		{
			x = x->child[i];
		}
	}
}

template <typename T>
void BTree<T>::print()
{
	if (root != 0 && root != NULL)
	{
		cout << "\n";
		printNode(root, 0);
		cout << ("\n");
	}
}

template <typename T>
void BTree<T>::printNode(BTREEDataNode<T>* node, int tab) {
	// Print indentation for the current level
	for (int i = 0; i < tab; i++) {
		cout << "\t";
	}

	// Print the node keys enclosed in vertical bars '|'
	cout << "|";
	for (int i = 0; i < node->size; i++) {
		cout << node->key[i].val << "|";
	}
	cout << "\n";

	// If the node is not a leaf, recursively print its children
	if (!node->leaf) {
		tab++; // Increase indentation for child nodes
		for (int i = 0; i <= node->size; i++) {
			printNode(node->child[i], tab);
		}
	}
}


template <typename T>
void BTree<T>::initializeNode(BTREEDataNode<T>* x)
{
	x->size = 0;
	x->key = new RBSUBNODE<T>[(2 * minDegree - 1)];
	x->child = new BTREEDataNode<T>*[2 * minDegree];
	x->counter = counter++;
}

template <typename T>
void BTree<T>::freeNode(BTREEDataNode<T>* x)
{
	if (!x->leaf)
	{
		for (int i = 0; i <= x->size; i++)
		{
			freeNode(x->child[i]);
		}
	}

	string path = "BRANCHES\\" + activeBranch4 + "\\TREES\\BTREE\\" + fieldname + "\\" + GetFileName(x);
	remove(path.c_str());
	delete[] x->child;
}

template <typename T>
int  BTree<T>::findIndex(BTREEDataNode<T>* x, T k)
{
	int  i = 0;
	while (i < x->size && (x->key[i].val < k))
	{
		i++;
	}
	return i;
}

void copyVect(CustomVector<AddressLocation>& vt1, CustomVector<AddressLocation>& vt2)
{
	vt1.resize(0);
	for (int i = 0; i < vt2.getSize(); ++i)
	{
		string s1 = vt2[i].filename;
		string s2 = vt2[i].linenumber;
		vt1.push_back(AddressLocation(s1, s2));
	}
}

template <typename T>
int  BTree<T>::nodeInsert(BTREEDataNode<T>* x, T k, CustomVector<AddressLocation>& list)
{
	int index;
	for (index = x->size; index > 0 && (k < x->key[index - 1].val); index--)
	{
		x->key[index].val = x->key[index - 1].val;
		copyVect(x->key[index].AddressList, x->key[index - 1].AddressList);

		x->child[index + 1] = x->child[index];
	}
	x->child[index + 1] = x->child[index];
	x->key[index].val = k;
	copyVect(x->key[index].AddressList, list);
	x->size++;
	return index;
}

template <typename T>
T BTree<T>::nodeDelete(BTREEDataNode<T>* x, int  index)
{
	T toReturn = x->key[index].val;
	x->size--;
	while (index < x->size)
	{
		x->key[index].val = x->key[index + 1].val;
		x->key[index].AddressList = x->key[index + 1].AddressList;
		x->child[index + 1] = x->child[index + 2];
		index++;
	}
	return toReturn;
}

template <typename T>
void BTree<T>::splitChild(BTREEDataNode<T>* x, int i)
{
	BTREEDataNode<T>* toSplit = x->child[i];
	BTREEDataNode<T>* newNode = new BTREEDataNode<T>;
	newNode->counter = counter++;
	initializeNode(newNode);
	newNode->leaf = toSplit->leaf;
	newNode->size = minDegree - 1;
	for (int j = 0; j < minDegree - 1; j++)
	{
		newNode->key[j].val = toSplit->key[j + minDegree].val;
		newNode->key[j].AddressList = toSplit->key[j + minDegree].AddressList;
	}

	if (!toSplit->leaf)
	{
		for (int j = 0; j < minDegree; j++)
		{
			newNode->child[j] = toSplit->child[j + minDegree];
		}
	}
	toSplit->size = minDegree - 1;
	nodeInsert(x, toSplit->key[minDegree - 1].val, toSplit->key[minDegree - 1].AddressList);
	x->child[i + 1] = newNode;
}

template <typename T>
char BTree<T>::mergeChildren(BTREEDataNode<T>* parent, int  i)
{
	BTREEDataNode<T>* leftKid = parent->child[i];
	BTREEDataNode<T>* rightKid = parent->child[i + 1];
	leftKid->key[leftKid->size].val = nodeDelete(parent, i);
	int  j = ++(leftKid->size);

	for (int k = 0; k < rightKid->size; k++)
	{
		leftKid->key[j + k].val = rightKid->key[k].val;
		leftKid->key[j + k].AddressList = rightKid->key[k].AddressList;

		leftKid->child[j + k] = rightKid->child[k];
	}

	leftKid->size += rightKid->size;
	leftKid->child[leftKid->size] = rightKid->child[rightKid->size];
	free(rightKid->child);
	free(rightKid->key);
	free(rightKid);

	if (parent->size == 0)
	{
		root = leftKid;
		free(parent->child);
		free(parent->key);
		free(parent);
		return 2;
	}

	return 1;
}

template <typename T>
char BTree<T>::fixChildSize(BTREEDataNode<T>* parent, int  index)
{
	BTREEDataNode<T>* kid = parent->child[index];

	if (kid->size < minDegree)
	{

		if (index != 0 && parent->child[index - 1]->size >= minDegree)
		{
			BTREEDataNode<T>* leftKid = parent->child[index - 1];
			for (int i = nodeInsert(kid, parent->key[index - 1].val); i != 0; i--)
			{
				kid->child[i] = kid->child[i - 1];
			}
			kid->child[0] = leftKid->child[leftKid->size];
			parent->key[index - 1].val = nodeDelete(leftKid, leftKid->size - 1);
		}

		else if (index != parent->size && parent->child[index + 1]->size >= minDegree)
		{
			BTREEDataNode<T>* rightKid = parent->child[index + 1];
			nodeInsert(kid, parent->key[index].val, parent->key[index].AddressList);
			kid->child[kid->size] = rightKid->child[0];
			rightKid->child[0] = rightKid->child[1];
			parent->key[index].val = nodeDelete(rightKid, 0);
		}

		else if (index != 0)
		{
			return mergeChildren(parent, index - 1);
		}

		else
		{
			return mergeChildren(parent, index);
		}

		return 1;
	}
	return 0;
}


BTree<string>& stringCreateBTREE(int index, string typee, string branch)
{
	activeBranch4 = branch;
	int degree;
	cout << "Enter the minimum degree : ";
	cin >> degree;
	BTree<string>* Tree = new  BTree<string>(degree);

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
		Tree->insert(lineREAD[index], fileName, to_string(lineNumber++));
	}
	//Tree->printInorder(Tree->root);
	file.close();

	return *Tree;
}

BTree<int>& intCreateBTREE(int index, string typee, string branch)
{
	activeBranch4 = branch;
	int degree;
	cout << "Enter the minimum degree : ";
	cin >> degree;
	BTree<int>* Tree = new  BTree<int>(degree);

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
		Tree->insert(stod(lineREAD[index]), fileName, to_string(lineNumber++));
	}
	//Tree->printInorder(Tree->root);
	file.close();

	return *Tree;
}

BTree<double>& doubleCreateBTREE(int index, string typee, string branch)
{
	activeBranch4 = branch;
	int degree;
	cout << "Enter the minimum degree : ";
	cin >> degree;
	BTree<double>* Tree = new  BTree<double>(degree);

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
		Tree->insert(stod(lineREAD[index]), fileName, to_string(lineNumber++));
	}
	//Tree->printInorder(Tree->root);
	file.close();

	return *Tree;
}

template<typename T>
void RemoveTupleFromFile(RBSUBNODE<T>* ptr, int index, string valToDel)
{
	CustomVector<string> tuples;
	set<int> indexis;
	int i = 0;

	// Iterate through ptr->AddressList using an index-based for loop
	for (int i = 0; i < ptr->AddressList.getSize(); ++i)
	{
		string str = "";
		CustomVector<string> temp = GetTuples(ptr->AddressList[i]);
		if (toLower(temp[index]) != toLower(valToDel))
			continue;

		indexis.insert(i);
		for (int j = 0; j < temp.getSize(); ++j)
			str += "," + temp[j];

		str.erase(0, 1);
		tuples.push_back(str);
	}

	stringstream sstream;
	for (int i = 0; i < ptr->AddressList.getSize(); ++i)
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

	CustomVector<AddressLocation> tempo;
	for (int i = 0; i < ptr->AddressList.getSize(); ++i)
	{
		if (indexis.find(i) != indexis.end())
			continue;
		tempo.push_back(ptr->AddressList[i]);
	}
	copyVect(ptr->AddressList, tempo);
}

void DeleteTuple(BTree<int>* BT, CustomVector<string> fields)
{
	string input = "";
	cout << "\nEnter Delete Query: ";
	cin.ignore();
	getline(cin, input, '\n');
	stringstream sstream;
	sstream << input;
	sstream >> input;
	cout << input << endl;
	CustomVector<string> tags;
	while (getline(sstream, input, ','))
	{
		if (input[0] == ' ')
			input.erase(0, 1);
		tags.push_back(input);
	}

	pair<BTREEDataNode<int>*, int> toDelete = BT->search(stoi(tags[0]));
	RBSUBNODE<int>* ptr = &toDelete.first->key[toDelete.second];
	int ind = getFieldIndex(fields, tags[1]);

	if (ind == -1)
	{
		cout << "INCORRECT QUERRY\n";
		return;
	}

	if (toDelete.first == NULL)
	{
		cout << "TUPLES NOT FOUND\n";
		return;
	}

	RemoveTupleFromFile<int>(ptr, ind, tags[2]);

	if (toLower(tags[1]) == toLower(BT->fieldname))
	{
		string path = "BRANCHES\\" + activeBranch4 + "\\TREES\\BTREE\\" + BT->fieldname + "\\" + BT->GetFileName(toDelete.first);
		remove(path.c_str());
		BT = &intCreateBTREE(getFieldIndex(fields, BT->fieldname), "", activeBranch4);
		BT->CreateTreeFile();
	}

	CustomVector<string> LogMessage;
	LogMessage.push_back("BTREE");

	string concatenatedTags;
	for (int i = 0; i < tags.getSize(); i++) {
		if (i > 0) {
			concatenatedTags += ", ";
		}
		concatenatedTags += tags[i];
	}

	LogMessage.push_back(concatenatedTags);
	LogMessage.push_back("DELETED");

	addCommit1(activeBranch4, LogMessage);
}

void DeleteTuple(BTree<string>* BT, CustomVector<string> fields)
{
	string input = "";
	cout << "\nEnter Delete Query: ";
	cin.ignore();
	getline(cin, input, '\n');
	stringstream sstream;
	sstream << input;
	sstream >> input;
	cout << input << endl;
	CustomVector<string> tags;

	while (getline(sstream, input, ','))
	{
		if (input[0] == ' ')
			input.erase(0, 1);
		tags.push_back(input);
	}

	pair<BTREEDataNode<string>*, int> toDelete = BT->search((tags[0]));
	RBSUBNODE<string>* ptr = &toDelete.first->key[toDelete.second];

	int ind = getFieldIndex(fields, tags[1]);
	if (ind == -1)
	{
		cout << "INCORRECT QUERRY\n";
		return;
	}

	if (toDelete.first == NULL)
	{
		cout << "TUPLES NOT FOUND\n";
		return;
	}

	RemoveTupleFromFile<string>(ptr, ind, tags[2]);

	if (toLower(tags[1]) == toLower(BT->fieldname))
	{
		string path = "BRANCHES\\" + activeBranch4 + "\\TREES\\BTREE\\" + BT->fieldname + "\\" + BT->GetFileName(toDelete.first);
		remove(path.c_str());
		BT = &stringCreateBTREE(getFieldIndex(fields, BT->fieldname), "", activeBranch4);
		BT->CreateTreeFile();
	}

	CustomVector<string> LogMessage;
	LogMessage.push_back("BTREE");

	string concatenatedTags;
	for (int i = 0; i < tags.getSize(); i++) {
		if (i > 0) {
			concatenatedTags += ", ";
		}
		concatenatedTags += tags[i];
	}

	LogMessage.push_back(concatenatedTags);
	LogMessage.push_back("DELETED");

	addCommit1(activeBranch4, LogMessage);
}

void DeleteTuple(BTree<double>* BT, CustomVector<string> fields)
{
	string input = "";
	cout << "\nEnter Delete Query: ";
	cin.ignore();
	getline(cin, input, '\n');
	stringstream sstream;
	sstream << input;
	sstream >> input;
	cout << input << endl;
	CustomVector<string> tags;
	while (getline(sstream, input, ','))
	{
		if (input[0] == ' ')
			input.erase(0, 1);
		tags.push_back(input);
	}

	if (tags.getSize() < 4)
	{
		cout << "INCORRECT QUERRY\n";
		return;
	}

	pair<BTREEDataNode<double>*, int> toDelete = BT->search(stoi(tags[0]));
	RBSUBNODE<double>* ptr = &toDelete.first->key[toDelete.second];

	int ind = getFieldIndex(fields, tags[1]);
	if (ind == -1)
	{
		cout << "INCORRECT QUERRY\n";
		return;
	}

	if (toDelete.first == NULL)
	{
		cout << "TUPLES NOT FOUND\n";
		return;
	}

	RemoveTupleFromFile<double>(ptr, ind, tags[2]);

	if (toLower(tags[1]) == toLower(BT->fieldname))
	{
		string path = "BRANCHES\\" + activeBranch4 + "\\TREES\\BTREE\\" + BT->fieldname + "\\" + BT->GetFileName(toDelete.first);
		remove(path.c_str());
		BT = &doubleCreateBTREE(getFieldIndex(fields, BT->fieldname), "", activeBranch4);
		BT->CreateTreeFile();
	}
	CustomVector<string> LogMessage;
	LogMessage.push_back("BTREE");

	string concatenatedTags;
	for (int i = 0; i < tags.getSize(); i++) {
		if (i > 0) {
			concatenatedTags += ", ";
		}
		concatenatedTags += tags[i];
	}

	LogMessage.push_back(concatenatedTags);
	LogMessage.push_back("DELETED");

	addCommit1(activeBranch4, LogMessage);
}

void display_commit_log2() {

	displayCommitChanges(activeBranch4);

}


template<typename T>
void PointSearch(BTree<T>* R, CustomVector<string> fields, T key)
{
	pair<BTREEDataNode<T>*, int> res = R->search(key);
	if (!res.first)
		return;
	DisplayAllTuples<T>(fields, res.first->key[res.second]);
}

template<typename T>
void RangeSearch(BTree<T>* R, CustomVector<string> fields, T start, T end)
{
	R->INRANGEROOT(R->root, fields, start, end);
}