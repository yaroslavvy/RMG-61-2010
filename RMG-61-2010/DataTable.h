#ifndef DATATABLE_H
#define DATATABLE_H

#include "List.h"
#include "Matrix.h"
#include "Matrix.cpp"

class DataTable : public Matrix<string> {
public:
	DataTable(const int columns, const int rows);
	void extractConcentrationsFromFormattedStructureToTable(List *startPtr);
	string getComponentArray(const int column);
	void setComponentArray(const int column, const string item);
	string getNameArray(const int row);
	void setNameArray(const int row, const string name);
	~DataTable();
protected:
	string *componentArray;
	string *nameArray;
};

#endif // !DATATABLE_H