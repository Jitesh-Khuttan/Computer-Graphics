#ifndef BRESENHAM
#define BRESENHAM

#include<bits/stdc++.h>
#include<GL/glut.h>
#include "object.h"
using namespace std;

class Bresenham : public Object
{
public:
    Bresenham(float* color,int thickness,string pattern);
    void BslopeGT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void BslopeLT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void draw(int startX,int endX,int startY,int endY,int width,int height);
    void translateObject(int,int);
    void rotateObject(int rotationAngle,pair<int,int> pivotPoint);
    void scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint);
    void setPattern(string pattern);
    void setThickness(int thickness);
    void setColor(float *color);
    void printCoordinates();
    bool selectObject(pair<int,int>);
    void reDrawSelectedObject(float*,int);
};
#endif // BRESENHAM
