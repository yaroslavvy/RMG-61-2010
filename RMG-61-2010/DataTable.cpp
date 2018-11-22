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

DataTable::DataTable(int columns, int rows)
	: Matrix<string>(columns, rows)
{
	componentArray = new string[columns];
	nameArray = new string[rows];

}

void DataTable::extractConcentrationsFromKedaStructureToTable(List *startPtr) {
	return;//позже дописать
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
	cout << endl << "Data has been extracted from list to table" << endl;
	return;
}

void DataTable::extractConcentrationsFromTableToFormattedStructure(List **startPtr) {
	string i("");
	List::insertElement(startPtr, i);
	for (int i = 0; i < Matrix<string>::getAmountOfMatrixColumns(); i++) {
		List::insertElement(startPtr, getComponentArray(i));
	}
	List::nextRow();
	List::insertElement(startPtr, getNameArray(0));
	for (int r = 0; r < Matrix<string>::getAmountOfMatrixRows(); r++) {
		for (int c = 0; c < Matrix<string>::getAmountOfMatrixColumns(); c++) {
			List::insertElement(startPtr, getMatrixValue(c, r));
		}
		List::nextRow();
		if (r < (Matrix<string>::getAmountOfMatrixRows() - 1)) {
			List::insertElement(startPtr, getNameArray(r + 1));
		}
	}
	cout << endl << "Data has been extracted from table to list" << endl;
	return;
}

void DataTable::printDataTable() {
	cout << "\t\t";
	for (int j = 0; j < this->getAmountOfMatrixColumns(); j++) {
		cout << this->getComponentArray(j) << "\t\t";
	}
	cout << endl;

	for (int i = 0; i < this->getAmountOfMatrixRows(); i++) {
		cout << this->getNameArray(i) << "\t\t";
		for (int j = 0; j < this->getAmountOfMatrixColumns(); j++) {
			cout << this->getMatrixValue(j, i) << "\t\t";
		}
		cout << endl;
	}

	cout << "amountOfMatrixColumns: " << getAmountOfMatrixColumns() << endl;
	cout << "amountOfMatrixRows: " << getAmountOfMatrixRows() << endl;

	return;
}

string DataTable::getComponentArray(int column) {
	return *(componentArray + column);
}

void DataTable::setComponentArray(int column, string item) {
	*(componentArray + column) = item;
	return;
}

string DataTable::getNameArray(int row) {
	return *(nameArray + row);
}

void DataTable::setNameArray(int row, string name) {
	*(nameArray + row) = name;
	return;
}

DataTable::~DataTable() {
}
