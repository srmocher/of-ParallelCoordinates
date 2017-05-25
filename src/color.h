#pragma once
#include <string>
#include "ofMain.h"

class CatColor
{
private:
	std::string category;
	ofColor color;
	
public:
	CatColor(string, ofColor,int);
	ofColor getColor();
	std::string getCategory();
	int getCount();
	int count;
};