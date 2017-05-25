#include "column.h"

Column::Column(string n)
{
	this->name = n;
	this->draggingOffset = 0.0f;
	this->inverted = false;
    this->filtered=false;
}

void Column::add_value(float val)
{
	DataPoint point;
	point.val = val;
	point.show = true;
	point.invert = false;
	this->floatValues.push_back(point);
}

void Column::add_value(string val)
{
	this->stringValues.push_back(val);
}

string Column::get_name()
{
	return this->name;
}

vector<DataPoint> Column::get_numeric_values()
{
	return this->floatValues;
}

vector<string> Column::get_string_values()
{
	return this->stringValues;
}

void Column::set_type(DataType type)
{
	this->type = type;
}

DataType Column::get_type()
{
	return this->type;
}


Column& Column::operator=(const Column &other)
{
	this->name = other.name;
	this->floatValues = other.floatValues;
	this->stringValues = other.stringValues;
	this->type = other.type;
	return *this;
}

Column::Column(){}

DataPoint::DataPoint(){}

void Column::set_numeric_values(vector<DataPoint> values)
{
	this->floatValues = values;
}

void Column::set_name(string name)
{
	this->name = name;
}