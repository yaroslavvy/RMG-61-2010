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
	static string numberEnding(const int number);
	static bool saveReport(FourDimArray* fourDimArrayPtr, ...);
	static void writeOneFourDimArray(FourDimArray *fourDimArrayPtr, ofstream & fout);
	static bool isIntegerFromString(const string possibleInteger);
	~Service();
};
#endif // !SERVICE_H
