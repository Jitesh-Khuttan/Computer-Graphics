#include "bspline.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"


Bspline :: Bspline(GLubyte* color,int thickness,string pattern)
{
	this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Bspline";
}


void Bspline :: reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected Circle "<<endl;
	glColor3ubv(colorToDraw);
	glPointSize(Thickness::THICKNESS10);
	if(colorToDraw == Color::BLACK)
	{
		list< pair<int,int> > :: iterator it;
		for(it = coordinates.begin(); it != coordinates.end();it++)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first,(*it).second);
			glEnd();	
		}
	}
	glPointSize(thicknessToDraw);
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		glBegin(GL_POINTS);
			glVertex2i((*it).first, (*it).second);
		glEnd();
	}
	glFlush();
}


float Bspline :: calculateCoefficient(float u,int k,int degree)
{
	if(degree == 1)
	{
		if(u >= float(k) && u <= float((k+1)))
			return 1;
		else 
			return 0;
	}
	else
	{
		return((u-k)/(degree-1)*calculateCoefficient(u,k,degree-1) + (k+degree - u)/(degree-1)*calculateCoefficient(u,k+1,degree-1));
	}
}

void Bspline :: drawCurve(list< pair<int,int> > controlPoints,int degree)
{
	int k,n,umin,umax;
	cout<<"\n\tDrawing Curve";
	this->controlPoints = controlPoints;
	totalControlPoints = controlPoints.size();
	
	this->degree = degree;
	n = totalControlPoints-1;
	umin = degree-1;
	umax = totalControlPoints;
	
	list< pair<int,int> > :: iterator it;
	
	for(float u = float(umin);u<=float(umax);u += 0.01)
	{
		//cout<<"\n\tcalculationg coordinates";
		pair<int,int> pixelCoordinate;
		k = 0;
		for(it = controlPoints.begin();it != controlPoints.end(); it++)
		{
			pixelCoordinate.first += (*it).first * calculateCoefficient(u,k,degree);
			pixelCoordinate.second += (*it).second * calculateCoefficient(u,k,degree);
			k++;
		}
		coordinates.push_back(pixelCoordinate);
	}
	
	
	glColor3ubv(color);
	glPointSize(thickness);
	
	for(it = coordinates.begin();it != coordinates.end();it++)
	{
		patternIndex = patternIndex%10;
		if(pattern[patternIndex] == 49)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first,(*it).second);
			glEnd();
		}
		patternIndex++;
	}	
}
