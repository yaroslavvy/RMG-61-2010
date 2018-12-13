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

#include "Matrix.h"

template <class T>
Matrix<T>::Matrix(const int columns, const int rows) {
	setAmountOfMatrixColumns(columns);
	setAmountOfMatrixRows(rows);
	matrix = new T *[getAmountOfMatrixRows()];
	for (int i = 0; i < getAmountOfMatrixRows(); i++) {
		(*(matrix + i)) = new T[getAmountOfMatrixColumns()];
	}
	return;
}

template <class T>
void Matrix<T>::setAmountOfMatrixColumns(const int item) {
	amountOfMatrixColumns = item;
	return;
}

template <class T>
void Matrix<T>::setAmountOfMatrixRows(const int item) {
	amountOfMatrixRows = item;
	return;
}

template <class T>
int Matrix<T>::getAmountOfMatrixColumns() {
	return amountOfMatrixColumns;
}

template <class T>
int Matrix<T>::getAmountOfMatrixRows() {
	return amountOfMatrixRows;
}

template <class T>
void Matrix<T>::setMatrixValue(const int column, const int row, const T item) {
	*(*(matrix + row) + column) = item;
	return;
}

template <class T>
T Matrix<T>::getMatrixValue(const int column, const int row) {
	return *(*(matrix + row) + column);
}

template <class T>
Matrix<T>::~Matrix() {

}
