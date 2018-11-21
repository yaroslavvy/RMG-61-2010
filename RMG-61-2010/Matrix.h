#ifndef MATRIX_H
#define MATRIX_H

template <class T>
class Matrix {
public:
	Matrix(int columns, int rows);
	void setAmountOfMatrixColumns(int item);
	void setAmountOfMatrixRows(int item);
	int getAmountOfMatrixColumns();
	int getAmountOfMatrixRows();
	void setMatrixValue(int column, int row, T item);
	T getMatrixValue(int column, int row);
	~Matrix();
protected:
	int amountOfMatrixColumns;
	int amountOfMatrixRows;
	T **matrix;
};

#endif // !MATRIX_H