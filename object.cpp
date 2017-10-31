#include "object.h"
#include "axis.h"
#include "color.h"
#include "thickness.h"
#include "pattern.h"

extern list<Object*> allObjects;
extern int width,height;

void Object ::setColor(float* color)
{
    this->color = color;
}

void Object ::setPattern(string pattern)
{
    this->pattern = pattern;
}

void Object :: setThickness(int thickness)
{
    this->thickness = thickness;
}

bool Object :: selectObject(pair<int,int> clickedCoordinates)
{
	//cout<<"Inside Select Object Of MidPoint"<<endl;
	for(list< pair<int,int> >:: iterator it = coordinates.begin(); it != coordinates.end(); it++)
	{
		if(((*it).first + 2 >= clickedCoordinates.first  && (*it).first -2 <= clickedCoordinates.first ) && ((*it).second + 2 >= clickedCoordinates.second)  && (*it).second -2 <= clickedCoordinates.second)
		{
			reDrawSelectedObject(Color::NAVYBLUE,thickness+2);
			return true;
		}
	}
	return false;
}

void Object :: printCoordinates()
{
	list< pair<int,int> > :: iterator it;
	for(it = coordinates.begin(); it != coordinates.end();it++)
	{
		cout<<"\n\tCoordinates: ("<<(*it).first<<","<<(*it).second<<")"<<endl;
	}
}


void Object :: translateObject(int dx,int dy)
{
	if(this->objectName != "Ellipse")
	{
		reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
		Axis::drawAxis();
		
		list< pair<int,int> >:: iterator it;
		for(it = vertices.begin(); it!= vertices.end();it++)
		{
			(*it).first += dx;
			(*it).second += dy;
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
		draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
	
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}
	else
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
}

void Object :: rotateObject(int rotationAngle,pair<int,int> pivotPoint)
{
	if(this->objectName != "Ellipse")
	{
		int tempx,tempy;
		reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
		Axis::drawAxis();
	
		list< pair<int,int> >:: iterator it;
		for(it = vertices.begin(); it!= vertices.end();it++)
		{
			tempx = (*it).first;
			tempy = (*it).second;
			(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
			(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
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
		draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
	
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}
	else
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
	
}


void Object :: scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint)
{
	if(this->objectName != "Ellipse")
	{
		reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
		Axis::drawAxis();
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
		draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
		
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}	
	}
	
	else
	{
		pivotPoint.first = 0;
		pivotPoint.second = 0;
		reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
		Axis::drawAxis();
		list< pair<int,int> > :: iterator it;
		for(it = coordinates.begin(); it!= coordinates.end();it++)
		{
			(*it).first += (*it).first * scaleValue.first + pivotPoint.first * (1 - scaleValue.first);
			(*it).second += (*it).second * scaleValue.second + pivotPoint.second * (1 - scaleValue.second);
		}
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}	
}

