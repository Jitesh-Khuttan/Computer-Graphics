#ifndef MIDPOINT
#define MIDPOINT
#include "object.h"
#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

class MidPoint : public Object
{

public:
    MidPoint(GLubyte* color,int thickness,string pattern);
    void MslopeGT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void MslopeLT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void draw(int startX,int endX,int startY,int endY,int width,int height);
    void reDrawSelectedObject(GLubyte*,int);
};

#endif // MIDPOINT
