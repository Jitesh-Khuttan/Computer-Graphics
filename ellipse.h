#ifndef ELLIPSEOBJECT
#define ELLIPSEOBJECT

#include "object.h"
#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

class EllipseObject : public Object
{
    public:
    	int centerX,centerY,endX,endY;
        EllipseObject(float* color,int thickness,string pattern);
        void clipPointsOfEllipse(int X,int Y);
        void putEllipsePixel(int Xc,int Yc,int currentx,int currenty);
        void draw(int startX,int endX,int startY,int endY,int width,int height);
        void reDrawSelectedObject(float*,int);
};

#endif // ELLIPSEOBJECT
