#ifndef LIST_H
#define LIST_H

class List
{
public:
	List();
	static void nextRow();
	static void insertElement(List **startPtr, const string &item);
	static bool readFromFile(List **startPtr, const string &fileName);
	int getElementRow();
	int getElementColumn();
	string getElementValue();
	string getElementValue(const int column,const int row);
	void setElementValue(const string &item);
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