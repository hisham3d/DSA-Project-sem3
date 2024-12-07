#pragma once
#include"BTREE.h"
#include "CustomVector.h"
#include <iostream>
using namespace std;

String activeBranch;

void ourNames()
{
	cout << "************************************************************************" << endl;
	cout << endl << endl;
	cout << "                        Hisham Abdullah 23I-0693" << endl;
	cout << "                        Ruhab Ahmed 23I-0559" << endl;
	cout << "                        Daniyal Zaidi 23I-0023" << endl;
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
	cout << "[5] Exit" << endl;
	cout << "Enter your choice: ";
	cin >> option;
	cout << endl << endl;
	return option;
}

template<typename T>
void subMENUBTREE(BTree<T>* IBTREE, CustomVector<String> Fields)
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
		cout << "\nDo you Want to peform again (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

void MENUBTREE(CustomVector<String>Fields, CustomVector<String> DataTypes)
{
	BTree<String>* SBTREE = NULL;
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
void RBSUBMENU(RedBlackTree <T>* IRBTREE, CustomVector<String>& Fields)
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
		String obj;
		cin.ignore();
		cout << "\nENTER THE INDEX YOU WANT TO SEARCH: ";
		getline(cin, obj.toStdString(), '\n');
		PointSearch(&ReadRBNodeFromFile(IRBTREE->fieldname + "\\" + IRBTREE->GetNodeFilename(IRBTREE->root)), obj, Fields, IRBTREE->fieldname);
	}

	if (option == 4)
	{
		String n1, n2;
		cin.ignore();
		cout << "\nENTER THE STARTING VALUE FOR SEARCH: ";
		getline(cin, n1.toStdString(), '\n');
		cin.ignore();
		cout << "\nENTER THE END VALUE FOR SEARCH: ";
		getline(cin, n2.toStdString(), '\n');
		RangeSearch(&ReadRBNodeFromFile(IRBTREE->fieldname + "\\" + IRBTREE->GetNodeFilename(IRBTREE->root)), n1, n2, Fields, IRBTREE->fieldname);
	}
}

void MENURBTREE(CustomVector<String>Fields, CustomVector<String> DataTypes)
{
	RedBlackTree<String>* SRBTREE = NULL;
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
				RBSUBMENU<String>(SRBTREE, Fields);
				/*SRBTREE = &stringCreateRBTree(choice - 1, DataTypes[choice - 1], activeBranch);
				SRBTREE->fieldname = Fields[choice - 1];
				SRBTREE->CreateTreeFile(SRBTREE->root);*/
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
				/*IRBTREE = &intCreateRBTree(choice - 1, DataTypes[choice - 1], activeBranch);
				IRBTREE->fieldname = Fields[choice - 1];
				IRBTREE->CreateTreeFile(IRBTREE->root);*/
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
				/*DRBTREE = &doubleCreateRBTree(choice - 1, DataTypes[choice - 1], activeBranch);
				DRBTREE->fieldname = Fields[choice - 1];
				DRBTREE->CreateTreeFile(DRBTREE->root);*/
				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		cout << "\nDo you Want to run it again for same TREE (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

template<typename T>
void AVLSUBMENU(AVL<T>* IAVLTREE, CustomVector<String>& Fields, int choice)
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
		cout << "\nENTER THE VALUE YOU WANT TO DELETE: ";
		cin >> obj;
		DeleteTuple<T>(IAVLTREE, getFieldIndex(Fields, IAVLTREE->fieldname), obj);
	}

	if (option == 3)
	{
		String obj;
		cin.ignore();
		cout << "\nENTER THE INDEX YOU WANT TO SEARCH: ";
		getline(cin, obj.toStdString(), '\n');
		PointSearch(&ReadAvlNodeFromFile(IAVLTREE->fieldname + "\\" + IAVLTREE->GetNodeFilename(IAVLTREE->root)), obj, Fields, IAVLTREE->fieldname);
	}

	if (option == 4)
	{
		String n1, n2;
		cin.ignore();
		cout << "\nENTER THE STARTING VALUE FOR SEARCH: ";
		getline(cin, n1.toStdString(), '\n');
		cin.ignore();
		cout << "\nENTER THE END VALUE FOR SEARCH: ";
		getline(cin, n2.toStdString(), '\n');
		RangeSearch(&ReadAvlNodeFromFile(IAVLTREE->fieldname + "\\" + IAVLTREE->GetNodeFilename(IAVLTREE->root)), n1, n2, Fields, IAVLTREE->fieldname);
	}
}

void MENUAVLTREE(CustomVector<String>Fields, CustomVector<String> DataTypes)
{
	AVL<String>* SAVLTREE = NULL;
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
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\AVL\\" + Fields[choice - 1]);
			SAVLTREE->fieldname = Fields[choice - 1];
			SAVLTREE->CreateTreeFile(SAVLTREE->root);
			do {
				AVLSUBMENU<String>(SAVLTREE, Fields, choice);
				/*SAVLTREE = &stringCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
				SAVLTREE->fieldname = Fields[choice - 1];
				SAVLTREE->CreateTreeFile(SAVLTREE->root);*/
				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "int")
		{
			IAVLTREE = &intCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\AVL\\" + Fields[choice - 1]);
			IAVLTREE->fieldname = Fields[choice - 1];
			IAVLTREE->CreateTreeFile(IAVLTREE->root);
			do {
				AVLSUBMENU<int>(IAVLTREE, Fields, choice);
				/*IAVLTREE = &intCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
				IAVLTREE->fieldname = Fields[choice - 1];

				IAVLTREE->CreateTreeFile(IAVLTREE->root);*/
				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		if (DataTypes[choice - 1] == "double")
		{
			createFolder("BRANCHES\\" + activeBranch + "\\TREES\\AVL\\" + Fields[choice - 1]);
			DAVLTREE = &doubleCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
			DAVLTREE->fieldname = Fields[choice - 1];
			DAVLTREE->CreateTreeFile(DAVLTREE->root);
			do {
				AVLSUBMENU<double>(DAVLTREE, Fields, choice);
				/*DAVLTREE = &doubleCreateAvlTree(choice - 1, DataTypes[choice - 1], activeBranch);
				DAVLTREE->fieldname = Fields[choice - 1];
				DAVLTREE->CreateTreeFile(DAVLTREE->root);*/

				cout << "\nDo you Want to run it again (Y/N): ";
				cin >> ch;
			} while (ch == 'Y' || ch == 'y');
		}

		cout << "Do you Want to run it again for same TREE (Y/N): ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

void mainMenu(CustomVector<String>& Fields, CustomVector<String>& Entries, CustomVector<String>& DataTypes) {
	system("cls");
	ourNames();
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

	return;
}

int main()
{
	char ch = '1';

	CustomVector<String> Fields, Entries, DataTypes;
	GetFields(fileName, Fields, Entries);
	AllocateDataTypes(DataTypes, Entries);

	do {
		mainMenu(Fields, Entries, DataTypes);
		cout << "Press 1 to return to main menu: ";
		cin >> ch;
	} while (ch == '1');

	return 0;
}