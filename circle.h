#ifndef CIRCLE
#define CIRCLE

#include<bits/stdc++.h>
#include<GL/glut.h>
#include "object.h"
using namespace std;

class Circle : public Object
{
    public:
    	int centerX,centerY;
        Circle(float* color,int thickness,string pattern);
        void clipPointsOfCircle(int X,int Y);
        void putPixel(int Xc,int Yc,int currentx,int currenty);
        void draw(int startX,int endX,int startY,int endY,int width,int height);
        void translateObject(int,int);
        void rotateObject(int rotationAngle,pair<int,int> pivotPoint);
        void setPattern(string pattern);
        void setThickness(int thickness);
        void setColor(float *color);
        void printCoordinates();
        bool selectObject(pair<int,int>);
        void reDrawSelectedObject(float*,int);
        void fillBoundary(int x,int y,float* fillColor,float* boundaryColor);
        void scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint);
};

#endif // CIRCLE
