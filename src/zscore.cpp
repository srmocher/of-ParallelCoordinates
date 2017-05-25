#include "zscore.h"
#include <numeric>

ZScore::ZScore(Column c)
{
	this->selectedColumn = c;
}

void ZScore::calculateMeanAndSD()
{
	vector<DataPoint> values = selectedColumn.get_numeric_values();
	double sum = 0;
	for (int i = 0;i < values.size();i++)
		sum += values[i].val;
	this->mean = sum / values.size();

	double sq_sum = 0;
	for (auto i = 0;i < values.size();i++)
		sq_sum += values[i].val*values[i].val;
	this->standardDeviation = std::sqrt(sq_sum / values.size() - mean * mean);
}

float ZScore::getZScore(int rowIndex)
{
	vector<DataPoint> values = this->selectedColumn.get_numeric_values();
	float value = values[rowIndex].val;
	return (value - this->mean) / this->standardDeviation;
}

float ZScore::getMean()
{
	return this->mean;
}