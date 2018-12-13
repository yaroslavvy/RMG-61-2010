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
#include <algorithm>
using std::ifstream;
using std::ofstream;
using std::to_string;
using std::stof;

#include "DataTable.h"

DataTable::DataTable(const int columns, const int rows)
	: Matrix<string>(columns, rows) {
	componentArray = new string[columns];
	nameArray = new string[rows];
}

void DataTable::extractConcentrationsFromFormattedStructureToTable(List *startPtr) {
	string tmp;
	for (int i = 2; i <= List::getMaxCommonColumn(); i++) {
		setComponentArray(i - 2, startPtr->getElementValue(i, 1));
	}

	for (int j = 2; j < List::getCommonRow(); j++) {
		setNameArray(j - 2, startPtr->getElementValue(1, j));
	}

	for (int r = 2; r < List::getCommonRow(); r++) {
		for (int c = 2; c <= List::getMaxCommonColumn(); c++) {
			tmp = startPtr->getElementValue(c, r);
			std::replace(tmp.begin(), tmp.end(), ',', '.');
			setMatrixValue(c - 2, r - 2, tmp);
		}
	}
	cout << endl << "Data has been extracted" << endl;
	return;
}

string DataTable::getComponentArray(const int column) {
	return *(componentArray + column);
}

void DataTable::setComponentArray(const int column, const string item) {
	*(componentArray + column) = item;
	return;
}

string DataTable::getNameArray(const int row) {
	return *(nameArray + row);
}

void DataTable::setNameArray(const int row, const string name) {
	*(nameArray + row) = name;
	return;
}

DataTable::~DataTable() {
	/*for (int i = 0; i < getAmountOfMatrixRows(); i++) {
		delete[](*(matrix + i));
	}
	delete[] matrix;
	delete[] componentArray;
	delete[] nameArray;*/
}
