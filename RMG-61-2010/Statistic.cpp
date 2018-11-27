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

#include "Statistic.h"

Statistic::Statistic() {
}

FourDimArray * Statistic::averageCalculate(FourDimArray * input) {
	float total = 0;
	int parallelCounter = 0;
	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(input->getAmountOfSession(), input->getAmountOfComponent(), input->getAmountOfSampleName(), 1);

	for (int c = 0; c < input->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (input->getStrComponent(c)));
	}

	for (int sN = 0; sN < input->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (input->getStrSampleName(sN)));
	}

	for (int s = 0; s < input->getAmountOfSession(); s++) {
		for (int c = 0; c < input->getAmountOfComponent(); c++) {
			for (int sN = 0; sN < input->getAmountOfSampleName(); sN++) {
				total = 0;
				parallelCounter = 0;
				for (int p = 0; p < input->getAmountOfParallel(); p++) {
					if ((input->getFourDimArrayStatus(s, c, sN, p) == 0) && (input->getFourDimArrayVisible(s, c, sN, p)) && (input->getFourDimArrayExist(s, c, sN, p))) {
						total += input->getFourDimArrayConcentration(s, c, sN, p);
						parallelCounter++;
					}
				}
				if (parallelCounter > 0) {
					sPtr->setFourDimArrayConcentration(s, c, sN, 0, (total / parallelCounter));
					sPtr->setFourDimArrayExist(s, c, sN, 0, true);
				}
			}
		}
	}
	return sPtr;
}

FourDimArray * Statistic::biasCalculate(FourDimArray * average, FourDimArray * concentrations) {
	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(average->getAmountOfSession(), average->getAmountOfComponent(), average->getAmountOfSampleName(), 1);

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (average->getStrComponent(c)));
	}

	for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (average->getStrSampleName(sN)));
	}

	for (int s = 0; s < average->getAmountOfSession(); s++) {
		for (int c = 0; c < average->getAmountOfComponent(); c++) {
			for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
				if ((average->getFourDimArrayStatus(s, c, sN, 0) == 0) && (average->getFourDimArrayVisible(s, c, sN, 0)) && (average->getFourDimArrayExist(s, c, sN, 0)) && (concentrations->getFourDimArrayStatus(0, c, sN, 0) == 0) && (concentrations->getFourDimArrayVisible(0, c, sN, 0)) && (concentrations->getFourDimArrayExist(0, c, sN, 0))) {
					sPtr->setFourDimArrayConcentration(s, c, sN, 0, (average->getFourDimArrayConcentration(s, c, sN, 0) - concentrations->getFourDimArrayConcentration(0, c, sN, 0)));
					sPtr->setFourDimArrayExist(s, c, sN, 0, true);
				}
			}
		}
	}

	return sPtr;
}

FourDimArray * Statistic::dispersionCalculate(FourDimArray * input, FourDimArray * average) {
	int parallelCounter = 0;
	float totalSumOfSqr = 0;
	float isEqual = 0;
	bool equalFlag = true;
	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(input->getAmountOfSession(), input->getAmountOfComponent(), input->getAmountOfSampleName(), 1);

	for (int c = 0; c < input->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (input->getStrComponent(c)));
	}

	for (int sN = 0; sN < input->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (input->getStrSampleName(sN)));
	}

	for (int s = 0; s < input->getAmountOfSession(); s++) {
		for (int c = 0; c < input->getAmountOfComponent(); c++) {
			for (int sN = 0; sN < input->getAmountOfSampleName(); sN++) {
				parallelCounter = 0;
				totalSumOfSqr = 0;
				isEqual = 0;
				equalFlag = true;
				for (int p = 0; p < input->getAmountOfParallel(); p++) {
					if ((input->getFourDimArrayStatus(s, c, sN, p) == 0) && (input->getFourDimArrayVisible(s, c, sN, p)) && (input->getFourDimArrayExist(s, c, sN, p)) && (average->getFourDimArrayStatus(s, c, sN, 0) == 0) && (average->getFourDimArrayVisible(s, c, sN, 0)) && (average->getFourDimArrayExist(s, c, sN, 0))) {
						if ((p != 0) && (equalFlag)) {
							if (isEqual != input->getFourDimArrayConcentration(s, c, sN, p)) {
								equalFlag = false;
							}
						}
						isEqual = input->getFourDimArrayConcentration(s, c, sN, p);
						totalSumOfSqr += pow(average->getFourDimArrayConcentration(s, c, sN, 0) - input->getFourDimArrayConcentration(s, c, sN, p), 2);
						parallelCounter++;
					}
				}
				if (parallelCounter >= 2) {
					sPtr->setFourDimArrayConcentration(s, c, sN, 0, (equalFlag ? 0 : (totalSumOfSqr / (parallelCounter - 1))));
					sPtr->setFourDimArrayExist(s, c, sN, 0, true);
				}
			}
		}
	}
	return sPtr;
}

bool Statistic::cochranAndBartlettCriterionCalculate(FourDimArray * input, FourDimArray * dispersion) {
	bool changes = false;
	for (int c = 0; c < input->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < input->getAmountOfSampleName(); sN++) {
			switch (Statistic::selectCochranOrBartlettCriterion(input, dispersion, c, sN)) {
				case 1: 
					changes |= Statistic::cochranCriterionCalculate(input, dispersion, c, sN);
					break;
				case 2:
					changes |= Statistic::bartlettCriterionCalculate(input, dispersion, c, sN);
					break;
				default: 
					break;
			}
		}
	}
	return changes;
}

int Statistic::selectCochranOrBartlettCriterion(FourDimArray * input, FourDimArray * dispersion, int component, int sampleName) {
	if (dispersion->getAmountOfSession(component, sampleName) < 2) {
		return 0;
	}//����� ������ - �������� �� ������������� ������ ��������� �� ����� ��������� (���������� ��� 1 ������, �.�. ���������� ��� 1 ��������� �� ����� �������� �� � ����� �� ���������)
	
	for (int s = 0; s < input->getAmountOfSession(); s++) {
		for (int p = 0; p < input->getAmountOfParallel(); p++) {
			if (input->getFourDimArrayStatus(s, component, sampleName, p) != 0) {
				return input->getFourDimArrayStatus(s, component, sampleName, p);
			}
		}
	}// ���� ����� ������ ������ ���������� � ������� ��� ��� ������������� �����, �� ���������� ���������� ������������ ������ ��������� ��� ����������� �� ���� ��������� ��������
	if ((dispersion->getAmountOfSession(component, sampleName) > 40)) {
		return 2;
	}// ���� ����� 40 ������, �� �������� ������ ��������
	int i = 0;
	int *arrayOfParallels = new int[dispersion->getAmountOfSession(component, sampleName)];
	for (int s = 0; s < input->getAmountOfSession(); s++) {
		if (input->getAmountOfParallel(s, component, sampleName) > 1) {
			if (input->getAmountOfParallel(s, component, sampleName) > 6) {
				return 2;
			}// ���� � �����-�� ������ ����� 6 ������������ ���������, �� �������� ������ ��������
			arrayOfParallels[i] = input->getAmountOfParallel(s, component, sampleName);
			i++;
		}
	}

	if (Statistic::arrayElementsAreEqual(arrayOfParallels, input->getAmountOfSession(component, sampleName))) {
		return 1;// ���������� ���������� �� ���� ������� ���������, ������ ������
	}
	else {
		return 2;// ���������� ���������� � ������� �����������, ������ ��������
	}
}

bool Statistic::arrayElementsAreEqual(int *array, int length) {
	for (int i = 1; i < length; i++) {
		if (array[i - 1] != array[i]) {
			return false;
		}
	}
	return true;
}

bool Statistic::cochranCriterionCalculate(FourDimArray * input, FourDimArray * dispersion, int component, int sampleName) {
	float maxDispersion = 0;
	int maxDispersionSession = 0;
	float totalSumOfDispersions = 0;
	int amountOfParallels = 0;
	for (int s = 0; s < dispersion->getAmountOfSession(); s++) {
		if (((dispersion->getFourDimArrayConcentration(s, component, sampleName, 0)) > maxDispersion) && ((dispersion->getFourDimArrayStatus(s, component, sampleName, 0)) == 0) && (dispersion->getFourDimArrayExist(s, component, sampleName, 0))) {
			maxDispersion = (dispersion->getFourDimArrayConcentration(s, component, sampleName, 0));
			maxDispersionSession = s;
		}
	}
	for (int s = 0; s < dispersion->getAmountOfSession(); s++) {
		if (((dispersion->getFourDimArrayStatus(s, component, sampleName, 0)) == 0) && (dispersion->getFourDimArrayExist(s, component, sampleName, 0))) {
			totalSumOfDispersions += (dispersion->getFourDimArrayConcentration(s, component, sampleName, 0));
		}
	}
	for (int s = 0; s < input->getAmountOfSession(); s++) {
		if (input->getAmountOfParallel(s, component, sampleName) > 1) {
			amountOfParallels = input->getAmountOfParallel(s, component, sampleName);
			s = input->getAmountOfSession();
		}
	}
	if ((maxDispersion / totalSumOfDispersions) > Statistic::cochranCriticalValues((dispersion->getAmountOfSession(component, sampleName)), amountOfParallels)) {
		for (int p = 0; p < input->getAmountOfParallel(/*maxDispersionSession, component, sampleName*/); p++) {// ������� ���������� ������� �� �������� ��� ����
			input->setFourDimArrayStatus(maxDispersionSession, component, sampleName, p, 1);
		}
		return true;
	}
	return false;
}

bool Statistic::bartlettCriterionCalculate(FourDimArray * input, FourDimArray * dispersion, int component, int sampleName) {
	
	return false;
}

float Statistic::cochranCriticalValues(int amountOfSessions, int amountOfParallels) {
	float criticalValues[6][5] = {
		{0.999, 0.975, 0.939, 0.906, 0.877},
		{0.967, 0.871, 0.798, 0.746, 0.707},
		{0.906, 0.768, 0.684, 0.629, 0.590},
		{0.841, 0.684, 0.598, 0.544, 0.506},
		{0.781, 0.616, 0.532, 0.480, 0.445},
		{0.727, 0.561, 0.480, 0.431, 0.397}
	};
	return criticalValues[amountOfSessions - 2][amountOfParallels - 2];
}

Statistic::~Statistic() {
}
