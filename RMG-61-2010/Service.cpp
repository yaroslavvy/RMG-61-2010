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
#include <memory>

#include "Service.h"

Service::Service(){
}

void Service::callMenu() {
	string strAnswerDialogMenu = "";
	cout << endl << "How many experimental sessions (days) have been carried out (correct value must be > 0)?" << endl << endl;
	getline(cin, strAnswerDialogMenu);
	while ((!isIntegerFromString(strAnswerDialogMenu)) || (stoi(strAnswerDialogMenu) <= 0)) {
		cout << "Incorrect value (correct value must be integer > 0). Try again y/n?" << endl;
		getline(cin, strAnswerDialogMenu);
		if (strAnswerDialogMenu != "y") {
			cout << "Reading from file has been canceled" << endl;
			return;
		}
		cout << endl << "How many experimental sessions (days) have been carried out (correct value must be > 0)?" << endl << endl;
		getline(cin, strAnswerDialogMenu);
	}
	int amountOfSessions = stoi(strAnswerDialogMenu);

	struct sessions * arrayOfSessions = new sessions;
	arrayOfSessions->sessionDataTablesArray = new DataTable *[amountOfSessions];
	arrayOfSessions->crmConcentraions = NULL;
	arrayOfSessions->crmUncertainties = NULL;
	arrayOfSessions->amountOfSessions = amountOfSessions;

	string fileName = "";
	List * startPtr = NULL;
	for (int i = 0; i < amountOfSessions; i++) {
		do {
			cout << endl << "Input name of file which includes " << i + 1 << numberEnding(i + 1) << " session data:" << endl;
			getline(cin, fileName);
			startPtr = NULL;
		} while (List::readFromFile(&startPtr, fileName) == false);
		arrayOfSessions->sessionDataTablesArray[i] = new DataTable((List::getMaxCommonColumn() - 1), (List::getCommonRow() - 2));
		(arrayOfSessions->sessionDataTablesArray[i])->extractConcentrationsFromFormattedStructureToTable(startPtr);
		delete startPtr;
		startPtr = NULL;
	}

	do {
		cout << endl << "Input name of file which includes concentrations of CRMs:" << endl;
		getline(cin, fileName);
		startPtr = NULL;
	} while (List::readFromFile(&startPtr, fileName) == false);
	arrayOfSessions->crmConcentraions = new DataTable((List::getMaxCommonColumn() - 1), (List::getCommonRow() - 2));
	arrayOfSessions->crmConcentraions->extractConcentrationsFromFormattedStructureToTable(startPtr);
	delete startPtr;
	startPtr = NULL;

	do {
		cout << endl << "Input name of file which includes uncertainties of CRM's concentrations:" << endl;
		getline(cin, fileName);
		startPtr = NULL;
	} while (List::readFromFile(&startPtr, fileName) == false);
	arrayOfSessions->crmUncertainties = new DataTable((List::getMaxCommonColumn() - 1), (List::getCommonRow() - 2));
	arrayOfSessions->crmUncertainties->extractConcentrationsFromFormattedStructureToTable(startPtr);
	delete startPtr;
	startPtr = NULL;

	cout << endl << "Input amount of parallel values (observed values) which will be included in test result? (1-5)" << endl;
	getline(cin, strAnswerDialogMenu);
	while ((!isIntegerFromString(strAnswerDialogMenu)) || (stoi(strAnswerDialogMenu) < 1) || (stoi(strAnswerDialogMenu) > 5)) {
		cout << "Incorrect value. Input please value from 1 to 5. Try again y/n?" << endl;
		getline(cin, strAnswerDialogMenu);
		if (strAnswerDialogMenu != "y") {
			cout << "Setting of amount of parallel values has been canceled" << endl;
			return;
		}
		cout << endl << "Input amount of parallel values (observed values) which will be included in test result? (1-5)" << endl << endl;
		getline(cin, strAnswerDialogMenu);
	}
	int amountOfParallelsInTestResult = stoi(strAnswerDialogMenu);

	FourDimArray * fourDimArrayPtr = FourDimArray::extractDataFromTableToFourDimArray(arrayOfSessions);
	fourDimArrayPtr->setDescription("Common formated data");

	FourDimArray * fourDimArrayConcentrationsPtr = new FourDimArray(1, fourDimArrayPtr->getAmountOfComponent(), fourDimArrayPtr->getAmountOfSampleName(), 1);
	fourDimArrayConcentrationsPtr->pasteValuesInCopyFormat(fourDimArrayPtr, arrayOfSessions->crmConcentraions, NULL);
	fourDimArrayConcentrationsPtr->setDescription("Concentrations of CRMs");

	FourDimArray * fourDimArrayUncertaintiesPtr = new FourDimArray(1, fourDimArrayPtr->getAmountOfComponent(), fourDimArrayPtr->getAmountOfSampleName(), 1);
	fourDimArrayUncertaintiesPtr->pasteValuesInCopyFormat(fourDimArrayPtr, arrayOfSessions->crmUncertainties, fourDimArrayConcentrationsPtr);
	fourDimArrayUncertaintiesPtr->setDescription("Uncertainties of CRM's concentrations");

	FourDimArray * averageFourDimArrayPtr = NULL;
	FourDimArray * dispersionFourDimArrayPtr = NULL;

	do {
		delete averageFourDimArrayPtr;
		averageFourDimArrayPtr = NULL;
		averageFourDimArrayPtr = Statistic::averageCalculate(fourDimArrayPtr);
		averageFourDimArrayPtr->setDescription("Average values");

		delete dispersionFourDimArrayPtr;
		dispersionFourDimArrayPtr = NULL;
		dispersionFourDimArrayPtr = Statistic::dispersionCalculate(fourDimArrayPtr, averageFourDimArrayPtr);
		dispersionFourDimArrayPtr->setDescription("Dispersion values");

	} while (Statistic::cochranAndBartlettCriterionCalculate(fourDimArrayPtr, dispersionFourDimArrayPtr));

	FourDimArray * possibleOutlierFourDimArrayPtr = Statistic::possibleOutlierReport(fourDimArrayPtr);
	possibleOutlierFourDimArrayPtr->setDescription("Possible outliers among dispersions");

	FourDimArray * repeatabilityFourDimArrayPtr = Statistic::repeatabilityCalculate(dispersionFourDimArrayPtr);
	repeatabilityFourDimArrayPtr->setDescription("Repeatability");

	FourDimArray * repeatabilityLimitFourDimArrayPtr = Statistic::repeatabilityLimitCalculate(repeatabilityFourDimArrayPtr, amountOfParallelsInTestResult);
	repeatabilityLimitFourDimArrayPtr->setDescription("Repeatability limit with " + std::to_string(amountOfParallelsInTestResult) + (amountOfParallelsInTestResult == 1 ? " parallel value" : " parallel values"));
		
	FourDimArray * fourDimArrayGrubbsCriterionPtr = FourDimArray::copyFourDimArray(fourDimArrayPtr);
	fourDimArrayGrubbsCriterionPtr->setDescription("Common formated data with Grubbs criterion");

	FourDimArray * averageFourDimArrayGrubbsCriterionPtr = FourDimArray::copyFourDimArray(averageFourDimArrayPtr);
	averageFourDimArrayGrubbsCriterionPtr->setDescription("Average values with Grubbs criterion");

	while (Statistic::grubbsCriterionCalculate(fourDimArrayGrubbsCriterionPtr, averageFourDimArrayGrubbsCriterionPtr)) {

		delete averageFourDimArrayGrubbsCriterionPtr;
		averageFourDimArrayGrubbsCriterionPtr = NULL;
		averageFourDimArrayGrubbsCriterionPtr = Statistic::averageCalculate(fourDimArrayGrubbsCriterionPtr);
		averageFourDimArrayGrubbsCriterionPtr->setDescription("Average values with Grubbs criterion");

	}

	FourDimArray * averageOfAveragesFourDimArrayGrubbsCriterionPtr = Statistic::averageOfAveragesCalculate(averageFourDimArrayGrubbsCriterionPtr);
	averageOfAveragesFourDimArrayGrubbsCriterionPtr->setDescription("Average values amount sessions with Grubbs criterion");

	FourDimArray * biasFourDimArrayGrubbsCriterionPtr = Statistic::biasCalculate(averageOfAveragesFourDimArrayGrubbsCriterionPtr, fourDimArrayConcentrationsPtr);
	biasFourDimArrayGrubbsCriterionPtr->setDescription("Bias values with Grubbs criterion");

	FourDimArray * reproducibilityFourDimArrayPtr = Statistic::reproducibilityCalculate(fourDimArrayGrubbsCriterionPtr, averageFourDimArrayGrubbsCriterionPtr, averageOfAveragesFourDimArrayGrubbsCriterionPtr, repeatabilityFourDimArrayPtr, amountOfParallelsInTestResult);
	reproducibilityFourDimArrayPtr->setDescription("Reproducibility with " + std::to_string(amountOfParallelsInTestResult) + (amountOfParallelsInTestResult == 1 ? " parallel value" : " parallel values"));

	FourDimArray * reproducibilityLimitFourDimArrayPtr = Statistic::reproducibilityLimitCalculate(reproducibilityFourDimArrayPtr, amountOfParallelsInTestResult);
	reproducibilityLimitFourDimArrayPtr->setDescription("Reproducibility limit with " + std::to_string(amountOfParallelsInTestResult) + (amountOfParallelsInTestResult == 1 ? " parallel value" : " parallel values"));

	FourDimArray * studentTTestFourDimArrayPtr = Statistic::studentTTestCalculate(averageFourDimArrayGrubbsCriterionPtr, averageOfAveragesFourDimArrayGrubbsCriterionPtr, biasFourDimArrayGrubbsCriterionPtr, fourDimArrayUncertaintiesPtr);
	studentTTestFourDimArrayPtr->setDescription("Student's t-test");

	FourDimArray * truenessFourDimArrayPtr = Statistic::truenessCalculate(averageFourDimArrayGrubbsCriterionPtr, averageOfAveragesFourDimArrayGrubbsCriterionPtr, biasFourDimArrayGrubbsCriterionPtr, fourDimArrayUncertaintiesPtr, studentTTestFourDimArrayPtr);
	truenessFourDimArrayPtr->setDescription("Trueness");

	FourDimArray * accuracyFourDimArrayPtr = Statistic::accuracyCalculate(averageFourDimArrayGrubbsCriterionPtr, averageOfAveragesFourDimArrayGrubbsCriterionPtr, biasFourDimArrayGrubbsCriterionPtr, fourDimArrayUncertaintiesPtr, reproducibilityFourDimArrayPtr, studentTTestFourDimArrayPtr);
	accuracyFourDimArrayPtr->setDescription("Accuracy");

	if (Service::saveReport(fourDimArrayPtr, fourDimArrayConcentrationsPtr, fourDimArrayUncertaintiesPtr, averageFourDimArrayPtr, dispersionFourDimArrayPtr, possibleOutlierFourDimArrayPtr, repeatabilityFourDimArrayPtr, repeatabilityLimitFourDimArrayPtr, fourDimArrayGrubbsCriterionPtr, averageFourDimArrayGrubbsCriterionPtr, reproducibilityFourDimArrayPtr, reproducibilityLimitFourDimArrayPtr, studentTTestFourDimArrayPtr, truenessFourDimArrayPtr, accuracyFourDimArrayPtr, NULL)) {
		cout << endl << "Report has been saved as report.csv" << endl;
	}
	else {
		cout << endl << "Saving failed!" << endl;
	}

	for (int i = 0; i < amountOfSessions; i++) {
		delete arrayOfSessions->sessionDataTablesArray[i];
	}
	/*delete arrayOfSessions->crmConcentraions;
	delete arrayOfSessions->crmUncertainties;
	delete arrayOfSessions;
	delete fourDimArrayPtr;
	delete fourDimArrayConcentrationsPtr;
	delete fourDimArrayUncertaintiesPtr;
	delete averageFourDimArrayPtr;
	delete dispersionFourDimArrayPtr;
	delete possibleOutlierFourDimArrayPtr;
	delete repeatabilityFourDimArrayPtr;
	delete repeatabilityLimitFourDimArrayPtr;
	delete fourDimArrayGrubbsCriterionPtr;
	delete averageFourDimArrayGrubbsCriterionPtr;
	delete averageOfAveragesFourDimArrayGrubbsCriterionPtr;
	delete biasFourDimArrayGrubbsCriterionPtr;
	delete reproducibilityFourDimArrayPtr;
	delete reproducibilityLimitFourDimArrayPtr;
	delete studentTTestFourDimArrayPtr;
	delete truenessFourDimArrayPtr;
	delete accuracyFourDimArrayPtr;
	return;*/
}

string Service::numberEnding(const int number) {
	switch (number) {
	case 1: return "st";
	case 2: return "nd";
	case 3: return "rd";
	default: return "th";
	}
}

bool Service::saveReport(FourDimArray* fourDimArrayPtr, ...) {
	
	if (fourDimArrayPtr) {
		return false;
	}

	ofstream fout;
	fout.open("report.csv");

	FourDimArray** ptr = &fourDimArrayPtr;
	while (*ptr != NULL) {
		writeOneFourDimArray(*ptr, fout);
		++ptr;
	}
	
	fout.close();
	return true;
}

void Service::writeOneFourDimArray(FourDimArray *fourDimArrayPtr, ofstream & fout) {
	fout << "\n";
	fout << fourDimArrayPtr->getDescription();
	fout << "\n";
	for (int s = 0; s < fourDimArrayPtr->getAmountOfSession(); s++) {
		fourDimArrayPtr->getAmountOfSession() == 1 ? fout << ";" : fout << s + 1 << " session:" << ";";
		for (int c = 0; c < fourDimArrayPtr->getAmountOfComponent(); c++) {
			fout << fourDimArrayPtr->getStrComponent(c);
			c == (fourDimArrayPtr->getAmountOfComponent() - 1) ? fout << "\n" : fout << ";";
		}
		for (int sN = 0; sN < fourDimArrayPtr->getAmountOfSampleName(); sN++) {
			fout << fourDimArrayPtr->getStrSampleName(sN) << ";";
			for (int p = 0; p < fourDimArrayPtr->getAmountOfParallel(); p++) {
				for (int c = 0; c < fourDimArrayPtr->getAmountOfComponent(); c++) {
					if (fourDimArrayPtr->getFourDimArrayExist(s, c, sN, p) && (fourDimArrayPtr->getFourDimArrayStatus(s, c, sN, p) == 0)) {
						fout << fourDimArrayPtr->getMessageWithNullStatus(s, c, sN, p) << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p);
					}
					else {
						if (fourDimArrayPtr->getFourDimArrayExist(s, c, sN, p) && (fourDimArrayPtr->getFourDimArrayStatus(s, c, sN, p) != 0)) {
							switch (fourDimArrayPtr->getFourDimArrayStatus(s, c, sN, p)) {
								case 1: fout << "Cochran(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								case 2: fout << "Bartlett(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								case 10: fout << "Cochran*(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								case 20: fout << "Bartlett*(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								case 3: fout << "1 parallel";
									break;
								case 41: fout << "Grubbs Min(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								case 42: fout << "Grubbs Max(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								case 43: fout << "Grubbs incorrect amount of parallel (s < 3, s > 41)(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								case 5: fout << "Incorrect amount of exist sessions for Student's t-test calculation (s < 2 s > 31)(" << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) << ")";
									break;
								default:
									break;
							}
						}
						else {
							fout << "";
						}
					}
					if (c < (fourDimArrayPtr->getAmountOfComponent() - 1)) {
						fout << ";";
					}
				}
				if (fourDimArrayPtr->getAmountOfParallel() != 1) {
					fout << "\n" << ";";
				}
			}
			fout << "\n";
		}
		if (fourDimArrayPtr->getAmountOfParallel() == 1) {
			fout << "\n";
		}
	}
}

bool Service::isIntegerFromString(const string possibleInteger) {
	return possibleInteger.find_first_not_of("0123456789") == string::npos;
}

Service::~Service()
{
}