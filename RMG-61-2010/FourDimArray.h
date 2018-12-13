#ifndef FOURDIMARRAY_H
#define FOURDIMARRAY_H

#include "DataTable.h"
#include "StructCell.h"
#include "StructureSessions.h"
#include "Matrix.h"

class FourDimArray {
public:
	FourDimArray(const int session, const int component, const int sampleName, const int parallel);//+

	void setAmountOfSessions(const int item);
	void setAmountOfComponents(const int item);
	void setAmountOfSampleNames(const int item);
	void setAmountOfParallels(const int item);

	int getAmountOfSessions();
	int getAmountOfSessions(const int component, const int sampleName);
	int getAmountOfComponents();
	int getAmountOfSampleNames();
	int getAmountOfParallels();
	int getAmountOfParallels(const int session, const int component, const int sampleName);
	int getAmountOfExistParallels(const int session, const int component, const int sampleName);

	static FourDimArray * extractDataFromTableToFourDimArray(const struct sessions * arrayOfSessions);

	static FourDimArray * copyFourDimArray(FourDimArray * input);

	void setFourDimArrayConcentration(const int session, const int component, const int sampleName, const int parallel, const float concentration);
	void setFourDimArrayStatus(const int session, const int component, const int sampleName, const int parallel, const int status);
	void setFourDimArrayExist(const int session, const int component, const int sampleName, const int parallel, const bool exist);
	void setDescription(const string description);
	void setMessageWithNullStatus(const int session, const int component, const int sampleName, const int parallel, const string message);

	float getFourDimArrayConcentration(const int session, const int component, const int sampleName, const int parallel);
	int getFourDimArrayStatus(const int session, const int component, const int sampleName, const int parallel);
	bool getFourDimArrayExist(const int session, const int component, const int sampleName, const int parallel);
	string getDescription();
	string getMessageWithNullStatus(const int session, const int component, const int sampleName, const int parallel);

	void pushParallel(const int session, const int component, const int sampleName, const float concentration);

	void pasteValuesInCopyFormat(FourDimArray *fourDimArrayPtr, DataTable *crm, FourDimArray *crmConcentration);

	void setStrComponent(const int componentOrder, const string component);
	void setStrComponent(const int componentOrder, const int component);
	void setStrSampleName(const int sampleNameOrder, const string sampleName);
	void setStrSampleName(const int sampleNameOrder, const int sampleName);

	string getStrComponent(const int componentOrder);
	string getStrSampleName(const int sampleNameOrder);

	static bool isFloatFromString(const string possibleFloat);

	~FourDimArray();

protected:
	int amountOfSessions;
	int amountOfComponents;
	int amountOfSampleNames;
	int amountOfParallels;

	string *componentsArray;
	string *sampleNamesArray;

	struct cell ****fourDimArray;

	string description;
};

#endif // !THREEDIMARRAY_H