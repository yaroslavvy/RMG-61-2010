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

FourDimArray::FourDimArray(int session, int component, int sampleName, int parallel) {
	setAmountOfSession(session);
	setAmountOfComponent(component);
	setAmountOfSampleName(sampleName);
	setAmountOfParallel(parallel);
	componentArray = new string[getAmountOfComponent()];
	sampleNameArray = new string[getAmountOfSampleName()];
	setDescription("Some four-dimension array with some data");
	tmpFloat = 1.000;
	tmpInt = 1;
	tmpBool = true;

	for (int i = 0; i < getAmountOfComponent(); i++) {
		setStrComponent(i, i);
	}
	for (int j = 0; j < getAmountOfSampleName(); j++) {
		setStrSampleName(j, j);
	}
	fourdimarray = new struct cell ***[getAmountOfSession()];
	for (int s = 0; s < getAmountOfSession(); s++) {
		(*(fourdimarray + s)) = new struct cell **[getAmountOfComponent()];
		for (int c = 0; c < getAmountOfComponent(); c++) {
			(*(*(fourdimarray + s) + c)) = new struct cell *[getAmountOfSampleName()];
			for (int sN = 0; sN < getAmountOfSampleName(); sN++) {
				(*(*(*(fourdimarray + s) + c) + sN)) = new struct cell[getAmountOfParallel()];
			}
		}
	}
	for (int s = 0; s < getAmountOfSession(); s++) {
		for (int c = 0; c < getAmountOfComponent(); c++) {
			for (int sN = 0; sN < getAmountOfSampleName(); sN++) {
				for (int p = 0; p < getAmountOfParallel(); p++) {
					setFourDimArrayConcentration(s, c, sN, p, 0);
					setFourDimArrayStatus(s, c, sN, p, 0);
					setFourDimArrayVisible(s, c, sN, p, true);
					setFourDimArrayExist(s, c, sN, p, false);
				}
			}
		}
	}
}

void FourDimArray::setAmountOfSession(int item) {
	amountOfSession = item;
	return;
}

void FourDimArray::setAmountOfComponent(int item) {
	amountOfComponent = item;
	return;
}

void FourDimArray::setAmountOfSampleName(int item) {
	amountOfSampleName = item;
	return;
}

void FourDimArray::setAmountOfParallel(int item) {
	amountOfParallel = item;
	return;
}

void FourDimArray::setDescription(string item) {
	description = item;
	return;
}

int FourDimArray::getAmountOfSession() {
	return amountOfSession;
}


int FourDimArray::getAmountOfComponent() {
	return amountOfComponent;
}


int FourDimArray::getAmountOfSampleName() {
	return amountOfSampleName;
}


int FourDimArray::getAmountOfParallel() {
	return amountOfParallel;
}

string FourDimArray::getDescription() {
	return description;
}


FourDimArray * FourDimArray::extractDataFromTableToFourDimArray(struct sessions * arrayOfSessions) {
	//Прежде чем создавать Ptr на объект необходимо определить какие параметры 4-х мерного массива нужно указать при его создании
	//определение компонентов
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
						componentArray = new string[tmpAmountOfComponents];
						for (int l = 0; l < (tmpAmountOfComponents - 1); l++) {
							*(componentArray + l) = *(tmpComponentArray + l);
						}
						delete[] tmpComponentArray;
						*(componentArray + (tmpAmountOfComponents - 1)) = (*(arrayOfSessions->sessionDataTablesArray + s))->getComponentArray(c);
					}
				}
			}
		}
	}
	cout << endl;
	for (int k = 0; k < tmpAmountOfComponents; k++) {
		cout << *(componentArray + k) << "\t\t";
	}
	cout << endl;
	cout << "tmpAmountOfComponents: " << tmpAmountOfComponents;
	cout << endl;

	//Определение имён образцов
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
						sampleNameArray = new string[tmpAmountOfSampleNames];
						for (int l = 0; l < (tmpAmountOfSampleNames - 1); l++) {
							*(sampleNameArray + l) = *(tmpSampleNameArray + l);
						}
						delete[] tmpSampleNameArray;
						*(sampleNameArray + (tmpAmountOfSampleNames - 1)) = (*(arrayOfSessions->sessionDataTablesArray + s))->getNameArray(sN);
					}
				}
			}
		}
	}

	cout << endl;
	for (int k = 0; k < tmpAmountOfSampleNames; k++) {
		cout << *(sampleNameArray + k) << endl;
	}
	cout << endl;
	cout << "tmpAmountOfSampleNames: " << tmpAmountOfSampleNames;
	cout << endl;

	//Определение максимального количества параллелей

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
			cout << endl;
			for (int c = 0; c < (*(arrayOfSessions->sessionDataTablesArray + s))->getAmountOfMatrixColumns(); c++) {
				cout << *(parallelArray + c) << "\t";// Вывод параллелей в сессии на экран
			}
		}
		delete[] parallelArray;
		parallelArray = NULL;
		cout << endl;// Вывод параллелей в сессии на экран
	}

	cout << endl;
	cout << "maxAmountOfParallels: " << maxAmountOfParallels;
	cout << endl;

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
				if (((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN) != "") && ((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN) != "NA")) {
					sPtr->pushParallel(s, iC, iSn, stof((*(arrayOfSessions->sessionDataTablesArray + s))->getMatrixValue(c, sN)));
				}
			}
		}
	}
	return sPtr;
}


void FourDimArray::printFourDimArray() {
	for (int s = 0; s < getAmountOfSession(); s++) {
		cout << endl << s + 1 << " session:" << endl;
		cout << "\t\t\t";
		for (int c = 0; c < getAmountOfComponent(); c++) {
			cout << getStrComponent(c);
			c == (getAmountOfComponent() - 1) ? cout << endl << endl : cout << "\t\t";
		}
		for (int sN = 0; sN < getAmountOfSampleName(); sN++) {
			cout << getStrSampleName(sN) << "\t\t";
			for (int p = 0; p < getAmountOfParallel(); p++) {
				for (int c = 0; c < getAmountOfComponent(); c++) {
					getFourDimArrayExist(s, c, sN, p) && getFourDimArrayVisible(s, c, sN, p) ? cout << getFourDimArrayConcentration(s, c, sN, p) : cout << "";
					if (c < (getAmountOfComponent() - 1)) {
						cout << "\t\t";
					}
				}
				cout << endl << "\t\t\t";
			}
			cout << endl;
		}
	}
	return;
}

void FourDimArray::setFourDimArrayConcentration(int session, int component, int sampleName, int parallel, float item) {
	(*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->concentration = item;
	return;
}

void FourDimArray::setFourDimArrayStatus(int session, int component, int sampleName, int parallel, int state) {
	(*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->status = state;
	return;
}

void FourDimArray::setFourDimArrayVisible(int session, int component, int sampleName, int parallel, bool state) {
	(*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->visible = state;
	return;
}

void FourDimArray::setFourDimArrayExist(int session, int component, int sampleName, int parallel, bool state) {
	(*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->exist = state;
	return;
}

float FourDimArray::getFourDimArrayConcentration(int session, int component, int sampleName, int parallel) {
	return (*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->concentration;
}

int FourDimArray::getFourDimArrayStatus(int session, int component, int sampleName, int parallel) {
	return (*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->status;
}

bool FourDimArray::getFourDimArrayVisible(int session, int component, int sampleName, int parallel) {
	return (*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->visible;
}

bool FourDimArray::getFourDimArrayExist(int session, int component, int sampleName, int parallel) {
	return (*(*(*(fourdimarray + session) + component) + sampleName) + parallel)->exist;
}

void FourDimArray::setStrComponent(int componentOrder, string component) {
	(*(componentArray + componentOrder)) = component;
	return;
}

void FourDimArray::setStrSampleName(int sampleNameOrder, string sampleName) {
	(*(sampleNameArray + sampleNameOrder)) = sampleName;
	return;
}

void FourDimArray::setStrComponent(int componentOrder, int component) {
	string c = to_string(component);
	(*(componentArray + componentOrder)) = c;
	return;
}

void FourDimArray::setStrSampleName(int sampleNameOrder, int sampleName) {
	string sN = to_string(sampleName);
	(*(sampleNameArray + sampleNameOrder)) = sN;
	return;
}

string FourDimArray::getStrComponent(int componentOrder) {
	return (*(componentArray + componentOrder));
}

string FourDimArray::getStrSampleName(int sampleNameOrder) {
	return (*(sampleNameArray + sampleNameOrder));
}

void FourDimArray::pushParallel(int session, int component, int sampleName, float concentration) {
	int parallel = 0;
	while (getFourDimArrayExist(session, component, sampleName, parallel)) {
		parallel++;
	}
	setFourDimArrayConcentration(session, component, sampleName, parallel, concentration);
	setFourDimArrayExist(session, component, sampleName, parallel, true);
	return;
}

FourDimArray::~FourDimArray() {

}
