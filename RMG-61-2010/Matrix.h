#ifndef MATRIX_H
#define MATRIX_H

template <class T>
class Matrix {
public:
	Matrix(const int columns, const int rows);
	void setAmountOfMatrixColumns(const int item);
	void setAmountOfMatrixRows(const int item);
	int getAmountOfMatrixColumns();
	int getAmountOfMatrixRows();
	void setMatrixValue(const int column,const int row, const T item);
	T getMatrixValue(const int column, const int row);
	~Matrix();
protected:
	int amountOfMatrixColumns;
	int amountOfMatrixRows;
	T **matrix;
};

#endif // !MATRIX_H