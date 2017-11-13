#ifndef BEZIER
#define BEZIER

#include<bits/stdc++.h>
#include<GL/glut.h>
#include "object.h"
using namespace std;

class Bezier : public Object
{
	
	private:
		int calculateCoefficient(int n,int k);
		int factorial(int number);
	public:
		int totalControlPoints;
		list< pair<int,int> > controlPoints;
		
		Bezier(GLubyte* color,int thickness,string pattern);
		
		void draw(int startX,int endX,int startY,int endY,int width,int height){
		}
    	void reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw);
    	void drawCurve(list< pair<int,int> > controlPoints);
};


#endif
