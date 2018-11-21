#ifndef DATATABLE_H
#define DATATABLE_H

#include "List.h"
#include "Matrix.h"
#include "Matrix.cpp"//при наследовании специализации базового шаблонного класса, файл реализации базового шаблонного класса необходимо включать в хедер производного конкретного класса

class DataTable : public Matrix<string> {
public:
	DataTable(int columns, int rows);
	void extractConcentrationsFromKedaStructureToTable(List *startPtr);
	void extractConcentrationsFromFormattedStructureToTable(List *startPtr);

	void extractConcentrationsFromTableToFormattedStructure(List **startPtr);

	void printDataTable();

	string getComponentArray(int column);
	void setComponentArray(int column, string item);
	string getNameArray(int row);
	void setNameArray(int row, string name);
	~DataTable();
protected:
	string *componentArray;
	string *nameArray;
};

#endif // !DATATABLE_H