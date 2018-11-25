#ifndef SERVICE_H
#define SERVICE_H


#include "List.h"
#include "DataTable.h"
#include "FourDimArray.h"
#include "Statistic.h"
#include "StructureSessions.h"

class Service {
public:
	Service();
	static void callMenu();
	template<class T>
	static bool isPtrNull(T *sPtr);
	static string numberEnding(int number);
	static bool saveReport(FourDimArray* fourDimArrayPtr, ...);
	static void writeOneFourDimArray(FourDimArray *fourDimArrayPtr, ofstream & fout);
	~Service();
protected:
	static string item;
	static string strAnswerDialogMenu;
	static int intAnswerDialogMenu;
	static int command;
	static int column;
	static int row;
	static int tmp;
	static string fileName;
	static List *startPtr;
	static DataTable *newDataTable;
	//static DataTable **sessionDataTablesArray;
	static struct sessions *arrayOfSessions;
	static FourDimArray *fourDimArrayPtr;
	static FourDimArray *fourDimArrayConcentrationsPtr;
	static FourDimArray *fourDimArrayUncertaintiesPtr;
	static FourDimArray *averageFourDimArrayPtr;
	static FourDimArray *biasFourDimArrayPtr;
	static FourDimArray *dispersionFourDimArrayPtr;
	static FourDimArray *cochrenCriteriaPtr;

};
#endif // !SERVICE_H
