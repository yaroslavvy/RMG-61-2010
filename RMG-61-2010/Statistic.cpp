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

bool Statistic::cochrenCriteriaCalculate(FourDimArray * cochrenCriteria, FourDimArray * input, FourDimArray * dispersion) {
	//пиши есчо
	return true;// если за текущую иттерацию были исключения дисперсий
	return false;//если исключений дисперсий за текущую иттерацию не было
}

Statistic::~Statistic() {
}
