#ifndef SIMPLEDDA
#define SIMPLEDDA

#include "object.h"
#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

class SimpleDDA : public Object
{
public:
    SimpleDDA(GLubyte* color,int thickness,string pattern);
    void draw(int startX,int endX,int startY,int endY,int width,int height);
    void reDrawSelectedObject(GLubyte*,int);
};


#endif
