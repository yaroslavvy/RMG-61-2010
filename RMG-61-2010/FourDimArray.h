#ifndef FOURDIMARRAY_H
#define FOURDIMARRAY_H

#include "DataTable.h"
#include "StructCell.h"
#include "StructureSessions.h"
#include "Matrix.h"

class FourDimArray {
public:
	FourDimArray(int session, int component, int sampleName, int parallel);

	void setAmountOfSession(int item);
	void setAmountOfComponent(int item);
	void setAmountOfSampleName(int item);
	void setAmountOfParallel(int item);

	int getAmountOfSession();
	int getAmountOfSession(int component, int sampleName);
	int getAmountOfComponent();
	int getAmountOfSampleName();
	int getAmountOfParallel();
	int getAmountOfParallel(int session, int component, int sampleName);
	int getAmountOfParallel(int session, int component, int sampleName, string withoutSmth);

	static FourDimArray * extractDataFromTableToFourDimArray(struct sessions * arrayOfSessions);

	static FourDimArray * copyFourDimArray(FourDimArray * input);

	void printFourDimArray();

	void setFourDimArrayConcentration(int session, int component, int sampleName, int parallel, float concentration);
	void setFourDimArrayStatus(int session, int component, int sampleName, int parallel, int status);
	void setFourDimArrayVisible(int session, int component, int sampleName, int parallel, bool visible);
	void setFourDimArrayExist(int session, int component, int sampleName, int parallel, bool exist);
	void setDescription(string description);
	void setMessageWithNullStatus(int session, int component, int sampleName, int parallel, string message);

	float getFourDimArrayConcentration(int session, int component, int sampleName, int parallel);
	int getFourDimArrayStatus(int session, int component, int sampleName, int parallel);
	bool getFourDimArrayVisible(int session, int component, int sampleName, int parallel);
	bool getFourDimArrayExist(int session, int component, int sampleName, int parallel);
	string getDescription();
	string getMessageWithNullStatus(int session, int component, int sampleName, int parallel);

	void pushParallel(int session, int component, int sampleName, float concentration);

	void pasteValuesInCopyFormat(FourDimArray *fourDimArrayPtr, DataTable *crm, FourDimArray *crmConcentration);

	void setStrComponent(int componentOrder, string component);
	void setStrSampleName(int sampleNameOrder, string sampleName);

	void setStrComponent(int componentOrder, int component);
	void setStrSampleName(int sampleNameOrder, int sampleName);

	string getStrComponent(int componentOrder);
	string getStrSampleName(int sampleNameOrder);

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

	float tmpFloat;
	int tmpInt;
	bool tmpBool;
};

#endif // !THREEDIMARRAY_H