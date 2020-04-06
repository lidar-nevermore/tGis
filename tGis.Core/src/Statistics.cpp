#include "Statistics.h"
#include "cmath"

BEGIN_NAME_SPACE(tGis, Core)

Statistics::Statistics(bool calcMean, bool calcVar)
{
	min = DBL_MAX;
	max = 0 - DBL_MAX;
	mean = 0;
	sum = 0;
	count = 0;
	calcmean = calcMean;
	calcvar = calcVar;
	sumvar = 0.0;
	meanvar = 0.0;
	stddev = 0.0;
}

void Statistics::Recursive(double x)
{
	if (x < min)
	{
		min = x;
	}

	if (x > max)
	{
		max = x;
	}
	sum += x;

	double preMean = mean;
	if (calcmean || calcvar)
	{
		if (count == 0)
		{
			mean = sum;
			preMean = mean;
		}
		else
		{
			mean = sum / count;
		}
	}		

	if (calcvar)
	{
		sumvar = (x - preMean)*(x - mean);
		meanvar = sumvar / count;
		stddev = sqrt(meanvar);
	}

	count += 1;
}

void Statistics::InitRecursiveVariance()
{
	mean = sum / count;
	sumvar = 0.0;
	meanvar = 0.0;
	stddev = 0.0;
}

void Statistics::RecursiveVariance(double x)
{
	sumvar = (x - mean)*(x - mean);
}

void Statistics::FinishRecursiveVariance()
{
	meanvar = sumvar / count;
	stddev = sqrt(meanvar);
}



END_NAME_SPACE(tGis, Core)

