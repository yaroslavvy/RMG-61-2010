#ifndef STATISTIC_H
#define STATISTIC_H

#include "FourDimArray.h"

class Statistic
{
public:
	Statistic();
	static FourDimArray * averageCalculate(FourDimArray * input);
	~Statistic();
protected:

};

#endif // !STATISTIC_H