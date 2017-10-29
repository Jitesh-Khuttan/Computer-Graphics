#include "symmetricDDA.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern int width,height;

pair<int,int> currentCoordinates;


SymmetricDDA::SymmetricDDA(float* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Symmetric Line";
}

void SymmetricDDA :: printCoordinates()
{
	list< pair<int,int> > :: iterator it;
	for(it = coordinates.begin(); it != coordinates.end();it++)
	{
		cout<<"\n\tCoordinates: ("<<(*it).first<<","<<(*it).second<<")"<<endl;
	}
}


bool SymmetricDDA :: selectObject(pair<int,int> clickedCoordinates)
{
	cout<<"Inside Select Object Of SymmetricDDA "<<endl;
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


void SymmetricDDA :: reDrawSelectedObject(float* colorToDraw,int thicknessToDraw)
{
	cout<<"Redrawing Selected SymmetricDDA "<<endl;
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


void SymmetricDDA :: translateObject(int dx,int dy)
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
void SymmetricDDA ::setColor(float* color)
{
    this->color = color;
}

void SymmetricDDA ::setPattern(string pattern)
{
    this->pattern = pattern;
}

void SymmetricDDA :: setThickness(int thickness)
{
    this->thickness = thickness;
}

void SymmetricDDA ::rotateObject(int rotationAngle,pair<int,int> pivotPoint) 
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

void SymmetricDDA ::scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint)
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



void SymmetricDDA :: draw(int startX,int endX,int startY,int endY,int width,int height)
{
        int maxValue,dx,dy,LLE,n,loop;
        double xinc,yinc,C_X,C_Y;
        //cout<<"\n\tThickness: "<<thickness;
        //cout<<"\n\tReceived: StartX:"<<startX<<" StartY: "<<startY<<" endX:"<<endX<<" endY:"<<endY;
        startX = startX - width/2;
        startY = height/2 - startY;
        endX = endX - width/2;
        endY = height/2 - endY;
        //cout<<"\n\tConverted: StartX:"<<startX<<" StartY: "<<startY<<" endX:"<<endX<<" endY:"<<endY;
        
       	vertices.clear();
	    vertices.push_back(pair<int,int>(startX,startY));
	    vertices.push_back(pair<int,int>(endX,endY)); 
        
		dy = (endY - startY);
        dx = (endX - startX);

//        dy = (endY - startY);
//        dx = (endX - startX);
        //printf("\n\tValue of dy and dx are: %d %d",dy,dx);
        maxValue = max(dx,dy);
        if(maxValue < 0)
            maxValue = abs(maxValue);

        //printf("MaxValue is: %d",maxValue);
        for(loop=1;loop<=maxValue;loop++)
        {
                if(pow(2,loop) >= maxValue && pow(2,loop-1) <= maxValue)
                {
                        n = loop;
                        break;
                }
        }
        //printf("Value of n is: %d",n);
        LLE = pow(2,n);
        xinc = (float)dx/LLE;
        yinc = (float)dy/LLE;
        //printf("\n\tValue of LLE,xinc,yinc: %d %f %f",LLE,xinc,yinc);
        C_X = startX; //Current X
        C_Y = startY; //Current Y

        currentCoordinates.first = C_X;
        currentCoordinates.second = C_Y;
        coordinates.push_back(currentCoordinates);

        loop = 1;
        glColor3fv(color);
        //glPointSize((GLdouble)thickness);
        glPointSize(thickness);
       // cout<<"PATTERN IS: "<<pattern;
        while(loop<=LLE)
        {
        		patternIndex = (patternIndex)%10;
        		//cout<<"Pattern Type: "<<pattern[patternIndex]<<endl;
                glBegin(GL_POINTS);
                	if(pattern[patternIndex] == 49)
                	{
	        			//cout<<"\n\tPutting Point at: ("<<round(C_X)<<","<<round(C_Y)<<")";
	                	glVertex2i(round(C_X),round(C_Y));
	                }
                glEnd();
                C_X = C_X + xinc;
                C_Y = C_Y + yinc;

                currentCoordinates.first = C_X;
                currentCoordinates.second = C_Y;
                coordinates.push_back(currentCoordinates);
                loop++;
                patternIndex++;
        }
        glFlush();
}



