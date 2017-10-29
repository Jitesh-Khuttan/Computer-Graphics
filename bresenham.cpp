#include "bresenham.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern int height,width;

Bresenham :: Bresenham(float* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Bresenham Line";
}

void Bresenham :: printCoordinates()
{
	list< pair<int,int> > :: iterator it;
	for(it = coordinates.begin(); it != coordinates.end();it++)
	{
		cout<<"\n\tCoordinates: ("<<(*it).first<<","<<(*it).second<<")"<<endl;
	}
}

bool Bresenham :: selectObject(pair<int,int> clickedCoordinates)
{
	cout<<"Inside Select Object Of Bresenham"<<endl;
	for(list< pair<int,int> >:: iterator it = coordinates.begin(); it != coordinates.end(); it++)
	{
		if(((*it).first >= clickedCoordinates.first -10 && (*it).first <= clickedCoordinates.first + 10 ) && ((*it).second >= clickedCoordinates.second) - 10 && (*it).second <= clickedCoordinates.second+10)
		{
			reDrawSelectedObject(Color::NAVYBLUE,thickness+2);
			return true;
		}
	}
	return false;
}


void Bresenham :: reDrawSelectedObject(float* colorToDraw,int thicknessToDraw)
{
	cout<<"Redrawing Selected Bresenham "<<endl;
	glPointSize(thicknessToDraw);
	glColor3fv(colorToDraw);
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


void Bresenham :: translateObject(int dx,int dy)
{
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
				(*it).first += dx;
				(*it).second += dy;
	}

	list<Object*>:: iterator i;
	for(i = allObjects.begin(); i!= allObjects.end();i++)
	{
		(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
	}
}

void Bresenham ::scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint)
{
	list< pair<int,int> > :: iterator it;
	for(it = vertices.begin(); it!= vertices.end();it++)
	{
				(*it).first += (*it).first * scaleValue.first + pivotPoint.first * (1 - scaleValue.first);
				(*it).second += (*it).second * scaleValue.second + pivotPoint.second * (1 - scaleValue.second);
	}
	int startX,startY,endX,endY;
	int counter = 1;

	for(it = vertices.begin(); it!= vertices.end();it++)
	{
		if(counter == 1)
		{
			startX = (*it).first;
			startY = (*it).second;
		}
		else if(counter == 2)
		{
			endX =(*it).first;
			endY = (*it).second;
		}
		counter++;
	}
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
}


void Bresenham ::setColor(float* color)
{
    this->color = color;
}

void Bresenham ::setPattern(string pattern)
{
    this->pattern = pattern;
}

void Bresenham :: setThickness(int thickness)
{
    this->thickness = thickness;
}


void Bresenham ::rotateObject(int rotationAngle,pair<int,int> pivotPoint) 
{
	cout<<"Rotation Angle: "<<rotationAngle<<endl;
	int tempx,tempy;
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		tempx = (*it).first;
		tempy = (*it).second;
		(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
		(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
	}

	list<Object*>:: iterator i;
	for(i = allObjects.begin(); i!= allObjects.end();i++)
	{
		(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
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
	cout<<"inside bresenham drawing"<<endl;
    pair<int,int> currentCoordinates;
	float dx, dy;
	int x, y, steps;
	bool LineSlopeGT1;
	startX = startX - width/2;
    startY = height/2 - startY;
    endX = endX - width/2;
    endY = height/2 - endY;
    
    vertices.clear();
    vertices.push_back(pair<int,int>(startX,startY));
    vertices.push_back(pair<int,int>(endX,endY));
	
	x = startX;
	y = startY;
	dx = fabs(endX - startX);
	dy = fabs(endY - startY);

	steps = max(fabs(dx), fabs(dy));

	//cout << "Steps: " << steps << endl << endl;
	//cout << "(X,Y): (" << x << "," << y << ")" << endl;
	glColor3fv(color);
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


