#pragma once
#include <vector>
#include <string>
#include "column.h"
using namespace std;


class TableReader
{
private:
	vector<Column> columns;	
	int numColumns;
public:
	TableReader();
	~TableReader();
	void read_data(std::string& filename);
	Column get_column(int which_column);
	int get_num_columns();
	vector<Column> get_columns();
};



