#include "pch.h"

#include <windows.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <clocale>
#include <cstdlib>
#include <cmath>
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;

#include "Service.h"

int Service::command = 0;
int Service::column = 0;
int Service::row = 0;
int Service::tmp = 0;
string Service::strAnswerDialogMenu = "";
int Service::intAnswerDialogMenu = 0;
string Service::fileName = "";
string Service::item = "";
List * Service::startPtr = NULL;
DataTable * Service::newDataTable = NULL;
struct sessions * Service::arrayOfSessions = NULL;
//DataTable ** Service::sessionDataTablesArray = NULL;
FourDimArray * Service::fourDimArrayPtr = NULL;
FourDimArray * Service::averagefourDimArrayPtr = NULL;


Service::Service()
{
}

void Service::callMenu() {
	while (command != 100) {
		cout << endl << "Possible commands:" << endl << "1 - insert" << endl << "2 - print list" << endl;
		cout << "3 - next row" << endl << "4 - print element" << endl << "5 - save in file " << endl << "6 - read from file" << endl;
		cout << "7 - extract data from structure to table" << endl << "8 - extract data from table to structure" << endl;
		cout << "9 - print table" << endl << "10 - create initial data tables for statistic calculation" << endl << "11 - delete data structure" << endl;
		cout << "12 - identify concentrations" << endl << "13 - calculate average values" << endl << "14 - save the report" << endl;
		cout << "100 - exit" << endl << endl;
		cin >> command;
		cout << endl;
		switch (command)
		{
		case 1:
			cout << "Input value:" << endl;
			cin.get();
			getline(cin, item);
			cout << endl;
			List::insertElement(&startPtr, item);
			break;
		case 2:
			if (isPtrNull(startPtr)) {
				cout << "List is empty" << endl;
				break;
			}
			startPtr->printList(startPtr);
			cout << endl;
			break;
		case 3:
			if (isPtrNull(startPtr)) {
				cout << "List is empty" << endl;
				break;
			}
			startPtr->nextRow();
			cout << endl;
			break;
		case 4:
			if (isPtrNull(startPtr)) {
				cout << "List is empty" << endl;
				break;
			}
			cout << "Input element's column:" << endl;
			cin >> column;
			cout << "Input element's row:" << endl;
			cin >> row;
			startPtr->printElementValue(column, row);
			cout << endl;
			break;
		case 5:
			if (isPtrNull(startPtr)) {
				cout << "List is empty" << endl;
				break;
			}
			cout << "Input file name" << endl;
			cin.get();
			getline(cin, fileName);
			startPtr->saveInFile(startPtr, fileName);
			break;
		case 6:
			if (!isPtrNull(startPtr)) {
				cout << "Warning! Reading from file is deleting the current data structure" << endl;
				cout << "Continue? y/n" << endl;
				cin >> strAnswerDialogMenu;
				if (strAnswerDialogMenu == "y") {
					cout << "Input file name" << endl;
					cin.get();
					getline(cin, fileName);
					delete startPtr;
					startPtr = NULL;
					List::readFromFile(&startPtr, fileName);
					break;
				}
				cout << "Reading from file has been canceled" << endl;
				break;
			}
			cout << "Input file name" << endl;
			cin.get();
			getline(cin, fileName);
			List::readFromFile(&startPtr, fileName);
			break;
		case 7:
			if (isPtrNull(startPtr)) {
				cout << "List is empty" << endl;
				break;
			}
			newDataTable = new DataTable((List::getMaxCommonColumn() - 1), (List::getCommonRow() - 2));
			newDataTable->extractConcentrationsFromFormattedStructureToTable(startPtr);
			break;
		case 8:
			if (!isPtrNull(startPtr)) {
				cout << "Warning! Extracting from table is deleting the current data structure" << endl;
				cout << "Continue? y/n" << endl;
				cin >> strAnswerDialogMenu;
				if (strAnswerDialogMenu == "y") {
					delete startPtr;
					startPtr = NULL;
					newDataTable->extractConcentrationsFromTableToFormattedStructure(&startPtr);
					break;
				}
				cout << "Extracting from table has been canceled" << endl;
				break;
			}
			newDataTable->extractConcentrationsFromTableToFormattedStructure(&startPtr);
			break;
		case 9:
			if (isPtrNull(newDataTable)) {
				cout << "Table is empty" << endl;
				break;
			}
			newDataTable->printDataTable();
			break;
		case 10:
			if (!isPtrNull(arrayOfSessions)) {
				cout << endl << "Warning! Reading from file is deleting the current data tables" << endl;
				cout << "Continue? y/n" << endl;
				cin >> strAnswerDialogMenu;
				if (strAnswerDialogMenu != "y") {
					cout << endl << "Reading from file has been canceled" << endl;
					break;
				}
				cin.get();
			}

			cout << endl << "How many experimental sessions (days) have been carried out (correct value must be > 0)?" << endl << endl;
			cin >> intAnswerDialogMenu;
			while (intAnswerDialogMenu <= 0) {
				cout << "Incorrect value (correct value must be > 0). Try again y/n?" << endl;
				if (strAnswerDialogMenu != "y") {
					cout << "Reading from file has been canceled" << endl;
					break;
				}
				cin >> intAnswerDialogMenu;
			}
			tmp = 1;
			arrayOfSessions = NULL;
			arrayOfSessions = new sessions;
			arrayOfSessions->sessionDataTablesArray = NULL; // как то нужно очищать память от динамически выделенной памяти под массив указателей на объекты DataTable
			arrayOfSessions->amountOfSessions = intAnswerDialogMenu;
			arrayOfSessions->sessionDataTablesArray = new DataTable *[intAnswerDialogMenu];

			cin.get(); // разобрать с танцем с бубном в случае с cin потоком, почему иногда не читает первый символ
			while (tmp <= intAnswerDialogMenu) {
				do {
					cout << endl << "Input name of file which include " << tmp << numberEnding(tmp) << " session data:" << endl;
					getline(cin, fileName);
					startPtr = NULL;
				} while (List::readFromFile(&startPtr, fileName) == false);
				*(arrayOfSessions->sessionDataTablesArray + tmp - 1) = new DataTable((List::getMaxCommonColumn() - 1), (List::getCommonRow() - 2));
				(*(arrayOfSessions->sessionDataTablesArray + tmp - 1))->extractConcentrationsFromFormattedStructureToTable(startPtr);
				delete startPtr;
				startPtr = NULL;
				tmp++;
			}
			tmp = 1;
			while (tmp <= intAnswerDialogMenu) {
				cout << endl << tmp << numberEnding(tmp) << " session data:" << endl;
				(*(arrayOfSessions->sessionDataTablesArray + tmp - 1))->printDataTable();
				tmp++;
			}
			break;
		case 11:
			if (isPtrNull(startPtr)) {
				cout << "List is empty" << endl;
				break;
			}
			delete startPtr;
			startPtr = NULL;
			break;
		case 12:
			fourDimArrayPtr = FourDimArray::extractDataFromTableToFourDimArray(arrayOfSessions);
			cout << endl << "Inner data is:" << endl;
			fourDimArrayPtr->printFourDimArray();
			break;
		case 13:
			if (isPtrNull(fourDimArrayPtr)) {
				cout << "fourDimArray is empty" << endl;
				break;
			}
			averagefourDimArrayPtr = Statistic::averageCalculate(fourDimArrayPtr);
			cout << endl << "Average values among parallels of inner data is:" << endl;
			averagefourDimArrayPtr->printFourDimArray();
			break;
		case 14:
			if (Service::saveReport(fourDimArrayPtr, averagefourDimArrayPtr)) {
				cout << endl << "Report has been saved as report.csv" << endl;
			}
			else {
				cout << endl << "Saving failed!" << endl;
			}
			break;
		case 100:
			break;
		default:
			cout << "Command is undefined, try again" << endl;
		}
	}
	return;
}

template<class T>
bool Service::isPtrNull(T *sPtr) {
	return sPtr == NULL ? true : false;
}

string Service::numberEnding(int number) {
	switch (number) {
	case 1: return "st";
	case 2: return "nd";
	case 3: return "rd";
	default: return "th";
	}
}

bool Service::saveReport(FourDimArray *fourDimArrayPtr, FourDimArray *averagefourDimArrayPtr) {
	ofstream fout;
	fout.open("report.csv");
	if (Service::isPtrNull(fourDimArrayPtr) || Service::isPtrNull(averagefourDimArrayPtr)) {
		return false;
	}
	fout << "Report";// common data
	fout << "\n";
	for (int s = 0; s < fourDimArrayPtr->getAmountOfSession(); s++) {
		fout << s + 1 << " session:" << ";";
		for (int c = 0; c < fourDimArrayPtr->getAmountOfComponent(); c++) {
			fout << fourDimArrayPtr->getStrComponent(c);
			c == (fourDimArrayPtr->getAmountOfComponent() - 1) ? fout << "\n" : fout << ";";
		}
		for (int sN = 0; sN < fourDimArrayPtr->getAmountOfSampleName(); sN++) {
			fout << fourDimArrayPtr->getStrSampleName(sN) << ";";
			for (int p = 0; p < fourDimArrayPtr->getAmountOfParallel(); p++) {
				for (int c = 0; c < fourDimArrayPtr->getAmountOfComponent(); c++) {
					fourDimArrayPtr->getFourDimArrayExist(s, c, sN, p) && fourDimArrayPtr->getFourDimArrayVisible(s, c, sN, p) ? fout << fourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) : fout << "";
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

	fout << "\n" << "\n";// Average values
	fout << "Average values";
	fout << "\n";
	for (int s = 0; s < averagefourDimArrayPtr->getAmountOfSession(); s++) {
		fout << s + 1 << " session:" << ";";
		for (int c = 0; c < averagefourDimArrayPtr->getAmountOfComponent(); c++) {
			fout << averagefourDimArrayPtr->getStrComponent(c);
			c == (averagefourDimArrayPtr->getAmountOfComponent() - 1) ? fout << "\n" : fout << ";";
		}
		for (int sN = 0; sN < averagefourDimArrayPtr->getAmountOfSampleName(); sN++) {
			fout << averagefourDimArrayPtr->getStrSampleName(sN) << ";";
			for (int p = 0; p < averagefourDimArrayPtr->getAmountOfParallel(); p++) {
				for (int c = 0; c < averagefourDimArrayPtr->getAmountOfComponent(); c++) {
					averagefourDimArrayPtr->getFourDimArrayExist(s, c, sN, p) && averagefourDimArrayPtr->getFourDimArrayVisible(s, c, sN, p) ? fout << averagefourDimArrayPtr->getFourDimArrayConcentration(s, c, sN, p) : fout << "";
					if (c < (averagefourDimArrayPtr->getAmountOfComponent() - 1)) {
						fout << ";";
					}
				}
				if (averagefourDimArrayPtr->getAmountOfParallel() != 1) {
					fout << "\n" << ";";
				}
			}
			fout << "\n";
		}
		if (averagefourDimArrayPtr->getAmountOfParallel() == 1) {
			fout << "\n";
		}
	}

	fout.close();
	return true;
}

Service::~Service()
{
}

/*fout << "Average values";
for (int i = 0; i < averagefourDimArrayPtr->getAmountOfComponent(); i++) {
	fout << ";";
}*/