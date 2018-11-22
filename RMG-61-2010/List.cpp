#include "pch.h"

#include <windows.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <clocale>
#include <cstdlib>
#include <cmath>
#include <stdarg.h>
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;

#include "List.h"

int List::commonRow = 1;
int List::commonColumn = 0;
int List::maxCommonColumn = 0;

void List::nextRow() {
	commonRow++;
	commonColumn = 0;
}

List::List() {
	commonColumn++;
	elementRow = commonRow;
	elementColumn = commonColumn;
	commonColumn > maxCommonColumn ? maxCommonColumn = commonColumn : maxCommonColumn = maxCommonColumn;
}

void List::printList(List *sPtr) {
	int tmpRow = 0;
	cout << "List:" << endl;
	while (sPtr != NULL) {
		if (sPtr->getElementRow() > tmpRow) {
			cout << endl;
			cout << sPtr->getElementValue();
			tmpRow = sPtr->getElementRow();
		}
		else {
			cout << "\t";
			cout << sPtr->getElementValue();
		}
		sPtr = sPtr->nextPtr;
	}
	return;
}

int List::getElementRow() {
	return elementRow;
}

int List::getElementColumn() {
	return elementColumn;
}

string List::getElementValue() {
	return elementValue;
}

string List::getElementValue(int column, int row) {
	List *sPtr = this;
	while (((sPtr->getElementColumn()) != column) || ((sPtr->getElementRow()) != row)) {
		sPtr = sPtr->nextPtr;
		if (sPtr == NULL) {
			return "NA";
		}
	}
	if (sPtr->getElementValue() == "") {
		return "NA";
	}
	return sPtr->getElementValue();
}

void List::setElementValue(string &item) {
	elementValue = item;
	return;
}

void List::printElementValue(int column, int row) {
	List *sPtr = this;
	cout << "Element of List (" << column << ", " << row << ") is: ";

	while (((sPtr->getElementColumn()) != column) || ((sPtr->getElementRow()) != row)) {
		sPtr = sPtr->nextPtr;
		if (sPtr == NULL) {
			cout << "hasn't found" << endl << endl;
			return;
		}
	}
	cout << sPtr->getElementValue() << endl << endl;
	return;
}

void List::saveInFile(List *sPtr, string &fileName) {
	ofstream fout;
	fout.open(fileName);
	int tmpRow = 1;
	while (sPtr != NULL) {
		if (sPtr->getElementRow() > tmpRow) {
			fout << "\n";
			fout << sPtr->getElementValue();
			tmpRow = sPtr->getElementRow();
		}
		else {
			if (((sPtr->elementColumn) != 1) || ((sPtr->elementRow) != 1)) {
				fout << ";";
			}
			fout << sPtr->getElementValue();
		}
		sPtr = sPtr->nextPtr;
	}
	fout << "\n";
	fout.close();
	return;
}

void List::insertElement(List **sPtr, string item) {
	List *newPtr = NULL;
	List *lastPtr = NULL;
	newPtr = new List;
	newPtr->setElementValue(item);
	newPtr->nextPtr = NULL;
	if (*sPtr == NULL) {
		*sPtr = newPtr;
		List::commonRow = 1;
		List::commonColumn = 0;
		List::maxCommonColumn = 0;
		commonColumn++;
		newPtr->elementRow = commonRow;
		newPtr->elementColumn = commonColumn;
		commonColumn > maxCommonColumn ? maxCommonColumn = commonColumn : maxCommonColumn = maxCommonColumn;
	}
	else {
		lastPtr = *sPtr;
		while (lastPtr->nextPtr != NULL) {
			lastPtr = lastPtr->nextPtr;
		}
		lastPtr->nextPtr = newPtr;
	}
	return;
}

bool List::readFromFile(List **sPtr, string fileName) {
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open()) {
		cout << endl << "File can not be opened!" << endl;
		return false;
	}
	string file;
	string tmp;
	unsigned int i = 0;
	while (fin.eof() == 0) {
		getline(fin, file);
		i = 0;
		while (i < file.length()) {
			tmp.clear();
			while ((i < file.length()) && (file[i] != ';')) {
				tmp += file[i];
				i++;
			}
			insertElement(sPtr, tmp);
			i++;
		}
		if (fin.eof() == 0) {
			(*sPtr)->nextRow();
		}
	}
	fin.close();
	cout << endl << "File has been read" << endl;
	return true;
}

int List::getCommonColumn() {
	return commonColumn;
}

int List::getCommonRow() {
	return commonRow;
}

int List::getMaxCommonColumn() {
	return maxCommonColumn;
}

List * List::getNextPtr() {
	return nextPtr;
}

List::~List() {
	delete nextPtr;
}
