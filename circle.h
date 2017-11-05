#ifndef CIRCLE
#define CIRCLE

#include<bits/stdc++.h>
#include<GL/glut.h>
#include "object.h"
using namespace std;

class Circle : public Object
{
	
	private:
		void fillCircle4(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor);
		void fillCircle8(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor);
    public:
    	int centerX,centerY;
    	GLubyte* fillColor;
    	list< pair<int,int> > filledCoordinates;
    	bool filled;
    	
        Circle(GLubyte* color,int thickness,string pattern);
        void clipPointsOfCircle(int X,int Y);
        void putPixel(int Xc,int Yc,int currentx,int currenty);
        void draw(int startX,int endX,int startY,int endY,int width,int height);
        void reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw);
        void fillBoundary(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor,int type);
        void rePaintFilledCoordinates();
};

#endif // CIRCLE
