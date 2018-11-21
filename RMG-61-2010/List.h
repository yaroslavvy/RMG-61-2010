#ifndef LIST_H
#define LIST_H

class List
{
public:
	List();
	void printList(List *startPtr);
	static void nextRow();
	static void insertElement(List **startPtr, string item);
	static bool readFromFile(List **startPtr, string fileName);
	int getElementRow();
	int getElementColumn();
	string getElementValue();
	string getElementValue(int column, int row);
	void setElementValue(string &item);
	void printElementValue(int elementColumn, int elementRow);
	void saveInFile(List *startPtr, string &fileName);
	static int getCommonColumn();
	static int getMaxCommonColumn();
	static int getCommonRow();
	List * getNextPtr();
	~List();
protected:
	static int commonRow;
	static int commonColumn;
	static int maxCommonColumn;
	string elementValue;
	int elementRow;
	int elementColumn;
	List *nextPtr;
};
#endif // !LIST_H