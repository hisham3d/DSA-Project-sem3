#pragma once
#include"BTREE.h"
#include"MERKLETREE.h"
#include "CustomVector.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

string activeBranch;

void ourNames()
{
	cout << "************************************************************************" << endl;
	cout << endl << endl;
	cout << "                        Hisham Abdullah 23I-0693" << endl;
	cout << "                        Ruhab Ahmed 23I-0559" << endl;
	cout << "                        Daniyal Hussain 23I-0023" << endl;
	cout << endl << endl;
	cout << "************************************************************************" << endl;
	cout << endl << endl;
}

int mainmenu()
{
	int choice;
	cout << "-------------------- MAIN MENU --------------------" << endl;
	cout << endl;
	cout << "Enter the tree you want to use (1-3): " << endl;
	cout << "[1] AVL Trees" << endl;
	cout << "[2] B Trees" << endl;
	cout << "[3] Red Black Trees" << endl;
	cout << "[4] Merkle Trees" << endl;
	cin >> choice;
	cout << endl << endl;
	return choice;
}

int submenu()
{
	system("cls");
	int option;
	cout << "                               Git Commands Available:" << endl;
	cout << endl;
	cout << "choose the function you want to perform (1-5): " << endl;
	cout << "[1] Update" << endl;
	cout << "[2] Delete" << endl;
	cout << "[3] Point Search" << endl;
	cout << "[4] Range Search" << endl;
	cout << "[5] Display Commit changes log" << endl;
	cout << "[6] Display selected tree" << endl;
	cout << "Enter your choice: ";
	cin >> option;
	cout << endl << endl;
	return option;
}

template<typename T>
void subMENUBTREE(BTree<T>* IBTREE, CustomVector<string> Fields)
{
	char ch;
	do
	{
		system("cls");
		int option = submenu();
		if (option == 1)
		{
			cout << "option not availabe for this tree yet\n";
		}

		if (option == 2)
		{
			DeleteTuple(IBTREE, Fields);
		}

		if (option == 3)
		{
			T obj;
			cout << "\nENTER THE INDEX YOU WANT TO SEARCH : ";
			cin >> obj;
			PointSearch(IBTREE, Fields, obj);
		}

		if (option == 4)
		{
			T n1, n2;
			cout << "\nENTER THE STARTING VALUE FOR SEARCH : ";
			cin >> n1;
			cout << "\nENTER THE END VALUE FOR SEARCH : ";
			cin >> n2;
			RangeSearch(IBTREE, Fields, n1, n2);
		}
		if (option == 5) {
			display_commit_log2();
		}
		if (option == 6) {
			IBTREE->print();
		}

		cout << "\nDo you Want to peform again (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

void MENUBTREE(CustomVector<string>Fields, CustomVector<string> DataTypes)
{
	BTree<string>* SBTREE = NULL;
	BTree<int>* IBTREE = NULL;
	BTree<double>* DBTREE = NULL;
	char ch;
	do
	{
		system("cls");
		int choice = 0;
		for (int i = 0; i < Fields.getSize(); i++)
		{
			cout << "[" << i + 1 << "] " << Fields[i] << endl;
		}

		cout << "Enter your choice : ";
		cin >> choice;

		if (DataTypes[choice - 1] == "string")
		{
			SBTREE = &stringCreateBTREE(choice - 1, DataTypes[choice - 1], activeBranch);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\BTREE\\" + Fields[choice - 1]);
			SBTREE->fieldname = Fields[choice - 1];
			SBTREE->CreateTreeFile();
			subMENUBTREE(SBTREE, Fields);
		}

		if (DataTypes[choice - 1] == "int")
		{
			IBTREE = &intCreateBTREE(choice - 1, DataTypes[choice - 1], activeBranch);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\BTREE\\" + Fields[choice - 1]);
			IBTREE->fieldname = Fields[choice - 1];
			IBTREE->CreateTreeFile();
			subMENUBTREE(IBTREE, Fields);
		}

		if (DataTypes[choice - 1] == "double")
		{
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\BTREE\\" + Fields[choice - 1]);
			DBTREE = &doubleCreateBTREE(choice - 1, DataTypes[choice - 1], activeBranch);
			DBTREE->fieldname = Fields[choice - 1];
			DBTREE->CreateTreeFile();
			subMENUBTREE(DBTREE, Fields);
		}

		cout << "\nDo you Want to run it again  for same TREE (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

template<typename T>
void RBSUBMENU(RedBlackTree <T>* IRBTREE, CustomVector<string>& Fields)
{
	system("cls");
	int option = submenu();
	if (option == 1)
	{
		UpdateTuple(IRBTREE, Fields);
	}

	if (option == 2)
	{
		T obj;
		cout << "\nENTER THE VALUE YOU WANT TO DELETE: ";
		cin >> obj;
		DeleteTuple<T>(IRBTREE, getFieldIndex(Fields, IRBTREE->fieldname), obj);
	}

	if (option == 3)
	{
		string obj;
		cin.ignore();
		cout << "\nENTER THE INDEX YOU WANT TO SEARCH: ";
		getline(cin, obj, '\n');
		PointSearch(&ReadRBNodeFromFile(IRBTREE->fieldname + "\\" + IRBTREE->GetNodeFilename(IRBTREE->root)), obj, Fields, IRBTREE->fieldname);
	}

	if (option == 4)
	{
		string n1, n2;
		cin.ignore();
		cout << "\nENTER THE STARTING VALUE FOR SEARCH: ";
		getline(cin, n1, '\n');
		cin.ignore();
		cout << "\nENTER THE END VALUE FOR SEARCH: ";
		getline(cin, n2, '\n');
		RangeSearch(&ReadRBNodeFromFile(IRBTREE->fieldname + "\\" + IRBTREE->GetNodeFilename(IRBTREE->root)), n1, n2, Fields, IRBTREE->fieldname);
	}
	if (option == 5) {
		display_commit_log1();


	}
	if (option == 6) {
		IRBTREE->print(IRBTREE->root, "", true);
	}
}

void MENURBTREE(CustomVector<string>Fields, CustomVector<string> DataTypes)
{
	RedBlackTree<string>* SRBTREE = NULL;
	RedBlackTree<int>* IRBTREE = NULL;
	RedBlackTree<double>* DRBTREE = NULL;
	char ch;
	do
	{
		system("cls");
		int choice = 0;
		for (int i = 0; i < Fields.getSize(); i++)
		{
			cout << "[" << i + 1 << "] " << Fields[i] << endl;
		}

		cout << "Enter your choice: ";
		cin >> choice;

		if (DataTypes[choice - 1] == "string")
		{
			SRBTREE = &stringCreateRBTree(choice - 1, DataTypes[choice - 1], activeBranch);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\RB\\" + Fields[choice - 1]);
			SRBTREE->fieldname = Fields[choice - 1];
			SRBTREE->CreateTreeFile(SRBTREE->root);
			do {
				RBSUBMENU<string>(SRBTREE, Fields);

				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "int")
		{
			IRBTREE = &intCreateRBTree(choice - 1, DataTypes[choice - 1], activeBranch);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\RB\\" + Fields[choice - 1]);
			IRBTREE->fieldname = Fields[choice - 1];
			IRBTREE->CreateTreeFile(IRBTREE->root);
			do {
				RBSUBMENU<int>(IRBTREE, Fields);

				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "double")
		{
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\RB\\" + Fields[choice - 1]);
			DRBTREE = &doubleCreateRBTree(choice - 1, DataTypes[choice - 1], activeBranch);
			DRBTREE->fieldname = Fields[choice - 1];
			DRBTREE->CreateTreeFile(DRBTREE->root);
			do {
				RBSUBMENU<double>(DRBTREE, Fields);

				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		cout << "\nDo you Want to run it again for same TREE (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

template<typename T>
void AVLSUBMENU(AVL<T>* IAVLTREE, CustomVector<string>& Fields, int choice)
{
	int option = submenu();
	system("cls");
	if (option == 1)
	{
		//UpdateTuple(IAVLTREE, Fields, Fields[choice - 1]);
		UpdateTuple(IAVLTREE, Fields);
	}

	if (option == 2)
	{
		T obj;
		cout << "\nEnter value to delete: ";
		cin >> obj;
		DeleteTuple<T>(IAVLTREE, getFieldIndex(Fields, IAVLTREE->fieldname), obj);
	}

	if (option == 3)
	{
		string obj;
		cin.ignore();
		cout << "\nEnter Index to search: ";
		getline(cin, obj, '\n');
		PointSearch(&ReadAvlNodeFromFile(IAVLTREE->fieldname + "\\" + IAVLTREE->GetNodeFilename(IAVLTREE->root)), obj, Fields, IAVLTREE->fieldname);
	}

	if (option == 4)
	{
		string n1, n2;
		cin.ignore();
		cout << "\nEnter starting value to search: ";
		getline(cin, n1, '\n');
		cin.ignore();
		cout << "\nEnter ending value to search: ";
		getline(cin, n2, '\n');
		RangeSearch(&ReadAvlNodeFromFile(IAVLTREE->fieldname + "\\" + IAVLTREE->GetNodeFilename(IAVLTREE->root)), n1, n2, Fields, IAVLTREE->fieldname);
	}
	if (option == 5) {
		display_commit_log();

	}
	if (option == 6) {
		IAVLTREE->printTree(IAVLTREE->root);

	}
}

void MENUAVLTREE(CustomVector<string>Fields, CustomVector<string> DataTypes)
{
	AVL<string>* SAVLTREE = NULL;
	AVL<int>* IAVLTREE = NULL;
	AVL<double>* DAVLTREE = NULL;
	char ch;
	do
	{
		system("cls");
		int choice = 0;

		for (int i = 0; i < Fields.getSize(); i++)
		{
			cout << "[" << i + 1 << "] " << Fields[i] << endl;
		}

		cout << "Enter your choice: ";
		cin >> choice;

		if (DataTypes[choice - 1] == "string")
		{
			SAVLTREE = &stringCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\MERKLETREE\\" + Fields[choice - 1]);
			SAVLTREE->fieldname = Fields[choice - 1];
			SAVLTREE->CreateTreeFile(SAVLTREE->root);
			do {
				AVLSUBMENU<string>(SAVLTREE, Fields, choice);

				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "int")
		{
			IAVLTREE = &intCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\MERKLETREE\\" + Fields[choice - 1]);
			IAVLTREE->fieldname = Fields[choice - 1];
			IAVLTREE->CreateTreeFile(IAVLTREE->root);
			do {
				AVLSUBMENU<int>(IAVLTREE, Fields, choice);

				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "double")
		{
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\MERKLETREE\\" + Fields[choice - 1]);
			DAVLTREE = &doubleCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
			DAVLTREE->fieldname = Fields[choice - 1];
			DAVLTREE->CreateTreeFile(DAVLTREE->root);
			do {
				AVLSUBMENU<double>(DAVLTREE, Fields, choice);

				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		cout << "Do you Want to run it again for same TREE (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

template<typename T>
void MerkleTreeSubMenu(MerkleTree<T>* tree, vector<string>& Fields, int choice)
{
	int option = submenu();
	system("cls");

	if (option == 1)
	{
		cout << "\nMerkle Root: " << tree->getRootHash() << endl;
	}

	if (option == 2)
	{
		cout << "\nDisplaying Merkle Tree..." << endl;
		tree->printTree();
	}

	if (option == 3)
	{
		string obj;
		cin.ignore();
		cout << "\nEnter value to search in tree: ";
		getline(cin, obj, '\n');
	}

	if (option == 4)
	{
		// Option for other actions related to Merkle Tree
		// For example, you might implement range-based logic or hash-based queries
	}

	if (option == 5)
	{
		tree->printHashTable();
		//display_commit_log();
	}
	if (option == 6) {
		tree->printTree(tree->root);
	}
}

void MENUMerkleTree(vector<string>& Fields, vector<string>& DataTypes)
{
	MerkleTree<string>* stringTree = NULL;
	MerkleTree<int>* intTree = NULL;
	MerkleTree<double>* doubleTree = NULL;
	char ch;

	do {
		system("cls");
		int choice = 0;

		for (int i = 0; i < Fields.size(); i++)
		{
			cout << "[" << i + 1 << "] " << Fields[i] << endl;
		}

		cout << "Enter your choice: ";
		cin >> choice;

		if (DataTypes[choice - 1] == "string")
		{
			vector<string> data;  // Populate this with data from your source (CSV, etc.)

			stringTree = &createStringMerkleTreeFromCSV(choice - 1, DataTypes[choice - 1]);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\MERKLETREE\\" + Fields[choice - 1]);
			stringTree->fieldname = Fields[choice - 1];
			stringTree->CreateTreeFile(stringTree->root, activeBranch, Fields[choice - 1]);  // For stringTree
			do {
				MerkleTreeSubMenu<string>(stringTree, Fields, choice);
				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "int")
		{
			vector<int> data;  

			intTree = &createIntMerkleTreeFromCSV(choice - 1, DataTypes[choice - 1]);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\MERKLETREE\\" + Fields[choice - 1]);
			intTree->fieldname = Fields[choice - 1];
			intTree->CreateTreeFile(intTree->root, activeBranch, Fields[choice - 1]);  // For int tree

			do {
				MerkleTreeSubMenu<int>(intTree, Fields, choice);
				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "double")
		{
			vector<double> data;  

			doubleTree = &createDoubleMerkleTreeFromCSV(choice - 1, DataTypes[choice - 1]);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\MERKLETREE\\" + Fields[choice - 1]);
			doubleTree->fieldname = Fields[choice - 1];
			doubleTree->CreateTreeFile(doubleTree->root, activeBranch, Fields[choice - 1]);
			do {
				MerkleTreeSubMenu<double>(doubleTree, Fields, choice);
				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		cout << "Do you Want to run it again for same TREE (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}


void mainMenu(CustomVector<string>& Fields, CustomVector<string>& Entries, CustomVector<string>& DataTypes) {
	system("cls");
	ourNames();

	cout << "VERSION: " << to_string(getLatestVersion()) << endl;

	cout << "------------------- BRANCHES --------------------" << endl;
	activeBranch = "main";
	activeBranch = branchSelection(activeBranch);


	int treechoice = mainmenu();
	if (treechoice == 1)
	{
		MENUAVLTREE(Fields, DataTypes);
	}

	else if (treechoice == 2)
	{
		MENUBTREE(Fields, DataTypes);
	}

	else if (treechoice == 3)
	{
		MENURBTREE(Fields, DataTypes);
	}
	else if (treechoice == 4) {

		vector<string> stdFields;
		for (int i = 0; i < Fields.getSize(); ++i) {
			stdFields.push_back(Fields[i]);
		}

		vector<string> stdDataTypes;
		for (int i = 0; i < DataTypes.getSize(); ++i) {
			stdDataTypes.push_back(DataTypes[i]);
		}

		MENUMerkleTree(stdFields, stdDataTypes);
	}

	return;
}

int main()
{
	//createFile("FilesToREAD\\healthcare_dataset.txt");
	UpdateDataFile();
	char ch = '1';

	CustomVector<string> Fields, Entries, DataTypes;
	GetFields(fileName, Fields, Entries);
	AllocateDataTypes(DataTypes, Entries);

	do {
		mainMenu(Fields, Entries, DataTypes);
		cout << "Press 1 to return to main menu: ";
		cin >> ch;
	} while (ch == '1');

	UpdateCsvFile();

	return 0;
}