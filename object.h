#ifndef OBJECT
#define OBJECT

#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

//Abstract Class
class Object
{
	
private:
	void fill4(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor);
	void fill8(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor);
public:
    list< pair<int,int> > coordinates;
    list< pair<int,int> > vertices;
    GLubyte* color;
    string pattern;
    int thickness;
    int patternIndex;
    string objectName;
    GLubyte* fillColor;
	list< pair<int,int> > filledCoordinates;
	bool filled;

    void translateObject(int,int);
    void rotateObject(int rotationAngle,pair<int,int> pivotPoint);
    void setPattern(string pattern);
    void setThickness(int thickness);
    void setColor(GLubyte *color);
	void printCoordinates();
    bool selectObject(pair<int,int>);
    void scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint);
    void fillBoundary(int x,int y,GLubyte* fillColor,GLubyte* selectedObject,int type);
    void floodFill(int x, int y, GLubyte* fillColor);
    virtual void draw(int startX,int endX,int startY,int endY,int width,int height) = 0;
    virtual void reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw) = 0;
};

#endif // OBJECT
