#ifndef NGON
#define NGON
#include "object.h"
#include <bits/stdc++.h>
using namespace std;

class Ngon : public Object
{
	public:
		int totalVertices;
		float rotationAngle;
		list< pair<int,int> > verticeList;
		pair<int,int> pivotPoint;
		pair<int,int> startPoint;
		Ngon(float* color,int thickness,string pattern);
		void setVertices(int);
		void setRotationAngle(float);
		void draw(int startX,int endX,int startY,int endY,int width,int height);
	    void translateObject(int,int);
	    void rotateObject(int rotationAngle,pair<int,int> pivotPoint);
	    void setPattern(string pattern);
	    void setThickness(int thickness);
		void setColor(float *color);
	    void printCoordinates();
	    bool selectObject(pair<int,int>);
		void reDrawSelectedObject(float*,int);
	    void fillBoundary(int x,int y,float* fillColor,float* selectedObject) {
		}
		void scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint){
		}
};

#endif
