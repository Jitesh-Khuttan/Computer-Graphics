#ifndef SYMMETRICDDA
#define SYMMETRICDDA

#include "object.h"
#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

class SymmetricDDA : public Object
{
public:
    SymmetricDDA(float* color,int thickness,string pattern);
    void draw(int startX,int endX,int startY,int endY,int width,int height);
    void translateObject(int,int);
    void rotateObject(int rotationAngle,pair<int,int> pivotPoint);
    void setPattern(string pattern);
    void setThickness(int thickness);
    void setColor(float *color);
    void printCoordinates();
    bool selectObject(pair<int,int>);
    void reDrawSelectedObject(float*,int);
    void scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint);
};


#endif // SYMMETRICDDA