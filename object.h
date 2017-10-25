#ifndef OBJECT
#define OBJECT

#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

//Abstract Class
class Object
{
public:
    list< pair<int,int> > coordinates;
    list< pair<int,int> > vertices;
    float* color;
    string pattern;
    int thickness;
    int patternIndex;
    string objectName;

    virtual void draw(int startX,int endX,int startY,int endY,int width,int height) = 0;
    virtual void translateObject(int,int) = 0;
    virtual void rotateObject(int rotationAngle,pair<int,int> pivotPoint) = 0;
    virtual void setPattern(string pattern) = 0;
    virtual void setThickness(int thickness) = 0;
    virtual void setColor(float *color) = 0;
    virtual void printCoordinates() = 0;
    virtual bool selectObject(pair<int,int>) = 0;
    virtual void reDrawSelectedObject(float*,int) = 0;
    virtual void scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint) = 0;
    virtual void fillBoundary(int x,int y,float* fillColor,float* selectedObject) {}
};

#endif // OBJECT
