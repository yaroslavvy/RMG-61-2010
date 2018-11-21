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
				if (parallelCounter == 0) {
					sPtr->setFourDimArrayConcentration(s, c, sN, 0, 0);
					sPtr->setFourDimArrayStatus(s, c, sN, 0, 0);
					sPtr->setFourDimArrayVisible(s, c, sN, 0, false);
					sPtr->setFourDimArrayExist(s, c, sN, 0, false);
				}
				else {
					sPtr->setFourDimArrayConcentration(s, c, sN, 0, (total / parallelCounter));
					sPtr->setFourDimArrayStatus(s, c, sN, 0, 0);
					sPtr->setFourDimArrayVisible(s, c, sN, 0, true);
					sPtr->setFourDimArrayExist(s, c, sN, 0, true);
				}
			}
		}
	}
	return sPtr;
}

Statistic::~Statistic() {
}
