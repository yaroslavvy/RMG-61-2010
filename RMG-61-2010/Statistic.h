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
	static int selectCochranOrBartlettCriterion(FourDimArray * input, FourDimArray * dispersion, int component, int sampleName);
	static bool cochranCriterionCalculate(FourDimArray * input, FourDimArray * dispersion, int component, int sampleName);
	static bool bartlettCriterionCalculate(FourDimArray * input, FourDimArray * dispersion, int component, int sampleName);
	static bool arrayElementsAreEqual(int *array, int length);
	static float cochranCriticalValues(int amountOfSessions, int amountOfParallels);
	static float chiDistributionValues(int amountOfDipersion);
	static FourDimArray * possibleOutlierReport(FourDimArray * input);
	static int findOutlier(FourDimArray * input, int session, int component, int sampleName);
	static FourDimArray * repeatabilityCalculate(FourDimArray * dispersion);
	static FourDimArray * repeatabilityLimitCalculate(FourDimArray * repeatability, int amountOfParallelsInTestResult);
	static bool grubbsCriterionCalculate(FourDimArray * input, FourDimArray * average);
	static float criticalValuesGrubbsCriterion(int amountOfAverages);

	~Statistic();
protected:

};

#endif // !STATISTIC_H