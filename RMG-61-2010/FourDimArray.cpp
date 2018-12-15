#include "pch.h"

#include <windows.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <clocale>
#include <cstdlib>
#include <cmath>
#include <stdarg.h>
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::to_string;

#include "FourDimArray.h"

FourDimArray::FourDimArray(const int session, const int component, const int sampleName, const int parallel) {
	setAmountOfSessions(session);
	setAmountOfComponents(component);
	setAmountOfSampleNames(sampleName);
	setAmountOfParallels(parallel);
	componentsArray = new string[getAmountOfComponents()];
	sampleNamesArray = new string[getAmountOfSampleNames()];
	setDescription("Some four-dimension array with some data");

	for (int i = 0; i < getAmountOfComponents(); i++) {
		setStrComponent(i, i);
	}
	for (int j = 0; j < getAmountOfSampleNames(); j++) {
		setStrSampleName(j, j);
	}
	fourDimArray = new struct cell ***[getAmountOfSessions()];
	for (int s = 0; s < getAmountOfSessions(); s++) {
		(*(fourDimArray + s)) = new struct cell **[getAmountOfComponents()];
		for (int c = 0; c < getAmountOfComponents(); c++) {
			(*(*(fourDimArray + s) + c)) = new struct cell *[getAmountOfSampleNames()];
			for (int sN = 0; sN < getAmountOfSampleNames(); sN++) {
				(*(*(*(fourDimArray + s) + c) + sN)) = new struct cell[getAmountOfParallels()];
			}
		}
	}
	for (int s = 0; s < getAmountOfSessions(); s++) {
		for (int c = 0; c < getAmountOfComponents(); c++) {
			for (int sN = 0; sN < getAmountOfSampleNames(); sN++) {
				for (int p = 0; p < getAmountOfParallels(); p++) {
					setFourDimArrayConcentration(s, c, sN, p, 0);
					setFourDimArrayStatus(s, c, sN, p, 0);
					setFourDimArrayExist(s, c, sN, p, false);
					setMessageWithNullStatus(s, c, sN, p, "");
				}
			}
		}
	}
}

void FourDimArray::setAmountOfSessions(const int item) {
	amountOfSessions = item;
	return;
}

void FourDimArray::setAmountOfComponents(const int item) {
	amountOfComponents = item;
	return;
}

void FourDimArray::setAmountOfSampleNames(const int item) {
	amountOfSampleNames = item;
	return;
}

void FourDimArray::setAmountOfParallels(const int item) {
	amountOfParallels = item;
	return;
}

void FourDimArray::setDescription(const string item) {
	description = item;
	return;
}

void FourDimArray::setMessageWithNullStatus(const int session, const int component, const int sampleName, const int parallel, const string message) {
	(*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->messageWithNullStatus = message;
	return;
}

int FourDimArray::getAmountOfSessions() {
	return amountOfSessions;
}

int FourDimArray::getAmountOfSessions(const int component, const int sampleName) {
	int sessionsCounter = 0;
	for (int s = 0; s < getAmountOfSessions(); s++) {
		for (int p = 0; p < getAmountOfParallels(); p++) {
			if ((getFourDimArrayStatus(s, component, sampleName, p) == 0) && (getFourDimArrayExist(s, component, sampleName, p))) {
				sessionsCounter++;
				break;
			}
		}
	}
	return sessionsCounter;
}

int FourDimArray::getAmountOfComponents() {
	return amountOfComponents;
}


int FourDimArray::getAmountOfSampleNames() {
	return amountOfSampleNames;
}


int FourDimArray::getAmountOfParallels() {
	return amountOfParallels;
}

int FourDimArray::getAmountOfParallels(const int session, const int component, const int sampleName) {
	int parallelsCounter = 0;
	for (int p = 0; p < getAmountOfParallels(); p++) {
		if ((getFourDimArrayStatus(session, component, sampleName, p) == 0) && (getFourDimArrayExist(session, component, sampleName, p))) {
			parallelsCounter++;
		}
	}
	return parallelsCounter;
}

int FourDimArray::getAmountOfExistParallels(const int session, const int component, const int sampleName) {
	int parallelsCounter = 0;
	for (int p = 0; p < getAmountOfParallels(); p++) {
		if (getFourDimArrayExist(session, component, sampleName, p)) {
		parallelsCounter++;
		}
	}
	return parallelsCounter;
}

string FourDimArray::getDescription() {
	return description;
}

string FourDimArray::getMessageWithNullStatus(const int session, const int component, const int sampleName, const int parallel){
	return (*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->messageWithNullStatus;
}

FourDimArray * FourDimArray::extractDataFromTableToFourDimArray(const struct sessions * arrayOfSessions) {
	//It should define 4 parameters of FourDimArray (max amount of sessions, components, sample names and parallels) before creating it.
	//Defining max amount of components
	int tmpAmountOfComponents = 1;
	string *componentArray = NULL;
	string *tmpComponentArray = NULL;
	for (int s = 0; s < arrayOfSessions->amountOfSessions; s++) {
		for (int c = 0; c < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixColumns(); c++) {
			if (componentArray == NULL) {
				componentArray = new string[tmpAmountOfComponents];
				componentArray[0] = (*(arrayOfSessions->sessionDataTablesArray))->getComponentArray(0);
			}
			for (int i = 0; i < tmpAmountOfComponents; i++) {
				if (*(componentArray + i) == (*(arrayOfSessions->sessionDataTablesArray + s))->getComponentArray(c)) {
					i = tmpAmountOfComponents;
				}
				else {
					if (i == (tmpAmountOfComponents - 1)) {
						tmpComponentArray = new string[tmpAmountOfComponents];
						for (int j = 0; j < tmpAmountOfComponents; j++) {
							*(tmpComponentArray + j) = *(componentArray + j);
						}
						tmpAmountOfComponents++;
						delete[] componentArray;
						componentArray = NULL;
						componentArray = new string[tmpAmountOfComponents];
						for (int l = 0; l < (tmpAmountOfComponents - 1); l++) {
							*(componentArray + l) = *(tmpComponentArray + l);
						}
						delete[] tmpComponentArray;
						tmpComponentArray = NULL;
						*(componentArray + (tmpAmountOfComponents - 1)) = (*(arrayOfSessions->sessionDataTablesArray + s))->getComponentArray(c);
					}
				}
			}
		}
	}
	//Defining max amount of sample names
	int tmpAmountOfSampleNames = 1;
	string *sampleNameArray = NULL;
	string *tmpSampleNameArray = NULL;
	for (int s = 0; s < arrayOfSessions->amountOfSessions; s++) {
		for (int sN = 0; sN < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixRows(); sN++) {
			if (sampleNameArray == NULL) {
				sampleNameArray = new string[tmpAmountOfSampleNames];
				sampleNameArray[0] = (*(arrayOfSessions->sessionDataTablesArray))->getNameArray(0);
			}
			for (int i = 0; i < tmpAmountOfSampleNames; i++) {
				if (*(sampleNameArray + i) == (*(arrayOfSessions->sessionDataTablesArray + s))->getNameArray(sN)) {
					i = tmpAmountOfSampleNames;
				}
				else {
					if (i == (tmpAmountOfSampleNames - 1)) {
						tmpSampleNameArray = new string[tmpAmountOfSampleNames];
						for (int j = 0; j < tmpAmountOfSampleNames; j++) {
							*(tmpSampleNameArray + j) = *(sampleNameArray + j);
						}
						tmpAmountOfSampleNames++;
						delete[] sampleNameArray;
						sampleNameArray = NULL;
						sampleNameArray = new string[tmpAmountOfSampleNames];
						for (int l = 0; l < (tmpAmountOfSampleNames - 1); l++) {
							*(sampleNameArray + l) = *(tmpSampleNameArray + l);
						}
						delete[] tmpSampleNameArray;
						tmpSampleNameArray = NULL;
						*(sampleNameArray + (tmpAmountOfSampleNames - 1)) = (*(arrayOfSessions->sessionDataTablesArray + s))->getNameArray(sN);
					}
				}
			}
		}
	}
	//Defining max amount of parallels
	int *parallelArray = NULL;
	int maxAmountOfParallels = 0;
	for (int s = 0; s < arrayOfSessions->amountOfSessions; s++) {
		parallelArray = new int[(*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixColumns()];
		for (int i = 0; i < tmpAmountOfSampleNames; i++) {
			for (int x = 0; x < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixColumns(); x++) {
				*(parallelArray + x) = 0;
			}
			for (int sN = 0; sN < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixRows(); sN++) {
				if (*(sampleNameArray + i) == (*(arrayOfSessions->sessionDataTablesArray + s))->getNameArray(sN)) {
					for (int c = 0; c < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixColumns(); c++) {
						if (((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN) != "") && ((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN) != "NA")) {
							*(parallelArray + c) += 1;
						}
					}
				}
			}
			for (int c = 0; c < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixColumns(); c++) {
				if (*(parallelArray + c) > maxAmountOfParallels) {
					maxAmountOfParallels = *(parallelArray + c);
				}
			}
		}
		delete[] parallelArray;
		parallelArray = NULL;
	}
	// Creating FourDimArray
	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(arrayOfSessions->amountOfSessions, tmpAmountOfComponents, tmpAmountOfSampleNames, maxAmountOfParallels);

	for (int c = 0; c < tmpAmountOfComponents; c++) {
		sPtr->setStrComponent(c, *(componentArray + c));
	}

	for (int sN = 0; sN < tmpAmountOfSampleNames; sN++) {
		sPtr->setStrSampleName(sN, *(sampleNameArray + sN));
	}

	for (int s = 0; s < arrayOfSessions->amountOfSessions; s++) {
		for (int c = 0; c < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixColumns(); c++) {
			int iC = 0;
			while (sPtr->getStrComponent(iC) != (*(arrayOfSessions->sessionDataTablesArray + s))->getComponentArray(c)) {
				iC++;
			}
			for (int sN = 0; sN < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixRows(); sN++) {
				int iSn = 0;
				while (sPtr->getStrSampleName(iSn) != (*(arrayOfSessions->sessionDataTablesArray + s))->getNameArray(sN)) {
					iSn++;
				}
				if (((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN) != "") && ((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN) != "NA") && (isFloatFromString((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN)))) {
					sPtr->pushParallel(s, iC, iSn, stof((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN)));
				}
			}
		}
	}
	return sPtr;
}

FourDimArray * FourDimArray::copyFourDimArray(FourDimArray * input) {
	FourDimArray * sPtr = NULL;
	sPtr = new FourDimArray(input->getAmountOfSessions(), input->getAmountOfComponents(), input->getAmountOfSampleNames(), input->getAmountOfParallels());
	 
	for (int c = 0; c < input->getAmountOfComponents(); c++) {
		sPtr->setStrComponent(c, (input->getStrComponent(c)));
	}

	for (int sN = 0; sN < input->getAmountOfSampleNames(); sN++) {
		sPtr->setStrSampleName(sN, (input->getStrSampleName(sN)));
	}
	 
	 sPtr->setDescription(input->getDescription());

	 for (int s = 0; s < input->getAmountOfSessions(); s++) {
		 for (int c = 0; c < input->getAmountOfComponents(); c++) {
			 for (int sN = 0; sN < input->getAmountOfSampleNames(); sN++) {
				 for (int p = 0; p < input->getAmountOfParallels(); p++) {
					 sPtr->setFourDimArrayConcentration(s, c, sN, p, input->getFourDimArrayConcentration(s, c, sN, p));
					 sPtr->setFourDimArrayStatus(s, c, sN, p, input->getFourDimArrayStatus(s, c, sN, p));
					 sPtr->setFourDimArrayExist(s, c, sN, p, input->getFourDimArrayExist(s, c, sN, p));
				 }
			 }
		 }
	 }
	return sPtr;
}

void FourDimArray::setFourDimArrayConcentration(const int session, const int component, const int sampleName, const int parallel, const float item) {
	(*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->concentration = item;
	return;
}

void FourDimArray::setFourDimArrayStatus(const int session, const int component, const int sampleName, const int parallel, const int state) {
	(*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->status = state;
	return;
}

void FourDimArray::setFourDimArrayExist(const int session, const int component, const int sampleName, const int parallel, const bool state) {
	(*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->exist = state;
	return;
}

float FourDimArray::getFourDimArrayConcentration(const int session, const int component, const int sampleName, const int parallel) {
	return (*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->concentration;
}

int FourDimArray::getFourDimArrayStatus(const int session, const int component, const int sampleName, const int parallel) {
	return (*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->status;
}

bool FourDimArray::getFourDimArrayExist(const int session, const int component, const int sampleName, const int parallel) {
	return (*(*(*(fourDimArray + session) + component) + sampleName) + parallel)->exist;
}

void FourDimArray::setStrComponent(const int componentOrder, const string component) {
	(*(componentsArray + componentOrder)) = component;
	return;
}

void FourDimArray::setStrSampleName(const int sampleNameOrder, const string sampleName) {
	(*(sampleNamesArray + sampleNameOrder)) = sampleName;
	return;
}

void FourDimArray::setStrComponent(const int componentOrder, const int component) {
	string c = to_string(component);
	(*(componentsArray + componentOrder)) = c;
	return;
}

void FourDimArray::setStrSampleName(const int sampleNameOrder, const int sampleName) {
	string sN = to_string(sampleName);
	(*(sampleNamesArray + sampleNameOrder)) = sN;
	return;
}

string FourDimArray::getStrComponent(const int componentOrder) {
	return (*(componentsArray + componentOrder));
}

string FourDimArray::getStrSampleName(const int sampleNameOrder) {
	return (*(sampleNamesArray + sampleNameOrder));
}

void FourDimArray::pushParallel(const int session, const int component, const int sampleName, const float concentration) {
	int parallel = 0;
	while (getFourDimArrayExist(session, component, sampleName, parallel)) {
		parallel++;
	}
	setFourDimArrayConcentration(session, component, sampleName, parallel, concentration);
	setFourDimArrayExist(session, component, sampleName, parallel, true);
	return;
}

void FourDimArray::pasteValuesInCopyFormat(FourDimArray *fourDimArrayPtr, DataTable *crm, FourDimArray *crmConcentration) {
	string tmpString = "";
	for (int c = 0; c < fourDimArrayPtr->getAmountOfComponents(); c++) {
		setStrComponent(c, fourDimArrayPtr->getStrComponent(c));
	}
	for (int sN = 0; sN < fourDimArrayPtr->getAmountOfSampleNames(); sN++) {
		setStrSampleName(sN, fourDimArrayPtr->getStrSampleName(sN));
	}
	for (int c = 0; c < getAmountOfComponents(); c++) {
		for (int iC = 0; iC < crm->getAmountOfMatrixColumns(); iC++) {
			if (getStrComponent(c) == crm->getComponentArray(iC)) {
				for (int sN = 0; sN < getAmountOfSampleNames(); sN++) {
					for (int iSn = 0; iSn < crm->getAmountOfMatrixRows(); iSn++) {
						if (getStrSampleName(sN) == crm->getNameArray(iSn)) {
							if ((crm->getMatrixValue(iC, iSn) != "") && (crm->getMatrixValue(iC, iSn) != "NA")) {
								if ((crm->getMatrixValue(iC, iSn).find('%') == string::npos) && (isFloatFromString(crm->getMatrixValue(iC, iSn)))) {
									pushParallel(0, c, sN, stof(crm->getMatrixValue(iC, iSn)));
								}
								else {
									if(crmConcentration == NULL){
										cout << "Error! Char '%' used in file with concentrations. Use this char only in file with uncertainties" << endl;
										return;
									}
									tmpString.clear();
									for (int i = 0; i < crm->getMatrixValue(iC, iSn).find('%'); i++) {
										tmpString += crm->getMatrixValue(iC, iSn)[i];
									}
									if (crmConcentration->getFourDimArrayExist(0, c, sN, 0) && (isFloatFromString(tmpString))) {
										pushParallel(0, c, sN, stof(tmpString)*0.01*crmConcentration->getFourDimArrayConcentration(0, c, sN, 0));
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return;
}

bool FourDimArray::isFloatFromString(const string possibleFloat) {
	return possibleFloat.find_first_not_of("0123456789.,") == string::npos;
}

FourDimArray::~FourDimArray() {

	for (int s = 0; s < getAmountOfSessions(); s++) {
		for (int c = 0; c < getAmountOfComponents(); c++) {
			for (int sN = 0; sN < getAmountOfSampleNames(); sN++) {
				delete[] (*(*(*(fourDimArray + s) + c) + sN));
			}
		}
	}

	for (int s = 0; s < getAmountOfSessions(); s++) {
		for (int c = 0; c < getAmountOfComponents(); c++) {
			delete[] (*(*(fourDimArray + s) + c));
		}
	}

	for (int s = 0; s < getAmountOfSessions(); s++) {
		delete[] (*(fourDimArray + s));
	}

	delete[] fourDimArray;
	delete[] componentsArray;
	delete[] sampleNamesArray;

}