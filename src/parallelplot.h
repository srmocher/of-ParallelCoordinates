#pragma once
#include <vector>
#include "column.h"
#include "color.h"
#include <map>

class Filter;
class ParallelPlot
{
private:
	int width, height, axisLength;
	vector<Column> columns;
	vector<CatColor> colors;
	vector<Column> mappedColumns;
	vector<Filter> filters;
	void drawLabels();
	void drawAxes();
	void drawPoints();
	void drawRange();
	void drawSelection();
	void resetPoints();
	void reorderAxes(int first,int second);
	void drawLegend();
	void invertAxis(int);
	vector<Column> mapData();
	bool checkFilterExists(Column);
	void setupColorMapForCategories();
	void showSelectedPoints();
	ofColor getColorForRow(int);
	CatColor getCatColor(string);
	void setCatColor(CatColor);
	int x_pos, y_pos;
	bool registeredEvents = false,leftPressed;
	int selectionStartX, selectionStartY, selectionEndX, selectionEndY,selectableColumnIndex,columnDraggedIndex;
	bool canSelectionStart = false, canDrawSelection = false, dragging = false,columnDragged=true;
	float currentColInitialPosition;
	int legendColumnIndex,numCategorical,numCategoricalAttributes;
	map<string, ofColor> colorMap;
	vector<ofColor> presetColors;
	void generateColors();
    bool isPointFiltered(int);
   
public:
	void setup(int w,int h,vector<Column>,int,int,int);
	void draw();
	void mouseMoved(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseReleased(ofMouseEventArgs & args);
	void mouseScrolled(ofMouseEventArgs & args);
	void mouseEntered(ofMouseEventArgs & args);
	void mouseExited(ofMouseEventArgs & args);
	
};

class Filter
{
public:
	float selectionStartX;
	float selectionEndX;
	float selectionStartY;
	float selectionEndY;
	Column column;
	bool canDrawSelection;
};