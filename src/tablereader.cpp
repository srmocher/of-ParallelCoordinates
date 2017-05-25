#include "tablereader.h"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	std::stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;

}

bool is_number(const std::string& s)
{
	if (s.size() == 0)
		return false;
	for (int i = 0;i < s.size();i++)
	{
		if (s[i]!='.' && !isdigit(s[i]))
			return false;
	}
	return true;
}


TableReader::TableReader()
{}

TableReader::~TableReader() 
{
	
}
void TableReader::read_data(string& fileName)
{
	this->columns.clear();
	ifstream file(fileName);
	string str;
	
	//read column names
	getline(file, str);
   // cout<<"First line is "<<str<<endl;
	vector<string> column_names = split(str, '\t');
	for (int i = 0;i < column_names.size();i++)
	{		
		Column column(column_names[i]);
		this->columns.push_back(column);
	}

	//read values
	
	while (getline(file, str))
	{
		vector<string> values = split(str, '\t');
		for (int i = 0;i < values.size();i++)
		{
			if (is_number(values[i])) 
			{
				this->columns.at(i).add_value(stof(values[i]));
				
				this->columns.at(i).set_type(QUANTITATIVE);
			}
			else if(values[i].empty()) //missing data
			{
				if (this->columns.at(i).get_type()==QUANTITATIVE)
					this->columns.at(i).add_value(INFINITY);
				else if (this->columns.at(i).get_type() == CATEGORICAL)
					this->columns.at(i).add_value(MISSING_DATA);
			}
			else
			{
				this->columns.at(i).add_value(values[i]);
				this->columns.at(i).set_type(CATEGORICAL);
			}
		}
		
	}
	
}


Column TableReader::get_column(int columnIndex)
{
	Column column = this->columns.at(columnIndex);
	return column;
}


int TableReader::get_num_columns()
{
	int size =  columns.size();
	return size;
}

vector<Column> TableReader::get_columns()
{
	return this->columns;
}