#include "parallelplot.h"
#include "ofMain.h"
#include <iostream>
#include <unordered_set>

void ParallelPlot::setup(int w, int h, vector<Column> columns, int x_pos, int y_pos,int axisLength)
{
	this->width = 0;
	this->height = h;
	this->columns = columns;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->axisLength = axisLength;
	generateColors();
	
	setupColorMapForCategories();
	if (!registeredEvents)
	{
		ofRegisterMouseEvents(this);
	}
	this->selectionStartX = -1;
	this->selectionEndX = -1;
	this->selectionStartY = -1;
	this->selectionEndX = -1;
	this->selectionEndY = -1;
	this->mappedColumns = mapData();
	this->columnDragged = false;
}

void ParallelPlot::draw()
{
	
	drawAxes();
	drawLabels();
	drawPoints();
	
		drawSelection();
	
	drawLegend();
	drawRange();
}

vector<string> split(const std::string &s, char delim) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	vector<string> tokens;
	while (std::getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

float find_max(vector<DataPoint> values)
{
	float max = -INFINITY;
	for (auto it = values.begin();it != values.end();it++)
	{
		if (it->val > max && it->val!=INFINITY)
			max = it->val;
	}
	return max;
}
float find_min(vector<DataPoint> values)
{
	float min = INFINITY;
	for (auto it = values.begin();it != values.end();it++)
	{
		if (it->val < min && it->val!= INFINITY)
			min = it->val;
	}
	return min;
}
void ParallelPlot::drawAxes()
{
	ofPushMatrix();
	ofTranslate(x_pos, y_pos);
	width = 0;
	float xOffset = 0.0f;
	ofSetColor(ofColor(0, 0, 0,255));
	ofSetLineWidth(5);
	for (int i = 0;i < columns.size();i++)
	{
		if (columns[i].get_type() == QUANTITATIVE)
		{
			ofSetLineWidth(1.5f);
			ofDrawLine(xOffset + columns[i].draggingOffset, 0, xOffset+columns[i].draggingOffset, height);
			width = xOffset;
			xOffset = xOffset + (1.0f / (float)columns.size())*ofGetWindowWidth()+2.5;
			
		}
	}
	
	ofDrawLine(-5, height+50, width+5, height+50);
	ofTrueTypeFont font;
	font.loadFont("Serif.ttf", 10);
	font.drawString("MISSING VALUES", width / 2, height + 70);
	ofPopMatrix();
}

void ParallelPlot::drawLabels()
{
	ofPushMatrix();
	ofTranslate(x_pos-2, y_pos-3);
	ofTrueTypeFont font;
	float fontSize = (1.0f / (float)columns.size()) * 120;
	font.load("Serif.ttf",(int)fontSize);
	float xOffset = 0.0f;
	for (int i = 0;i < columns.size();i++)
	{
		if (columns[i].get_type() == QUANTITATIVE)
		{		
			int length = columns[i].get_name().length();
			if(i%2==1)
			font.drawString(columns[i].get_name(), xOffset-length+columns[i].draggingOffset, -15);
			else
				font.drawString(columns[i].get_name(), xOffset - length+columns[i].draggingOffset, -5);
			xOffset = xOffset + (1.0f / (float)columns.size())*ofGetWindowWidth()+2.5;

		}
	}
	ofPopMatrix();
}

vector<Column> ParallelPlot::mapData()
{
	vector<Column> output;
	for (int i = 0;i < columns.size();i++)
	{
		Column c = columns[i];
		if (c.get_type() == QUANTITATIVE)
		{
			vector<DataPoint> values = c.get_numeric_values();
			float min = find_min(values);
			float max = find_max(values);
			
			Column new_column(c.get_name());
			
			for (int j = 0;j < values.size();j++)
			{
				if (values[j].val != INFINITY)
				{
					float new_val = ofMap(values[j].val, min, max, 0, height);
					new_column.add_value(new_val);
				}
				else
					new_column.add_value(values[j].val);
			}
			output.push_back(new_column);
		}
		else
		{
			Column categorical(c.get_name());
			output.push_back(categorical);
		}
	}
	return output;
}

void ParallelPlot::drawRange()
{
	ofPushMatrix();
	ofTranslate(x_pos, y_pos+height);
	//vector<Column> mappedColumns = mapData();
	ofTrueTypeFont font;
	font.load("Serif.ttf", 9);
	int numPoints = 4;
	float xOffset = 0;
	for (int i = 0;i < columns.size();i++)
	{
		float yOffset = 0;
		if (columns[i].get_type() == QUANTITATIVE) 
		{
			if (columns[i].inverted)
				yOffset = -height;
			else
				yOffset = 0;
			vector<DataPoint> values = columns[i].get_numeric_values();
			float min = find_min(values);
			float max = find_max(values);
			float step = (max - min) /(float) numPoints;
			float s = min;
			int j = 0;
			while (j <= numPoints)
			{
				ostringstream oss;
				oss.precision(1);
				oss << fixed;
				oss << s;
				string val = oss.str();
				ofSetColor(0, 0, 0);
				font.drawString(val, xOffset - 35, yOffset+2);
				ofSetLineWidth(2.5f);
				ofDrawLine(xOffset, yOffset, xOffset - 7, yOffset);
				if (!columns[i].inverted)
					yOffset -= (float)height / (float)numPoints;
				else
					yOffset += (float)height / (float)numPoints;
				s += step;
				j++;
			}
			xOffset += (1.0f / (float)columns.size())*ofGetWindowWidth()+2.5;
		}
	}
	ofPopMatrix();
}

void ParallelPlot::drawPoints()
{
	ofPushMatrix();
	ofTranslate(x_pos, y_pos + height);
	//vector<Column> mappedColumns = mapData();
	float xOffset1 = 0,xOffset2 = xOffset1 + (1.0f / (float)columns.size())*ofGetWindowWidth()+2.5;
	int i = 0, j = 0;
	ofSetLineWidth(1.8f);
	vector<string> legends = columns[legendColumnIndex].get_string_values();
	while(i <columns.size()-1)
	{
		if (columns[i].get_type() != QUANTITATIVE) 
		{
			i++;
			continue;
		}
		
		int j = i + 1;
		//cout << columns[i].get_name() << "-" << columns[j].get_name() << endl;
		if (columns[j].get_type() != QUANTITATIVE) {
			i = i + 2;
			j = i + 1;
		}
		vector<DataPoint> p = mappedColumns[i].get_numeric_values();
		vector<DataPoint> q = mappedColumns[j].get_numeric_values();
		
		int numLines = 0;
		for (int k = 0;k < p.size();k++)
		{
			//ofSetColor(0, 0, 0);
			if(numCategorical>1)
				ofSetColor(colorMap[legends[k]]);
			else
			{
				vector<string> tokens = split(legends[k], ' ');
				ofSetColor(colorMap[tokens[0]]);
			}
			//ofSetColor(getColorForRow(k));
			
			if (p[k].val != INFINITY && q[k].val != INFINITY && p[k].show && q[k].show)
			{
				if(p[k].invert && q[k].invert)
					ofDrawLine(xOffset1+columns[i].draggingOffset, p[k].val-height, xOffset2+columns[i].draggingOffset, q[k].val-height);
				else if(p[k].invert)
					ofDrawLine(xOffset1+columns[i].draggingOffset, p[k].val-height, xOffset2+columns[j].draggingOffset, -q[k].val);
				else if(q[k].invert)
					ofDrawLine(xOffset1+columns[i].draggingOffset, -p[k].val, xOffset2+columns[j].draggingOffset, q[k].val-height);
				else
					ofDrawLine(xOffset1+columns[i].draggingOffset, -p[k].val, xOffset2+columns[j].draggingOffset, -q[k].val);
				
			}
			else if(p[k].val==INFINITY && p[k].show && q[k].show)
			{		
				if(q[k].invert)
					ofDrawLine(xOffset1,50, xOffset2+columns[j].draggingOffset, q[k].val - height);
				else
					ofDrawLine(xOffset1, 50, xOffset2+columns[j].draggingOffset, -q[k].val);
			}
			else
			{	
				if (q[k].show && p[k].show)
				{
					if (p[k].invert)
						ofDrawLine(xOffset1+columns[i].draggingOffset, p[k].val-height, xOffset2, 50);
					else
						ofDrawLine(xOffset1+columns[i].draggingOffset, -p[k].val, xOffset2, 50);
				}
			}
			numLines++;
		}
		//cout << numLines << endl;
		xOffset1 += (1.0f / (float)columns.size())*ofGetWindowWidth()+2.5;
		xOffset2 += (1.0f / (float)columns.size())*ofGetWindowWidth()+2.5;
		i++;
	}
	ofPopMatrix();
}

void ParallelPlot::setupColorMapForCategories()
{
	numCategoricalAttributes = 0;
		int colIndex;
	colors.clear();
	for (int i = 0;i < columns.size();i++)
	{
		if (columns[i].get_type() == CATEGORICAL) 
		{
			numCategoricalAttributes++;
			colIndex = i;
			legendColumnIndex = i;
		}
	}
	map<string,int> categories;
	if (numCategoricalAttributes == 1)
	{
		vector<string> values = columns[colIndex].get_string_values();
		
		for (int i = 0;i < values.size();i++)
		{
			
				vector<string> tokens = split(values[i], ' ');
				if (categories.count(tokens[0]) == 0)
				{
					categories[tokens[0]] = 1;
				}
				else
				{
					int count = categories[tokens[0]];
					count++;
					categories[tokens[0]] = count;
				}
			
		}
	}
	else
	{
		int minCountColIndex,minCount=30;
		for (int i = 0;i < columns.size();i++)
		{
			if (columns[i].get_type() == CATEGORICAL)
			{
				map<string,int> cats;
				vector<string> values = columns[i].get_string_values();
				for (int j = 0;j < values.size();j++)
				{
					
						if (cats.count(values[j]) == 0)
						{
							cats[values[j]] = 1;
						}
						else
						{
							int count = cats[values[j]];
							count++;
							cats[values[j]] = count;
						}
					
				}
				if (cats.size() < minCount) {
					minCount = cats.size();
					minCountColIndex = i;
					legendColumnIndex = i;
					categories = cats;
				}
			}
		}
	}
	numCategorical = numCategoricalAttributes;
	ofColor red = ofColor::red;
	ofColor blue = ofColor::blue;
	colorMap.clear();
	int i = 0;
	for (auto it = categories.begin();it != categories.end();it++)
	{
		string cat = it->first;
		ofColor color = presetColors[i];
		color.a = 100;
		CatColor catColor(cat, color,it->second);
	
		colors.push_back(catColor);
		colorMap[it->first] = color;
		i++;
	}
}

ofColor ParallelPlot::getColorForRow(int rowIndex)
{
	
	vector<string> stringValues = columns[legendColumnIndex].get_string_values();
	string category = stringValues[rowIndex];
	for (int i = 0;i < colors.size();i++)
	{
		if (colors[i].getCategory() == category || category.find(colors[i].getCategory())!=std::string::npos)
			return colors[i].getColor();
	}
	return NULL;
}

void ParallelPlot::mouseMoved(ofMouseEventArgs & args)
{
	
}
void ParallelPlot::mouseDragged(ofMouseEventArgs & args)
{
	int x = ofGetMouseX();
	int y = ofGetMouseY();
	float xOffset = x_pos;
	if (canSelectionStart)
	{
		dragging = true;
		for (int i = 0;i < columns.size();i++)
		{
			if (columns[i].get_type() == QUANTITATIVE)
			{
				if (i == selectableColumnIndex)
					break;
				xOffset = xOffset + ((1.0f) / (float)columns.size())*ofGetWindowWidth() + 2.5;
			}
		}
		Column col = mappedColumns[selectableColumnIndex];
		bool filterExists = checkFilterExists(col);
		if (!filterExists)
		{
			Filter f;
			f.column = col;
			f.selectionStartX = selectionStartX;
			f.selectionStartY = selectionStartY;
			selectionEndX = x;
			selectionEndY = y;
			f.selectionEndX = selectionEndX;
			f.selectionEndY = selectionEndY;
			filters.push_back(f);
		}
		else
		{
			int filterIndex;
			for (int i = 0;i<filters.size();i++)
				if (filters[i].column.get_name() == col.get_name()) 
				{
					filterIndex = i;
					break;
				}
			selectionEndX = x;
			selectionEndY = y;
			if (abs(selectionEndX - filters[filterIndex].selectionStartX) <= 50) {
				filters[filterIndex].selectionEndX = selectionEndX;
				filters[filterIndex].selectionEndY = selectionEndY;
			}
		}
		showSelectedPoints();		
	}
	if (y < y_pos-5)
	{
		if (!columnDragged)
		{
			columnDragged = true;
			int xOffset = x_pos;
			for (int i = 0;i < columns.size();i++)
			{
				if (columns[i].get_type() == QUANTITATIVE)
				{
					if (abs(currentColInitialPosition-xOffset)<=20)
					{
						columnDraggedIndex = i;
						break;
					}
					xOffset = xOffset + ((1.0f) / (float)columns.size())*ofGetWindowWidth() + 2.5;
				}
			}
			columns[columnDraggedIndex].draggingOffset = x - currentColInitialPosition;

		}
		else
		{
			columns[columnDraggedIndex].draggingOffset = x - currentColInitialPosition;
		}

	}
}
void ParallelPlot::mousePressed(ofMouseEventArgs & args)
{
	bool rightPressed = ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT);
	if (rightPressed && canDrawSelection)
	{
		canSelectionStart = false;
		canDrawSelection = false;
		resetPoints();
		return;
	}
	leftPressed = ofGetMousePressed(OF_MOUSE_BUTTON_LEFT);
	int x = ofGetMouseX();
	int y = ofGetMouseY();
	float xOffset = x_pos;
	float yOffset = y_pos;
	int i;
	for (i = 0;i < columns.size();i++)
	{
		if (columns[i].get_type() == QUANTITATIVE) 
		{
			if ((abs(x - xOffset)<=30) && abs(x - xOffset) > 1.5 && (y >= y_pos-2 && y <= height + 5))
			{
				canSelectionStart = true;
				selectionStartX = x;
				selectionStartY = y;
				selectableColumnIndex = i;
				break;
			}
			xOffset = xOffset + (1.0f / (float)columns.size())*ofGetWindowWidth() + 2.5;
		}
	}
	
	currentColInitialPosition = x;
	if(i==columns.size())
		canSelectionStart = false;
}
void ParallelPlot::mouseReleased(ofMouseEventArgs & args)
{
	int x = ofGetMouseX();
	int y = ofGetMouseY();
	float xOffset = x_pos;
	if (canSelectionStart && dragging && y >=y_pos)
	{
		for (int i = 0;i < columns.size();i++)
		{
			if (i == selectableColumnIndex)
				break;
			xOffset = xOffset + ((1.0f) / (float)columns.size())*ofGetWindowWidth() + 2.5;
		}
		
			Column col = mappedColumns[selectableColumnIndex];
            columns[selectableColumnIndex].filtered = true;
			bool filterExists = checkFilterExists(col);
			if (!filterExists) 
			{
				Filter f;
				f.column = col;
				f.selectionStartX = selectionStartX;
				f.selectionStartY = selectionStartY;
				selectionEndX = x;
				selectionEndY = y;
				f.selectionEndX = selectionEndX;
				f.selectionEndY = selectionEndY;
				filters.push_back(f);
			}
			else
			{
				int filterIndex;
				for(int i=0;i<filters.size();i++)
					if (filters[i].column.get_name() == col.get_name()) {
						filterIndex = i;
						break;
					}
				selectionEndX = x;
				selectionEndY = y;
				if (abs(selectionEndX - filters[filterIndex].selectionStartX) <= 50)
				{
					filters[filterIndex].selectionEndX = selectionEndX;
					filters[filterIndex].selectionEndY = selectionEndY;
				}
			}
			
			canDrawSelection =true;
			showSelectedPoints();		
	}
	else if (leftPressed && !columnDragged)
	{		
			float x = ofGetMouseX();
			float y = ofGetMouseY();
			float xOffset = x_pos, yOffset = y_pos - 5;
			int invertColumnIndex = -1;
			for (int i = 0;i < columns.size();i++)
			{
				if (columns[i].get_type() == QUANTITATIVE)
				{
					if (abs(x - xOffset) <= 15 && abs(y - yOffset) <= 20)
					{
						invertColumnIndex = i;
						break;
					}
					xOffset = xOffset + (1.0f / (float)columns.size())*ofGetWindowWidth() + 2.5;
				}
			}
			if (invertColumnIndex >= 0)
				invertAxis(invertColumnIndex);
			leftPressed = false;	
	}
	if (columnDragged && y < y_pos - 5)
	{
		columnDragged = false;
		float finalColPosition = x, minOffset = INFINITY, xOffset = x_pos;;
		int nearestCol = -1;
		for (int j = 0;j < columns.size();j++)
		{
			if (columns[j].get_type() == QUANTITATIVE)
			{
				if (j != columnDraggedIndex && minOffset > abs(xOffset - finalColPosition))
				{
					minOffset = abs(xOffset-finalColPosition);
					nearestCol = j;
				}
				xOffset = xOffset + (1.0f / (float)columns.size())*ofGetWindowWidth() + 2.5;
			}
		}
		if (minOffset <= 100)
		{
			columns[columnDraggedIndex].draggingOffset = 0;
			columnDragged = false;
			reorderAxes(columnDraggedIndex, nearestCol);
			
		}

	}
	dragging = false;
}

void ParallelPlot::mouseScrolled(ofMouseEventArgs & args)
{

}
void ParallelPlot::mouseEntered(ofMouseEventArgs & args)
{

}
void ParallelPlot::mouseExited(ofMouseEventArgs & args)
{

}

void ParallelPlot::drawSelection()
{
	for (int i = 0;i < filters.size();i++) 
	{
		ofSetColor(ofColor(128, 128, 128, 150));
		if (filters[i].selectionStartX < filters[i].selectionEndX && filters[i].selectionStartY < filters[i].selectionEndY)
		{
			ofDrawRectangle(filters[i].selectionStartX, filters[i].selectionStartY, filters[i].selectionEndX - filters[i].selectionStartX, filters[i].selectionEndY - filters[i].selectionStartY);
		}
		else if (filters[i].selectionStartX > filters[i].selectionEndX && filters[i].selectionStartY < filters[i].selectionEndY)
		{
			ofDrawRectangle(filters[i].selectionStartX, filters[i].selectionEndY, filters[i].selectionStartX - filters[i].selectionEndX, filters[i].selectionEndY - filters[i].selectionStartY);
		}
		else if (filters[i].selectionStartX < filters[i].selectionEndX && filters[i].selectionStartY > filters[i].selectionEndY)
		{
			ofDrawRectangle(filters[i].selectionStartX, filters[i].selectionStartY - filters[i].selectionEndY, filters[i].selectionEndX - filters[i].selectionStartX, filters[i].selectionStartY - filters[i].selectionEndY);
		}
		else if (filters[i].selectionStartX > filters[i].selectionEndX && filters[i].selectionStartY > filters[i].selectionEndY)
		{
			ofDrawRectangle(filters[i].selectionEndX, filters[i].selectionEndY, filters[i].selectionEndX - filters[i].selectionStartX, filters[i].selectionEndY - filters[i].selectionStartY);
		}
	}
}

void ParallelPlot::showSelectedPoints()
{
    
    
	for (int i = 0;i < filters.size();i++) {
		if (abs(filters[i].selectionStartX - filters[i].selectionEndX) >= 10) {
			
			float xOffset = x_pos;
			int selectedColIndex;
			Column current = filters[i].column;
			for(int j=0;j<columns.size();j++)
				if (columns[j].get_name() == current.get_name()) {
					selectedColIndex = j;
					break;
				}

			//cout << "Selected column is " << columns[selectableColumnIndex].get_name() << endl;

			vector<DataPoint> values = filters[i].column.get_numeric_values();
			vector<string> cats = columns[legendColumnIndex].get_string_values();
			for (int i = 0;i < colors.size();i++)
				colors[i].count = 0;
            bool *indices = new bool[values.size()];
            for(int i=0;i<values.size();i++)
            {
                indices[i]=true;
            }
			int min = std::min(filters[i].selectionStartY, filters[i].selectionEndY);
			int max = std::max(filters[i].selectionStartY, filters[i].selectionEndY);
			for (int k = 0;k < values.size();k++)
			{
				CatColor catColor = getCatColor(cats[k]);
				float axisPosition=y_pos + height - values[k].val;
                if (columns[selectedColIndex].inverted){
                    //cout<<"inverted"<<endl;
                    axisPosition =  y_pos + values[k].val;
                }

				
				if (axisPosition >= min && axisPosition <= max)
				{
					values[k].show = true;
					indices[k] = true;
					catColor.count++;
					//cout << values[i].val << endl;
				}
                else
				{
					values[k].show = false;
					indices[k] = false;
				}
				setCatColor(catColor);
			}
            mappedColumns[selectedColIndex].set_numeric_values(values);
            filters[i].column.set_numeric_values(values);
			for (int k = 0;k < columns.size();k++)
			{
				if (columns[k].get_type() == QUANTITATIVE && columns[k].filtered==false)
				{
					vector<DataPoint> points = mappedColumns[k].get_numeric_values();
					for (int l = 0;l < points.size();l++)
					{
                        if(isPointFiltered(l)==false)
                            points[l].show=true;
                        else
                            points[l].show=false;
                        
					}
					mappedColumns[k].set_numeric_values(points);
				}
			}
           
           
		}
	}
}

bool ParallelPlot::isPointFiltered(int pointIndex)
{
    for(int i=0;i<filters.size();i++)
    {
        vector<DataPoint> vals = filters[i].column.get_numeric_values();
        if(vals[pointIndex].show==false)
            return true;
    }
    return false;
}

void ParallelPlot::resetPoints()
{
	filters.clear();
	setupColorMapForCategories();
	for (int i = 0;i < mappedColumns.size();i++)
	{
		if (columns[i].get_type() == QUANTITATIVE) 
		{
			vector<DataPoint> points = mappedColumns[i].get_numeric_values();
			for (int j = 0;j < points.size();j++)
			{
				points[j].show = true;
			}
            columns[i].filtered=false;
            columns[i].inverted = false;
			mappedColumns[i].set_numeric_values(points);
		}
	}
}

void ParallelPlot::invertAxis(int columnIndex)
{
	vector<DataPoint> points = mappedColumns[columnIndex].get_numeric_values();
	for (int i = 0;i < points.size();i++)
	{
		points[i].invert = !points[i].invert;
	}
	mappedColumns[columnIndex].set_numeric_values(points);
	columns[columnIndex].inverted = !columns[columnIndex].inverted;
}

void ParallelPlot::drawLegend()
{
	ofPushMatrix();
	ofTranslate(x_pos-15, y_pos + height + 100);
	int size = columns[0].get_string_values().size();
	int xOffset = 0, yOffset=0;
	ofTrueTypeFont font;
	font.load("Serif.ttf", 8);
	int totalCounts = 0;
	for (int i = 0;i < colors.size();i++)
		totalCounts += colors[i].getCount();
	for (int i = 0;i < colors.size();i++)
	{
		CatColor current = colors[i];
		ofSetColor(current.getColor(),250);
		ofDrawRectangle(xOffset, yOffset, ((float)current.getCount()/(float)size)*500, 15);
		ofSetColor(0, 0, 0);
		string cnt = to_string(current.getCount());
			font.drawString(cnt + " "+ ofToUpper(current.getCategory()), xOffset + ((float)current.getCount() / (float)size) * 500 + 5,yOffset+10);
		yOffset += 30 ;
		if ((i+1) % 8 == 0) {
			yOffset = 0;
			xOffset += 225;
		}
	}
	ofPopMatrix();
}

bool ParallelPlot::checkFilterExists(Column column)
{
	for (int i = 0;i < filters.size();i++)
	{
		if (filters[i].column.get_name() == column.get_name())
			return true;
	}
	return false;
}

void ParallelPlot::reorderAxes(int firstColIndex, int secondColIndex)
{
	Column firstVals = mappedColumns[firstColIndex];
	Column secondVals = mappedColumns[secondColIndex];
	Column originalFirst = columns[firstColIndex];
	Column originalSecond = columns[secondColIndex];
	mappedColumns[firstColIndex].set_numeric_values(secondVals.get_numeric_values());
	mappedColumns[secondColIndex].set_numeric_values(firstVals.get_numeric_values());
	mappedColumns[firstColIndex].set_name(secondVals.get_name());
	mappedColumns[secondColIndex].set_name(firstVals.get_name());
	columns[firstColIndex].set_name(secondVals.get_name());
	columns[secondColIndex].set_name(firstVals.get_name());
	columns[firstColIndex].set_numeric_values(originalSecond.get_numeric_values());
	columns[secondColIndex].set_numeric_values(originalFirst.get_numeric_values());
}

void ParallelPlot::generateColors()
{
	presetColors.clear();
	presetColors.push_back(ofColor(166, 206, 227));
	presetColors.push_back(ofColor(31, 120, 180));
	presetColors.push_back(ofColor(178, 223, 138));
	presetColors.push_back(ofColor(51, 160, 44));
	presetColors.push_back(ofColor(251, 154, 153));
	presetColors.push_back(ofColor(227, 26, 28));
	presetColors.push_back(ofColor(253, 191, 111));
	presetColors.push_back(ofColor(225, 127, 0));
	presetColors.push_back(ofColor(202, 178, 214));
	presetColors.push_back(ofColor(106, 61, 154));
	presetColors.push_back(ofColor(255, 255, 153));
	presetColors.push_back(ofColor(177, 89, 40));
	presetColors.push_back(ofColor::magenta);
	presetColors.push_back(ofColor::darkCyan);
	presetColors.push_back(ofColor::goldenRod);
	presetColors.push_back(ofColor::cornsilk);
	presetColors.push_back(ofColor::darkSalmon);
	presetColors.push_back(ofColor::greenYellow);
	presetColors.push_back(ofColor::maroon);
	presetColors.push_back(ofColor::oliveDrab);
	presetColors.push_back(ofColor::peru);
	presetColors.push_back(ofColor::sienna);
	presetColors.push_back(ofColor::teal);
	presetColors.push_back(ofColor::darkKhaki);
	presetColors.push_back(ofColor::hotPink);
	presetColors.push_back(ofColor::mediumOrchid);
	presetColors.push_back(ofColor::blueSteel);
	presetColors.push_back(ofColor::blanchedAlmond);
	presetColors.push_back(ofColor::darkSlateGrey);
	presetColors.push_back(ofColor::indigo);
	presetColors.push_back(ofColor::tan);
	presetColors.push_back(ofColor::mediumSpringGreen);
	presetColors.push_back(ofColor::saddleBrown);
	presetColors.push_back(ofColor::lavenderBlush);
	presetColors.push_back(ofColor::lightCoral);
	presetColors.push_back(ofColor::rosyBrown);
	presetColors.push_back(ofColor::forestGreen);
	presetColors.push_back(ofColor::cadetBlue);
}

CatColor ParallelPlot::getCatColor(string cat)
{
	if (numCategoricalAttributes == 1)
		cat = split(cat, ' ')[0];
	for (int i = 0;i < colors.size();i++)
	{
		if (colors[i].getCategory() == cat)
			return colors[i];
	}
	CatColor test(NULL,NULL,-1);
	return test;
}

void ParallelPlot::setCatColor(CatColor catColor)
{
	
	for (int i = 0;i < colors.size();i++)
	{
		if (colors[i].getCategory() == catColor.getCategory())
			colors[i].count = catColor.count;
	}
}