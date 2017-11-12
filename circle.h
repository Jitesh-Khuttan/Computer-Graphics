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
        Circle(GLubyte* color,int thickness,string pattern);
        void clipPointsOfCircle(int X,int Y);
        void putPixel(int Xc,int Yc,int currentx,int currenty);
        void draw(int startX,int endX,int startY,int endY,int width,int height);
        void reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw);
        void rePaintFilledCoordinates();
};

#endif // CIRCLE
