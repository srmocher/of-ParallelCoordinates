#pragma once
#include "column.h"
class ZScore
{
private:
	Column selectedColumn;
	float mean;
	float standardDeviation;
	void calculateMeanAndSD();
public:
	ZScore(Column);
	float getZScore(int);
	float getMean();
};