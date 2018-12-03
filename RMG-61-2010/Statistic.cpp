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
	int statusFlag = 0;
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
					if ((input->getFourDimArrayStatus(s, c, sN, p) != 0) && (input->getFourDimArrayVisible(s, c, sN, p)) && (input->getFourDimArrayExist(s, c, sN, p))) {
						sPtr->setFourDimArrayStatus(s, c, sN, 0, input->getFourDimArrayStatus(s, c, sN, p));
						sPtr->setFourDimArrayExist(s, c, sN, 0, true);
						p = input->getAmountOfParallel();
					}
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
				if ((average->getFourDimArrayVisible(s, c, sN, 0)) && (average->getFourDimArrayExist(s, c, sN, 0)) && (concentrations->getFourDimArrayStatus(0, c, sN, 0) == 0) && (concentrations->getFourDimArrayVisible(0, c, sN, 0)) && (concentrations->getFourDimArrayExist(0, c, sN, 0))) {
					if ((average->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
						sPtr->setFourDimArrayConcentration(s, c, sN, 0, (average->getFourDimArrayConcentration(s, c, sN, 0) - concentrations->getFourDimArrayConcentration(0, c, sN, 0)));
					}
					else {
						sPtr->setFourDimArrayStatus(s, c, sN, 0, (average->getFourDimArrayStatus(s, c, sN, 0)));
					}
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
				if ((average->getFourDimArrayStatus(s, c, sN, 0) != 0) && (average->getFourDimArrayVisible(s, c, sN, 0)) && (average->getFourDimArrayExist(s, c, sN, 0))) {
					sPtr->setFourDimArrayStatus(s, c, sN, 0, (average->getFourDimArrayStatus(s, c, sN, 0)));
					sPtr->setFourDimArrayExist(s, c, sN, 0, true);
				}
				else {
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
	}//самое первое - проверка на необходимость отбора дисперсий по любым критериям (отсутствие или 1 сессия, т.е. отсутствие или 1 дисперсия не могут работать ни с одним из критериев)
	
	for (int s = 0; s < input->getAmountOfSession(); s++) {
		for (int p = 0; p < input->getAmountOfParallel(); p++) {
			if (input->getFourDimArrayStatus(s, component, sampleName, p) != 0) {
				return input->getFourDimArrayStatus(s, component, sampleName, p);
			}
		}
	}// если среди сессий одного компонента и образца уже был критериальный отсев, то необходимо продолжить пользоваться старым критерием вне зависимости от всех остальных факторов
	if ((dispersion->getAmountOfSession(component, sampleName) > 40)) {
		return 2;
	}// если более 40 сессий, то работает только Бартлетт
	int i = 0;
	int *arrayOfParallels = new int[dispersion->getAmountOfSession(component, sampleName)];
	for (int s = 0; s < input->getAmountOfSession(); s++) {
		if (input->getAmountOfParallel(s, component, sampleName) > 1) {
			if (input->getAmountOfParallel(s, component, sampleName) > 6) {
				return 2;
			}// если в какой-то сессии более 6 параллельных испытаний, то работает только Бартлетт
			arrayOfParallels[i] = input->getAmountOfParallel(s, component, sampleName);
			i++;
		}
	}

	if (Statistic::arrayElementsAreEqual(arrayOfParallels, input->getAmountOfSession(component, sampleName))) {
		return 1;// количество параллелей во всех сессиях одинаково, значит Кохрен
	}
	else {
		return 2;// количество параллелей в сессиях неодинаково, значит Бартлетт
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
	int tmpAmountOfParallels = 0;
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
		tmpAmountOfParallels = input->getAmountOfParallel(maxDispersionSession, component, sampleName);
		for (int p = 0; p < tmpAmountOfParallels; p++) {
			input->setFourDimArrayStatus(maxDispersionSession, component, sampleName, p, 1);
		}
		return true;
	}
	return false;
}

bool Statistic::bartlettCriterionCalculate(FourDimArray * input, FourDimArray * dispersion, int component, int sampleName) {
	float averageDispersion = 0;
	float sumOfDispersionLogarithm = 0;
	float sumOfReverseAmountOfParallels = 0;
	int n = 0;
	float m = 0;
	float c = 0;
	float chi = 0;
	int tmpAmountOfParallels = 0;
	for (int s = 0; s < dispersion->getAmountOfSession(); s++) {
		if ((dispersion->getFourDimArrayStatus(s, component, sampleName, 0) == 0) && (dispersion->getFourDimArrayExist(s, component, sampleName, 0))) {
			averageDispersion += (dispersion->getFourDimArrayConcentration(s, component, sampleName, 0))*(input->getAmountOfParallel(s, component, sampleName) - 1);
			sumOfDispersionLogarithm += (log(dispersion->getFourDimArrayConcentration(s, component, sampleName, 0)))*(input->getAmountOfParallel(s, component, sampleName) - 1);
			sumOfReverseAmountOfParallels += (1 / (input->getAmountOfParallel(s, component, sampleName) - 1));
			n += (input->getAmountOfParallel(s, component, sampleName) - 1);
		}
	}
	averageDispersion /= n;
	m = (n * log(averageDispersion)) - sumOfDispersionLogarithm;
	c = 1 + (1 / (3 * (dispersion->getAmountOfSession(component, sampleName) - 1)))*((sumOfReverseAmountOfParallels) - (1/n));
	chi = m / c;

	/*if (chi > chiDistributionValues(dispersion->getAmountOfSession(component, sampleName))) {
		tmpAmountOfParallels = input->getAmountOfParallel(input->getAmountOfParallel(), component, sampleName);
		for (int p = 0; p < tmpAmountOfParallels; p++) {
			input->setFourDimArrayStatus(maxDispersionSession, component, sampleName, p, 1);
		}
		return true;
	}*/
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

float Statistic::chiDistributionValues(int amountOfdipersion) {
	float criticalValues[30] = {5.024, 7.378, 9.348, 11.14, 12.83, 14.45, 16.01, 17.53, 19.02, 20.48, 21.92, 23.34, 24.74, 26.12, 27.49, 28.85, 30.19, 31.53, 32.85, 34.17, 35.48, 36.78, 38.08, 39.36, 40.65, 41.92, 43.19, 44.46, 45.72, 46.98};
	return (amountOfdipersion < 2) && (amountOfdipersion > 31) ? 0 : criticalValues[amountOfdipersion - 2];
}//[2;31] дисперсий

Statistic::~Statistic() {
}
