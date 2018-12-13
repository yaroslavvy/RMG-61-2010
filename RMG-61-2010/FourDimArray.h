#ifndef FOURDIMARRAY_H
#define FOURDIMARRAY_H

#include "DataTable.h"
#include "StructCell.h"
#include "StructureSessions.h"
#include "Matrix.h"

class FourDimArray {
public:
	FourDimArray(const int session, const int component, const int sampleName, const int parallel);

	void setAmountOfSession(const int item);
	void setAmountOfComponent(const int item);
	void setAmountOfSampleName(const int item);
	void setAmountOfParallel(const int item);

	int getAmountOfSession();
	int getAmountOfSession(const int component, const int sampleName);
	int getAmountOfComponent();
	int getAmountOfSampleName();
	int getAmountOfParallel();
	int getAmountOfParallel(const int session, const int component, const int sampleName);
	int getAmountOfParallel(const int session, const int component, const int sampleName, const string withoutSmth);

	static FourDimArray * extractDataFromTableToFourDimArray(const struct sessions * arrayOfSessions);

	static FourDimArray * copyFourDimArray(FourDimArray * input);

	void printFourDimArray();

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
	void setStrSampleName(const int sampleNameOrder, const string sampleName);

	void setStrComponent(const int componentOrder, const int component);
	void setStrSampleName(const int sampleNameOrder, const int sampleName);

	string getStrComponent(const int componentOrder);
	string getStrSampleName(const int sampleNameOrder);

	~FourDimArray();

protected:
	int amountOfSession;
	int amountOfComponent;
	int amountOfSampleName;
	int amountOfParallel;

	string *componentArray;
	string *sampleNameArray;

	struct cell ****fourdimarray;

	string description;
};

#endif // !THREEDIMARRAY_H