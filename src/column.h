#pragma once

#include <string>
#include <vector>
#include "ofMain.h"
#define MISSING_DATA "MISSING_DATA"
using namespace std;

class DataPoint
{
public:
	float val;
	bool show;
	bool invert;
	ofColor color;
	DataPoint();
};
enum DataType{CATEGORICAL,QUANTITATIVE};
class Column
{
private:
	std::string name;
	std::vector<DataPoint> floatValues;
	std::vector<string> stringValues;
	DataType type;
    
public:
	float draggingOffset;
	Column(string n);	
	Column();
	std::string get_name();
	vector<DataPoint> get_numeric_values();
	vector<string> get_string_values();
	void add_value(float val);
	void add_value(string val);
	void set_type(DataType type);
	DataType get_type();
	Column& operator=(const Column &other);
	void set_numeric_values(vector<DataPoint> values);
	void set_name(string);
	bool inverted;
    bool filtered;
};


