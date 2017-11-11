#include "bresenham.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern int height,width;

Bresenham :: Bresenham(GLubyte* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Bresenham Line";
}


void Bresenham :: reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected Bresenham "<<endl;
	glPointSize(thicknessToDraw);
	glColor3ubv(colorToDraw);
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		patternIndex = patternIndex%10;
		if(pattern[patternIndex] == 49)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first, (*it).second);
			glEnd();
		}
		patternIndex++;
	}
	glFlush();
}


void Bresenham :: fillBoundary(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor,int type)
{
	this->fillColor = fillColor;
	filled = true;
	if(type == 1)
		fill(x,y,fillColor,boundaryColor);
	
	reDrawSelectedObject(Color::BLACK,this->thickness+2);
	reDrawSelectedObject(this->color,this->thickness);
	//Redrawing the objects to the screen
	list<Object*>:: iterator i;
	for(i = allObjects.begin(); i!= allObjects.end();i++)
	{
		if(*i != this)
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
	}
}

void Bresenham :: fill(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor)
{	
	GLubyte interiorColor[3];
	glReadPixels(x,height - y,1,1,GL_RGB,GL_UNSIGNED_BYTE,interiorColor);

	if((interiorColor[0] != fillColor[0] || interiorColor[1] != fillColor[1] || interiorColor[2] != fillColor[2]) && (interiorColor[0] != boundaryColor[0] || interiorColor[1] != boundaryColor[1] || interiorColor[2] != boundaryColor[2]))
	{
		glColor3ubv(fillColor);
		glPointSize(Thickness::THICKNESS1);
		glBegin(GL_POINTS);
			glVertex2i(x - width/2,height/2 - y);
			filledCoordinates.push_back(make_pair(x-width/2,height/2 - y));
		glEnd();
		glFlush();
		fill(x+1,y,fillColor,boundaryColor);
		fill(x-1,y,fillColor,boundaryColor);
		fill(x,y+1,fillColor,boundaryColor);
		fill(x,y-1,fillColor,boundaryColor);
	}
}

void Bresenham :: BslopeLT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY)
{
    pair<int,int> currentCoordinates;
	//cout<<"\n\tThickness: "<<thickness;
	float prevDescParam, currDescParam;

	prevDescParam = (2 * dy) - dx;

	for (int i = 0; i < steps; i++)
	{
		if(endX > startX)
		{
			x++;
		}
		else if(endX < startX)
		{
			x--;
		}

		if (prevDescParam < 0)
		{
			currDescParam = prevDescParam + (2 * dy);
			prevDescParam = currDescParam;
		}
		else
		{
			if(endY > startY)
			{
				y++;
			}
			else if(endY < startY)
			{
				y--;
			}
			currDescParam = prevDescParam + (2 * dy) - (2 * dx);
			prevDescParam = currDescParam;
		}
		prevDescParam = currDescParam;

        currentCoordinates.first = x;
        currentCoordinates.second = y;
        coordinates.push_back(currentCoordinates);

		glPointSize(thickness);
		patternIndex = patternIndex%10;
		if(pattern[patternIndex] == 49)
		{
			glBegin(GL_POINTS);
				//glPointSize(50.0);
				glVertex2i(x, y);
			glEnd();
		}
		patternIndex++;
	}
	glFlush();
}

void Bresenham:: BslopeGT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY)
{
    pair<int,int> currentCoordinates;
	float prevDescParam, currDescParam;

	prevDescParam = (2 * dx) - dy;

	for (int i = 0; i < steps; i++)
	{
		if(endY > startY)
		{
			y++;
		}
		else if(endY < startY)
		{
			y--;
		}

		if (prevDescParam < 0)
		{
			currDescParam = prevDescParam + (2 * dx);
			prevDescParam = currDescParam;
		}
		else
		{
			if(endX > startX)
			{
				x++;
			}
			else if(endX < startX)
			{
				x--;
			}
			currDescParam = prevDescParam + (2 * dx) - (2 * dy);
			prevDescParam = currDescParam;
		}
		prevDescParam = currDescParam;

		currentCoordinates.first = x;
        currentCoordinates.second = y;
        coordinates.push_back(currentCoordinates);


		glPointSize(thickness);
		patternIndex = patternIndex%10;
		if(pattern[patternIndex] == 49)
		{
			glBegin(GL_POINTS);
				//glPointSize(50.0);
				glVertex2i(x, y);
			glEnd();
		}
		patternIndex++;
	}
	glFlush();
}

void Bresenham ::draw(int startX,int endX,int startY,int endY,int width,int height)
{
	//cout<<"inside bresenham drawing"<<endl;
    pair<int,int> currentCoordinates;
	float dx, dy;
	int x, y, steps;
	bool LineSlopeGT1;
	startX = startX - width/2;
    startY = height/2 - startY;
    endX = endX - width/2;
    endY = height/2 - endY;
    
    vertices.clear();
    coordinates.clear();
    vertices.push_back(pair<int,int>(startX,startY));
    vertices.push_back(pair<int,int>(endX,endY));
	
	x = startX;
	y = startY;
	dx = fabs(endX - startX);
	dy = fabs(endY - startY);

	steps = max(fabs(dx), fabs(dy));

	//cout << "Steps: " << steps << endl << endl;
	//cout << "(X,Y): (" << x << "," << y << ")" << endl;
	glColor3ubv(color);
	glPointSize((GLdouble)thickness);

	currentCoordinates.first = x;
    currentCoordinates.second = y;
    coordinates.push_back(currentCoordinates);
	glVertex2i(x, y);

	if(fabs(dx) > fabs(dy))
	{
		LineSlopeGT1 = false;
		//cout << "Slope LT 1" << endl;
		BslopeLT1(steps, x, y, dx, dy,startX,endX,startY,endY);
	}
	else
	{
		LineSlopeGT1 = true;
		//cout << "Slope GT 1" << endl;
		BslopeGT1(steps, x, y, dx, dy,startX,endX,startY,endY);
	}

}


