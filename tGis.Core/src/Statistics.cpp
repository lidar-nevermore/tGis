#include "Statistics.h"
#include "cmath"

BEGIN_NAME_SPACE(tGis, Core)

void Statistics::InitRecursive(double x, bool cMean, bool cVar)
{
	min = x;
	max = x;
	mean = x;
	sum = x;
	count = 1;
	calcmean = cMean;
	calcvar = cVar;
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

	if(calcmean)
		mean = sum / count;

	if (calcvar)
	{
		double preMean = mean;
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

