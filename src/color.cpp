#include "color.h"

CatColor::CatColor(string cat, ofColor color,int count)
{
	this->category = cat;
	this->color = color;
	this->count = count;
}

string CatColor::getCategory()
{
	return this->category;
}

ofColor CatColor::getColor()
{
	return this->color;
}

int CatColor::getCount()
{
	return this->count;
}