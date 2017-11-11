#ifndef BRESENHAM
#define BRESENHAM

#include<bits/stdc++.h>
#include<GL/glut.h>
#include "object.h"
using namespace std;

class Bresenham : public Object
{
private:
	void fill(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor);
public:
	GLubyte* fillColor;
	list< pair<int,int> > filledCoordinates;
	bool filled;
    Bresenham(GLubyte* color,int thickness,string pattern);
    void BslopeGT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void BslopeLT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void draw(int startX,int endX,int startY,int endY,int width,int height);
    void reDrawSelectedObject(GLubyte*,int);
    void fillBoundary(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor,int type);
};
#endif // BRESENHAM
