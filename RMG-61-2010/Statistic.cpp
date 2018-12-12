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
					if (input->getFourDimArrayExist(s, c, sN, p)) {
						sPtr->setFourDimArrayStatus(s, c, sN, 0, input->getFourDimArrayStatus(s, c, sN, p));
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

FourDimArray * Statistic::biasCalculate(FourDimArray * averageOfAverages, FourDimArray * concentrations) {

	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, averageOfAverages->getAmountOfComponent(), averageOfAverages->getAmountOfSampleName(), 1);

	for (int c = 0; c < averageOfAverages->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (averageOfAverages->getStrComponent(c)));
	}

	for (int sN = 0; sN < averageOfAverages->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (averageOfAverages->getStrSampleName(sN)));
	}

	for (int c = 0; c < averageOfAverages->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < averageOfAverages->getAmountOfSampleName(); sN++) {
			if ((concentrations->getFourDimArrayExist(0, c, sN, 0)) && (averageOfAverages->getFourDimArrayExist(0, c, sN, 0))) {
				sPtr->setFourDimArrayConcentration(0, c, sN, 0, averageOfAverages->getFourDimArrayConcentration(0, c, sN, 0) - concentrations->getFourDimArrayConcentration(0, c, sN, 0));
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
				sPtr->setFourDimArrayStatus(0, c, sN, 0, averageOfAverages->getFourDimArrayStatus(0, c, sN, 0));
			}
		}
	}	
	return sPtr;
}

FourDimArray * Statistic::averageOfAveragesCalculate(FourDimArray * average) {
	float totalSumOfAverages = 0;
	int averageCounter = 0;

	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, average->getAmountOfComponent(), average->getAmountOfSampleName(), 1);

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (average->getStrComponent(c)));
	}

	for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (average->getStrSampleName(sN)));
	}

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
			totalSumOfAverages = 0;
			averageCounter = 0;
			for (int s = 0; s < average->getAmountOfSession(); s++) {
				if ((average->getFourDimArrayExist(s, c, sN, 0)) && (average->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
					totalSumOfAverages += average->getFourDimArrayConcentration(s, c, sN, 0);
					averageCounter++;
				}
			}
			if ((average->getAmountOfSession(c, sN) > 1) && (average->getAmountOfSession(c, sN) <= 31)) {
				sPtr->setFourDimArrayConcentration(0, c, sN, 0, (totalSumOfAverages / averageCounter));
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
			}
			if ((average->getAmountOfSession(c, sN) == 1) || (average->getAmountOfSession(c, sN) > 31)) {
				sPtr->setFourDimArrayConcentration(0, c, sN, 0, (totalSumOfAverages / averageCounter));
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
				sPtr->setFourDimArrayStatus(0, c, sN, 0, 5);
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
				if (average->getFourDimArrayExist(s, c, sN, 0)) {
					sPtr->setFourDimArrayStatus(s, c, sN, 0, (average->getFourDimArrayStatus(s, c, sN, 0)));
					parallelCounter = 0;
					totalSumOfSqr = 0;
					isEqual = 0;
					equalFlag = true;
					for (int p = 0; p < input->getAmountOfParallel(); p++) {
						if (input->getFourDimArrayExist(s, c, sN, p)) {
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
	float maxDispersion = 0;
	int maxSession = 0;
	int tmpAmountOfParallels = 0;
	for (int s = 0; s < dispersion->getAmountOfSession(); s++) {
		if ((dispersion->getFourDimArrayStatus(s, component, sampleName, 0) == 0) && (dispersion->getFourDimArrayExist(s, component, sampleName, 0))) {
			averageDispersion += (dispersion->getFourDimArrayConcentration(s, component, sampleName, 0))*(input->getAmountOfParallel(s, component, sampleName) - 1);
			sumOfDispersionLogarithm += (log(dispersion->getFourDimArrayConcentration(s, component, sampleName, 0)))*(input->getAmountOfParallel(s, component, sampleName) - 1);
			sumOfReverseAmountOfParallels += (1 / (input->getAmountOfParallel(s, component, sampleName) - 1));
			n += (input->getAmountOfParallel(s, component, sampleName) - 1);
			if (dispersion->getFourDimArrayConcentration(s, component, sampleName, 0) > maxDispersion) {
				maxDispersion = dispersion->getFourDimArrayConcentration(s, component, sampleName, 0);
				maxSession = s;
			}
		}
	}
	averageDispersion /= n;
	m = (n * log(averageDispersion)) - sumOfDispersionLogarithm;
	c = 1 + (1 / (3 * (dispersion->getAmountOfSession(component, sampleName) - 1)))*((sumOfReverseAmountOfParallels) - (1/n));
	chi = m / c;

	if (chi > chiDistributionValues(dispersion->getAmountOfSession(component, sampleName))) {
		tmpAmountOfParallels = input->getAmountOfParallel(maxSession, component, sampleName);
		for (int p = 0; p < tmpAmountOfParallels; p++) {
				input->setFourDimArrayStatus(maxSession, component, sampleName, p, 2);
		}
		return true;
	}
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

FourDimArray * Statistic::possibleOutlierReport(FourDimArray * input) {
	int outlierCounter = 0;
	int outlierparallel = 0;
	int outlierStatus = 0;
	string content = "";
	string criterion = "";
	for (int s = 0; s < input->getAmountOfSession(); s++) {
		for (int c = 0; c < input->getAmountOfComponent(); c++) {
			for (int sN = 0; sN < input->getAmountOfSampleName(); sN++) {
				for (int p = 0; p < input->getAmountOfParallel(); p++) {
					if (input->getFourDimArrayStatus(s, c, sN, p) != 0) {
						p = input->getAmountOfParallel();
						outlierCounter++;
					}
				}
			}
		}
	}
	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, 1, outlierCounter, 0);
	sPtr->setStrComponent(0, "");
	outlierCounter = 0;
	for (int s = 0; s < input->getAmountOfSession(); s++) {
		for (int c = 0; c < input->getAmountOfComponent(); c++) {
			for (int sN = 0; sN < input->getAmountOfSampleName(); sN++) {
				for (int p = 0; p < input->getAmountOfParallel(); p++) {
					if ((input->getFourDimArrayStatus(s, c, sN, p) == 1) || (input->getFourDimArrayStatus(s, c, sN, p) == 10)) {
						criterion = ", Criterion: Cochran";
						outlierStatus = 10;
					}
					if ((input->getFourDimArrayStatus(s, c, sN, p) == 2) || (input->getFourDimArrayStatus(s, c, sN, p) == 20)) {
						criterion = ", Criterion: Bartlett";
						outlierStatus = 20;
					}
					if ((input->getFourDimArrayStatus(s, c, sN, p) != 0) && (input->getAmountOfParallel(s, c, sN, "withoutStatus") > 2)) {
						outlierparallel = Statistic::findOutlier(input, s, c, sN);
						sPtr->setStrSampleName(outlierCounter, "Session №" + to_string(s + 1) + ", " + "sample: " + input->getStrSampleName(sN) + ", " + "component: " + input->getStrComponent(c) + ", " + "parallel №" + to_string(outlierparallel + 1) + ", concentration: " + to_string(input->getFourDimArrayConcentration(s, c, sN, outlierparallel)) + criterion );
						input->setFourDimArrayStatus(s, c, sN, outlierparallel, outlierStatus);
						outlierCounter++;
						p = input->getAmountOfParallel();
					}
					if ((input->getFourDimArrayStatus(s, c, sN, p) != 0) && (input->getAmountOfParallel(s, c, sN, "withoutStatus") == 2)) {
						sPtr->setStrSampleName(outlierCounter, "Session №" + to_string(s + 1) + ", " + "sample: " + input->getStrSampleName(sN) + ", " + "component: " + input->getStrComponent(c) + ", " + "one among two parallels may be uncorrect, concentrations: " + to_string(input->getFourDimArrayConcentration(s, c, sN, 0)) + ", " + to_string(input->getFourDimArrayConcentration(s, c, sN, 1)) + criterion);
						input->setFourDimArrayStatus(s, c, sN, outlierparallel, outlierStatus);
						outlierCounter++;
						p = input->getAmountOfParallel();
					}
				}
			}
		}
	}
	return sPtr;
}

int Statistic::findOutlier(FourDimArray * input, int session, int component, int sampleName) {
	float *arrayAverage = new float[input->getAmountOfParallel()];
	OneDimArray *arrayDispersion = new OneDimArray[input->getAmountOfParallel()];
	float totalSum = 0;
	float totalSumSqr = 0;
	int countOfParallels = 0;
	float minDispersion = 0;
	int minParalleldispersion = 0;
	for (int exception = 0; exception < input->getAmountOfParallel(); exception++) {
		for (int p = 0; p < input->getAmountOfParallel(); p++) {
			if ((input->getFourDimArrayExist(session, component, sampleName, p)) && (exception != p)) {
				totalSum += input->getFourDimArrayConcentration(session, component, sampleName, p);
				countOfParallels++;
			}
		}
		totalSum > 0 && countOfParallels > 0 ? arrayAverage[exception] = totalSum / countOfParallels : arrayAverage[exception] = 0;
		totalSum = 0;
		countOfParallels = 0;
	}
	for (int exception = 0; exception < input->getAmountOfParallel(); exception++) {
		for (int p = 0; p < input->getAmountOfParallel(); p++) {
			if ((input->getFourDimArrayExist(session, component, sampleName, p)) && (exception != p)) {
				totalSumSqr += pow(abs(input->getFourDimArrayConcentration(session, component, sampleName, p) - arrayAverage[exception]), 2);
				countOfParallels++;
			}
		}
		if (countOfParallels > 1) {
			arrayDispersion[exception].value = totalSumSqr / (countOfParallels - 1);
			arrayDispersion[exception].isChanged = true;
		}
		else {
			arrayDispersion[exception].value = 0;
			arrayDispersion[exception].isChanged = false;
		} 
		totalSumSqr = 0;
		countOfParallels = 0;
	}
	for (int i = 0; i < input->getAmountOfParallel(); i++) {
		if (arrayDispersion[i].isChanged) {
			minDispersion = arrayDispersion[i].value;
			i = input->getAmountOfParallel();
		}
	}
	for (int i = 0; i < input->getAmountOfParallel(); i++) {
		if ((arrayDispersion[i].value < minDispersion) && (arrayDispersion[i].isChanged)) {
			minDispersion = arrayDispersion[i].value;
			minParalleldispersion = i;
		}
	}
	return minParalleldispersion;
}

FourDimArray * Statistic::repeatabilityCalculate(FourDimArray * dispersion) {
	
	float totalSumOfDispersion = 0;
	int dispersionCounter = 0;

	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, dispersion->getAmountOfComponent(), dispersion->getAmountOfSampleName(), 1);

	for (int c = 0; c < dispersion->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (dispersion->getStrComponent(c)));
	}

	for (int sN = 0; sN < dispersion->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (dispersion->getStrSampleName(sN)));
	}

	for (int c = 0; c < dispersion->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < dispersion->getAmountOfSampleName(); sN++) {
			totalSumOfDispersion = 0;
			dispersionCounter = 0;
			for (int s = 0; s < dispersion->getAmountOfSession(); s++) {
				if ((dispersion->getFourDimArrayExist(s, c, sN, 0)) && (dispersion->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
						totalSumOfDispersion += dispersion->getFourDimArrayConcentration(s, c, sN, 0);
						dispersionCounter++;
				}
			}
			if ((dispersionCounter > 0) && (totalSumOfDispersion >= 0)) {
				sPtr->setFourDimArrayConcentration(0, c, sN, 0, sqrt(totalSumOfDispersion / dispersionCounter));
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
			}
		}
	}
	return sPtr;
}

FourDimArray * Statistic::repeatabilityLimitCalculate(FourDimArray * repeatability, int amountOfParallelsInTestResult) {

	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, repeatability->getAmountOfComponent(), repeatability->getAmountOfSampleName(), 1);

	for (int c = 0; c < repeatability->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (repeatability->getStrComponent(c)));
	}

	for (int sN = 0; sN < repeatability->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (repeatability->getStrSampleName(sN)));
	}

	for (int c = 0; c < repeatability->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < repeatability->getAmountOfSampleName(); sN++) {
			if ((repeatability->getFourDimArrayExist(0, c, sN, 0)) && (repeatability->getFourDimArrayStatus(0, c, sN, 0) == 0) && (amountOfParallelsInTestResult > 0) && (amountOfParallelsInTestResult <= 5)){
				if (amountOfParallelsInTestResult > 1) {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, ((repeatability->getFourDimArrayConcentration(0, c, sN, 0)) * qTable(amountOfParallelsInTestResult)));
				}
				else {
					sPtr->setFourDimArrayStatus(0, c, sN, 0, 3);
				}
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
			}
		}
	}
	return sPtr;
}

bool Statistic::grubbsCriterionCalculate(FourDimArray * input, FourDimArray * average) {
	OneDimArray * arrayAverage = NULL;
	arrayAverage = new OneDimArray[input->getAmountOfSession()];
	float minAverage = 0;
	float maxAverage = 0;
	float totalSumAverages = 0;
	int counterAverages = 0;
	int minPosition = 0;
	int maxPosition = 0;
	float standardDeviation = 0;
	float averageOfAverages = 0;
	float sumOfSqDifferences = 0;
	bool firstRealValue = false;
	bool wasChanged = false;
	
	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
			if ((input->getAmountOfSession(c, sN) > 2) && (input->getAmountOfSession(c, sN) < 41)) {
				for (int i = 0; i < input->getAmountOfSession(); i++) {
					arrayAverage[i].value = 0;
					arrayAverage[i].isChanged = false;
				}
				for (int s = 0; s < input->getAmountOfSession(); s++) {
					if ((average->getFourDimArrayExist(s, c, sN, 0)) && (average->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
						arrayAverage[s].value = average->getFourDimArrayConcentration(s, c, sN, 0);
						arrayAverage[s].isChanged = true;
					}
				}
				firstRealValue = false;
				for (int s = 0; s < input->getAmountOfSession(); s++) {
					if ((arrayAverage[s].isChanged) && (!firstRealValue)) {
						minAverage = arrayAverage[s].value;
						firstRealValue = true;
					}
				}
				maxAverage = 0;
				totalSumAverages = 0;
				counterAverages = 0;
				standardDeviation = 0;
				averageOfAverages = 0;
				sumOfSqDifferences = 0;
				minPosition = 0;
				maxPosition = 0;
				for (int s = 0; s < input->getAmountOfSession(); s++) {
					if (arrayAverage[s].isChanged) {
						totalSumAverages += arrayAverage[s].value;
						counterAverages++;
						if (arrayAverage[s].value < minAverage) {
							minAverage = arrayAverage[s].value;
							minPosition = s;
						}
						if (arrayAverage[s].value > maxAverage) {
							maxAverage = arrayAverage[s].value;
							maxPosition = s;
						}
					}
				}
				averageOfAverages = totalSumAverages / counterAverages;
				for (int s = 0; s < input->getAmountOfSession(); s++) {
					if (arrayAverage[s].isChanged) {
						sumOfSqDifferences += pow(abs(arrayAverage[s].value - averageOfAverages), 2);
					}
				}
				standardDeviation = sqrt(sumOfSqDifferences / (counterAverages - 1));
				if (((maxAverage - averageOfAverages) / standardDeviation) > criticalValuesGrubbsCriterion(counterAverages)) {
					for (int p = 0; p < input->getAmountOfParallel(); p++) {
						if ((input->getFourDimArrayExist(maxPosition, c, sN, p)) && (input->getFourDimArrayStatus(maxPosition, c, sN, p) == 0)) {
							input->setFourDimArrayStatus(maxPosition, c, sN, p, 42);
						}
					}
				}
				if (((averageOfAverages - minAverage) / standardDeviation) > criticalValuesGrubbsCriterion(counterAverages)) {
					for (int p = 0; p < input->getAmountOfParallel(); p++) {
						if ((input->getFourDimArrayExist(minPosition, c, sN, p)) && (input->getFourDimArrayStatus(minPosition, c, sN, p) == 0)) {
							input->setFourDimArrayStatus(minPosition, c, sN, p, 41);
							wasChanged = true;
						}
					}
				}
			}
			else {
				for (int s = 0; s < input->getAmountOfSession(); s++) {
					for (int p = 0; p < input->getAmountOfParallel(); p++) {
						if ((input->getFourDimArrayExist(s, c, sN, p)) && (input->getFourDimArrayStatus(s, c, sN, p) == 0)) {
							input->setFourDimArrayStatus(s, c, sN, p, 43);
							wasChanged = true;
						}
					}
				}
			}
		}
	}
	return wasChanged;
}

float Statistic::criticalValuesGrubbsCriterion(int amountOfAverages) {
	float criticalValuesGrubbsCriterion[38] = {1.155, 1.481, 1.715, 1.887, 2.020, 2.126, 2.215, 2.29, 2.355, 2.412, 2.462, 2.507, 2.549, 2.585, 2.62, 2.651, 2.681, 2.709, 2.733, 2.758, 2.781, 2.802, 2.822, 2.841, 2.859, 2.876, 2.893, 2.908, 2.924, 2.938, 2.952, 2.965, 2.979, 2.991, 3.003, 3.014, 3.025, 3.036};
	return criticalValuesGrubbsCriterion[amountOfAverages - 3];
}

float Statistic::qTable(int amountOfParallelsInTestResult) {
	float qTable[4] = { 2.77, 3.31, 3.63, 3.86 };
	return qTable[amountOfParallelsInTestResult - 2];
}

FourDimArray * Statistic::reproducibilityCalculate(FourDimArray * input, FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * repeatability, int amountOfParallelsInTestResult){
	float sumOfSqDifferences = 0;
	int sqDifferencesCounter = 0;
	float dispersion = 0;
	float standartDeviation = 0;
	int minAmountOfParallel = 0;

	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, average->getAmountOfComponent(), average->getAmountOfSampleName(), 1);

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (average->getStrComponent(c)));
	}

	for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (average->getStrSampleName(sN)));
	}

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
			if (average->getAmountOfSession(c, sN) > 2) {
				sumOfSqDifferences = 0;
				sqDifferencesCounter = 0;
				dispersion = 0;
				standartDeviation = 0;
				minAmountOfParallel = 0;
				for (int s = 0; s < average->getAmountOfSession(); s++) {
					if ((average->getFourDimArrayExist(s, c, sN, 0)) && (average->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
						sumOfSqDifferences += pow(abs(average->getFourDimArrayConcentration(s, c, sN, 0) - averageOfAverages->getFourDimArrayConcentration(0, c, sN, 0)), 2);
						sqDifferencesCounter++;
					}
				}
				if (sqDifferencesCounter > 1) {
					dispersion = sumOfSqDifferences / (sqDifferencesCounter - 1);
				}
				for (int s = 0; s < input->getAmountOfSession(); s++) {
					if ((input->getAmountOfParallel(s, c, sN) > 0)) {
						minAmountOfParallel = input->getAmountOfParallel(s, c, sN);
						s = input->getAmountOfSession();
					}
				}
				for (int s = 0; s < input->getAmountOfSession(); s++) {
					if ((input->getAmountOfParallel(s, c, sN) < minAmountOfParallel) && (input->getAmountOfParallel(s, c, sN) != 0)) {
						minAmountOfParallel = input->getAmountOfParallel(s, c, sN);
					}
				}
			
				standartDeviation = sqrt((dispersion + (((static_cast<float>(1) / amountOfParallelsInTestResult) - (static_cast<float>(1) / minAmountOfParallel)) * pow(repeatability->getFourDimArrayConcentration(0, c, sN, 0), 2))));
				if (amountOfParallelsInTestResult == 1) {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, sqrt(pow(standartDeviation, 2) + (pow(repeatability->getFourDimArrayConcentration(0, c, sN, 0), 2) / 2)));
					sPtr->setFourDimArrayExist(0, c, sN, 0, true);
				}
				if ((amountOfParallelsInTestResult > 1) && (amountOfParallelsInTestResult <= 5)) {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, standartDeviation);
					sPtr->setFourDimArrayExist(0, c, sN, 0, true);
				}
				if ((sPtr->getFourDimArrayConcentration(0, c, sN, 0) < repeatability->getFourDimArrayConcentration(0, c, sN, 0)) && (amountOfParallelsInTestResult > 0) && (amountOfParallelsInTestResult <= 5)) {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, repeatability->getFourDimArrayConcentration(0, c, sN, 0));
					sPtr->setMessageWithNullStatus(0, c, sN, 0, "Repeat. ");
				}
			}
		}
	}
	return sPtr;
}

FourDimArray * Statistic::reproducibilityLimitCalculate(FourDimArray * reproducibility, int amountOfParallelsInTestResult) {
	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, reproducibility->getAmountOfComponent(), reproducibility->getAmountOfSampleName(), 1);

	for (int c = 0; c < reproducibility->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (reproducibility->getStrComponent(c)));
	}

	for (int sN = 0; sN < reproducibility->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (reproducibility->getStrSampleName(sN)));
	}

	for (int c = 0; c < reproducibility->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < reproducibility->getAmountOfSampleName(); sN++) {
			if ((reproducibility->getFourDimArrayExist(0, c, sN, 0)) && (reproducibility->getFourDimArrayStatus(0, c, sN, 0) == 0) && (amountOfParallelsInTestResult > 0) && (amountOfParallelsInTestResult <= 5)) {
				if (amountOfParallelsInTestResult > 1) {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, ((reproducibility->getFourDimArrayConcentration(0, c, sN, 0)) * qTable(amountOfParallelsInTestResult)));
				}
				else {
					sPtr->setFourDimArrayStatus(0, c, sN, 0, 3);
				}
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
			}
		}
	}
	return sPtr;
}

FourDimArray * Statistic::studentTTestCalculate(FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * bias, FourDimArray * uncertainties) {
	int averageCounter = 0;
	float sumOfSqDifferences = 0;
	float averageOfDispersions = 0;
	
	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, average->getAmountOfComponent(), average->getAmountOfSampleName(), 1);

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (average->getStrComponent(c)));
	}

	for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (average->getStrSampleName(sN)));
	}

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
			averageCounter = 0;
			sumOfSqDifferences = 0;
			averageOfDispersions = 0;
			if ((averageOfAverages->getFourDimArrayExist(0, c, sN, 0)) && (averageOfAverages->getFourDimArrayStatus(0, c, sN, 0) == 0) && (uncertainties->getFourDimArrayExist(0, c, sN, 0))) {
				for (int s = 0; s < average->getAmountOfSession(); s++) {
					if ((average->getFourDimArrayExist(s, c, sN, 0)) && (average->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
						sumOfSqDifferences += pow(abs(average->getFourDimArrayConcentration(s, c, sN, 0) - averageOfAverages->getFourDimArrayConcentration(0, c, sN, 0)), 2);
						averageCounter++;
					}
				}
				averageOfDispersions = sumOfSqDifferences / (averageCounter - 1);
				sPtr->setFourDimArrayConcentration(0, c, sN, 0, (abs(bias->getFourDimArrayConcentration(0, c, sN, 0)) / sqrt((averageOfDispersions / averageCounter) + (pow(uncertainties->getFourDimArrayConcentration(0, c, sN, 0), 2) / 3))));
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
				if (sPtr->getFourDimArrayConcentration(0, c, sN, 0) > percentagePointsOfStudentDistribution(averageCounter)) {
					sPtr->setMessageWithNullStatus(0, c, sN, 0, "Sign. ");
				}
			}
		}
	}
	return sPtr;
}

float Statistic::percentagePointsOfStudentDistribution(int amountOfExistAverages) {
	float pointsOfStudentDistribution[30] = {12.71, 4.3, 3.18, 2.78, 2.57, 2.45, 2.37, 2.31, 2.26, 2.23, 2.2, 2.18, 2.16, 2.15, 2.14, 2.12, 2.11, 2.10, 2.09, 2.09, 2.08, 2.07, 2.07, 2.06, 2.06, 2.06, 2.05, 2.05, 2.04, 2.04};
	return pointsOfStudentDistribution[amountOfExistAverages - 2];
}

FourDimArray * Statistic::truenessCalculate(FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * bias, FourDimArray * uncertainties, FourDimArray * student) {
	int averageCounter = 0;
	float sumOfSqDifferences = 0;
	float averageOfDispersions = 0;
	float trueness = 0;

	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, average->getAmountOfComponent(), average->getAmountOfSampleName(), 1);

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (average->getStrComponent(c)));
	}

	for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (average->getStrSampleName(sN)));
	}

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
			averageCounter = 0;
			sumOfSqDifferences = 0;
			averageOfDispersions = 0;
			trueness = 0;
			if ((student->getFourDimArrayExist(0, c, sN, 0)) && (student->getFourDimArrayStatus(0, c, sN, 0) == 0) && (bias->getFourDimArrayExist(0, c, sN, 0)) && (bias->getFourDimArrayStatus(0, c, sN, 0) == 0) && (uncertainties->getFourDimArrayExist(0, c, sN, 0))) {
				for (int s = 0; s < average->getAmountOfSession(); s++) {
					if ((average->getFourDimArrayExist(s, c, sN, 0)) && (average->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
						sumOfSqDifferences += pow(abs(average->getFourDimArrayConcentration(s, c, sN, 0) - averageOfAverages->getFourDimArrayConcentration(0, c, sN, 0)), 2);
						averageCounter++;
					}
				}
				averageOfDispersions = sumOfSqDifferences / (averageCounter - 1);
				trueness = 2 * sqrt((averageOfDispersions / averageCounter) + (pow(uncertainties->getFourDimArrayConcentration(0, c, sN, 0), 2) / 3));
				if (student->getFourDimArrayConcentration(0, c, sN, 0) > percentagePointsOfStudentDistribution(averageCounter)) {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, abs(bias->getFourDimArrayConcentration(0, c, sN, 0) - trueness) > abs(bias->getFourDimArrayConcentration(0, c, sN, 0) + trueness) ? abs(bias->getFourDimArrayConcentration(0, c, sN, 0) - trueness) : abs(bias->getFourDimArrayConcentration(0, c, sN, 0) + trueness));
				}
				else {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, trueness);
				}
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
			}
		}
	}
	return sPtr;
}

FourDimArray * Statistic::accuracyCalculate(FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * bias, FourDimArray * uncertainties, FourDimArray * reproducibility, FourDimArray * student) {
	float totalSumOfAverages = 0;
	int averageCounter = 0;
	float sumOfSqDifferences = 0;
	float averageOfDispersions = 0;
	float truenessStDev = 0;
	float accuracy = 0;

	FourDimArray *sPtr = NULL;
	sPtr = new FourDimArray(1, average->getAmountOfComponent(), average->getAmountOfSampleName(), 1);

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		sPtr->setStrComponent(c, (average->getStrComponent(c)));
	}

	for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
		sPtr->setStrSampleName(sN, (average->getStrSampleName(sN)));
	}

	for (int c = 0; c < average->getAmountOfComponent(); c++) {
		for (int sN = 0; sN < average->getAmountOfSampleName(); sN++) {
			totalSumOfAverages = 0;
			averageCounter = 0;
			sumOfSqDifferences = 0;
			averageOfDispersions = 0;
			truenessStDev = 0;
			accuracy = 0;
			if ((student->getFourDimArrayExist(0, c, sN, 0)) && (student->getFourDimArrayStatus(0, c, sN, 0) == 0) && (bias->getFourDimArrayExist(0, c, sN, 0)) && (bias->getFourDimArrayStatus(0, c, sN, 0) == 0) && (uncertainties->getFourDimArrayExist(0, c, sN, 0))) {
				for (int s = 0; s < average->getAmountOfSession(); s++) {
					if ((average->getFourDimArrayExist(s, c, sN, 0)) && (average->getFourDimArrayStatus(s, c, sN, 0) == 0)) {
						sumOfSqDifferences += pow(abs(average->getFourDimArrayConcentration(s, c, sN, 0) - averageOfAverages->getFourDimArrayConcentration(0, c, sN, 0)), 2);
						averageCounter++;
					}
				}
				averageOfDispersions = sumOfSqDifferences / (averageCounter - 1);
				truenessStDev = sqrt((averageOfDispersions / averageCounter) + (pow(uncertainties->getFourDimArrayConcentration(0, c, sN, 0), 2) / 3));
				accuracy = (2 * sqrt(pow(reproducibility->getFourDimArrayConcentration(0, c, sN, 0), 2) + pow(truenessStDev, 2)));
				if (student->getFourDimArrayConcentration(0, c, sN, 0) > percentagePointsOfStudentDistribution(averageCounter)) {
					sPtr->setFourDimArrayConcentration(0, c, sN, 0, abs(bias->getFourDimArrayConcentration(0, c, sN, 0) - accuracy) > abs(bias->getFourDimArrayConcentration(0, c, sN, 0) + accuracy) ? abs(bias->getFourDimArrayConcentration(0, c, sN, 0) - accuracy) : abs(bias->getFourDimArrayConcentration(0, c, sN, 0) + accuracy));
				}
				else {
					if ((truenessStDev / reproducibility->getFourDimArrayConcentration(0, c, sN, 0)) <= (1 / 3)) {
						sPtr->setFourDimArrayConcentration(0, c, sN, 0, (2 * reproducibility->getFourDimArrayConcentration(0, c, sN, 0)));
					}
					else {
						sPtr->setFourDimArrayConcentration(0, c, sN, 0, accuracy);
					}
				}
				sPtr->setFourDimArrayExist(0, c, sN, 0, true);
			}
		}
	}
	return sPtr;
}

Statistic::~Statistic() {
}
