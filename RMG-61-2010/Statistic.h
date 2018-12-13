#ifndef STATISTIC_H
#define STATISTIC_H

#include "FourDimArray.h"
#include "StructOneDimArray.h"

class Statistic
{
public:
	Statistic();
	static FourDimArray * averageCalculate(FourDimArray * input);
	static FourDimArray * biasCalculate(FourDimArray * average, FourDimArray * concentrations);
	static FourDimArray * dispersionCalculate(FourDimArray * input, FourDimArray * average);
	static bool cochranAndBartlettCriterionCalculate(FourDimArray * input, FourDimArray * dispersion);
	static int selectCochranOrBartlettCriterion(FourDimArray * input, FourDimArray * dispersion, const int component, const int sampleName);
	static bool cochranCriterionCalculate(FourDimArray * input, FourDimArray * dispersion, const int component, const int sampleName);
	static bool bartlettCriterionCalculate(FourDimArray * input, FourDimArray * dispersion, const int component, const int sampleName);
	static bool arrayElementsAreEqual(int *array, const int length);
	static float cochranCriticalValues(const int amountOfSessions, const int amountOfParallels);
	static float chiDistributionValues(const int amountOfDipersion);
	static FourDimArray * possibleOutlierReport(FourDimArray * input);
	static int findOutlier(FourDimArray * input, const int session, const int component, const int sampleName);
	static FourDimArray * repeatabilityCalculate(FourDimArray * dispersion);
	static FourDimArray * repeatabilityLimitCalculate(FourDimArray * repeatability, const int amountOfParallelsInTestResult);
	static bool grubbsCriterionCalculate(FourDimArray * input, FourDimArray * average);
	static float criticalValuesGrubbsCriterion(const int amountOfAverages);
	static float qTable(const int amountOfParallelsInTestResult);
	static FourDimArray * averageOfAveragesCalculate(FourDimArray * average);
	static FourDimArray * reproducibilityCalculate(FourDimArray * input, FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * repeatability, const int amountOfParallelsInTestResult);
	static FourDimArray * reproducibilityLimitCalculate(FourDimArray * reproducibility, const int amountOfParallelsInTestResult);
	static FourDimArray * studentTTestCalculate(FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * bias, FourDimArray * uncertainties);
	static float percentagePointsOfStudentDistribution(const int amountOfExistAverages);
	static FourDimArray * truenessCalculate(FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * bias, FourDimArray * uncertainties, FourDimArray * student);
	static FourDimArray * accuracyCalculate(FourDimArray * average, FourDimArray * averageOfAverages, FourDimArray * bias, FourDimArray * uncertainties, FourDimArray * reproducibility, FourDimArray * student);

	~Statistic();
protected:

};

#endif // !STATISTIC_H