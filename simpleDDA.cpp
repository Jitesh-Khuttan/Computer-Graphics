#include "simpleDDA.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern int width,height;

SimpleDDA :: SimpleDDA(float* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Simple Line";
}

void SimpleDDA :: printCoordinates()
{
	list< pair<int,int> > :: iterator it;
	for(it = coordinates.begin(); it != coordinates.end();it++)
	{
		cout<<"\n\tCoordinates: ("<<(*it).first<<","<<(*it).second<<")"<<endl;
	}
}

bool SimpleDDA :: selectObject(pair<int,int> clickedCoordinates)
{
	cout<<"Inside Select Object Of SimpleDDA"<<endl;
	for(list< pair<int,int> >:: iterator it = coordinates.begin(); it != coordinates.end(); it++)
	{
		if(((*it).first >= clickedCoordinates.first -1 && (*it).first <= clickedCoordinates.first + 1 ) && ((*it).second >= clickedCoordinates.second) - 1 && (*it).second <= clickedCoordinates.second+1)
		{
			reDrawSelectedObject(Color::NAVYBLUE,thickness+2);
			return true;
		}
	}
	return false;
}


void SimpleDDA :: reDrawSelectedObject(float* colorToDraw,int thicknessToDraw)
{
	cout<<"Redrawing Selected SimpleDDA "<<endl;
	glColor3fv(colorToDraw);
	glPointSize(thicknessToDraw);
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


void SimpleDDA :: translateObject(int dx,int dy)
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
void SimpleDDA ::setColor(float* color)
{
    this->color = color;
}

void SimpleDDA ::setPattern(string pattern)
{
    this->pattern = pattern;
}

void SimpleDDA :: setThickness(int thickness)
{
    this->thickness = thickness;
}


void SimpleDDA ::rotateObject(int rotationAngle,pair<int,int> pivotPoint) 
{
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

void SimpleDDA ::scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint)
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



void SimpleDDA::draw(int startX,int endX,int startY,int endY,int width,int height)
{

        double xinc,yinc;
        int maxValue,loop,dx,dy;
		float C_X,C_Y;
		pair<int,int> currentCoordinates;
        cout<<"\n\tReceived: StartX:"<<startX<<" StartY: "<<startY<<" endX:"<<endX<<" endY:"<<endY;
        startX = startX - width/2;
        startY = height/2 - startY;
        endX = endX - width/2;
        endY = height/2 - endY;
        cout<<"\n\tConverted: StartX:"<<startX<<" StartY: "<<startY<<" endX:"<<endX<<" endY:"<<endY;
        
        vertices.clear();
	    vertices.push_back(pair<int,int>(startX,startY));
	    vertices.push_back(pair<int,int>(endX,endY));
        
		dy = (endY - startY);
        dx = (endX - startX);


        maxValue = max(dy,dx);
        if(maxValue < 0)
           maxValue = abs(maxValue);
        xinc = (GLdouble)dx/maxValue;
        yinc = (GLdouble)dy/maxValue;
        cout<<"\n\txinc: "<<xinc<<" yinc:"<<yinc;
        C_X = startX;
        C_Y = startY;

        //glClear(GL_COLOR_BUFFER_BIT);
        glColor3fv(color);
        //glPointSize(5.0);
		glPointSize(thickness);
        for(loop=1;loop<=maxValue;loop++)
        {
        	patternIndex = patternIndex%10;
        	if(pattern[patternIndex] == 49)
        	{
        		glBegin(GL_POINTS);
        			//cout<<"\n\tPutting Point at: ("<<round(C_X)<<","<<round(C_Y)<<")";
                	glVertex2i(round(C_X),round(C_Y));
                glEnd();
        	}
        	
	        currentCoordinates.first = round(C_X);
	        currentCoordinates.second = round(C_Y);
	        coordinates.push_back(currentCoordinates);
	        
            C_X += xinc;
            C_Y += yinc;
			patternIndex++;
        }
        glFlush();

}


