#include "object.h"
#include "axis.h"
#include "color.h"
#include "thickness.h"
#include "pattern.h"
#include "circle.h"
#include "ellipse.h"
#include "bezier.h"

extern list<Object*> allObjects;
extern int width,height;

void Object ::setColor(GLubyte* color)
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
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	if(this->objectName != "Ellipse" && this->objectName != "Bezier")
	{
		list< pair<int,int> >:: iterator it;
		for(it = vertices.begin(); it!= vertices.end();it++)
		{
			(*it).first += dx;
			(*it).second += dy;
		}
		if(objectName == "Circle")
		{
			if(((Circle*)this)->filled)
			{
				((Circle*)this)->rePaintFilledCoordinates();
				for(it = ((Circle*)this)->filledCoordinates.begin(); it!= ((Circle*)this)->filledCoordinates.end();it++)	//Downcasting for accessing filledCoordinates
				{
					(*it).first += dx;
					(*it).second += dy;
				}
				Axis::drawAxis();
			}
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
	else if(this->objectName == "Ellipse")		//For Ellipse
	{	
		list< pair<int,int> >:: iterator it;
		for(it = coordinates.begin(); it!= coordinates.end();it++)
		{
			(*it).first += dx;
			(*it).second += dy;
		}
		
		if(((EllipseObject*)this)->filled)
		{
			((EllipseObject*)this)->rePaintFilledCoordinates();
			for(it = ((EllipseObject*)this)->filledCoordinates.begin(); it!= ((EllipseObject*)this)->filledCoordinates.end();it++)	//Downcasting for accessing filledCoordinates
			{
				(*it).first += dx;
				(*it).second += dy;
			}
			Axis::drawAxis();
		}
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}
	else if(this->objectName == "Bezier")
	{
		coordinates.clear();
		glColor3ubv(Color::BLACK);
		list< pair<int,int> > :: iterator it;
		for(it = ((Bezier*)(this))->controlPoints.begin();it != ((Bezier*)(this))->controlPoints.end();it++)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first,(*it).second);
			glEnd();
			(*it).first += dx;
			(*it).second += dy;
		}
		((Bezier*)(this))->drawCurve(((Bezier*)(this))->controlPoints);		//Redrawing the curve at new translated coordinates
		
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}
}

void Object :: rotateObject(int rotationAngle,pair<int,int> pivotPoint)
{
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	if(this->objectName != "Ellipse" && this->objectName != "Bezier")
	{
		int tempx,tempy;
	
		list< pair<int,int> >:: iterator it;
		for(it = vertices.begin(); it!= vertices.end();it++)
		{
			tempx = (*it).first;
			tempy = (*it).second;
			(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
			(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
		}
		
		if(objectName == "Circle")
		{
			if(((Circle*)this)->filled)
			{
				((Circle*)this)->rePaintFilledCoordinates();
				for(it = ((Circle*)this)->filledCoordinates.begin(); it!= ((Circle*)this)->filledCoordinates.end();it++)	//Downcasting for accessing filledCoordinates
				{
					tempx = (*it).first;
					tempy = (*it).second;
					(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
					(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
				}
				Axis::drawAxis();
			}
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
	else if(this->objectName == "Ellipse")		//For Ellipse
	{
		int tempx,tempy;
	
		list< pair<int,int> >:: iterator it;
		for(it = coordinates.begin(); it!= coordinates.end();it++)
		{
			tempx = (*it).first;
			tempy = (*it).second;
			(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
			(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
		}
		
		if(((EllipseObject*)this)->filled)
		{
			((EllipseObject*)this)->rePaintFilledCoordinates();
			for(it = ((EllipseObject*)this)->filledCoordinates.begin(); it!= ((EllipseObject*)this)->filledCoordinates.end();it++)	//Downcasting for accessing filledCoordinates
			{
				tempx = (*it).first;
				tempy = (*it).second;
				(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
				(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
			}
			Axis::drawAxis();
		}
	
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}
	else if(this->objectName == "Bezier")
	{
		int tempx,tempy;
		coordinates.clear();
		glColor3ubv(Color::BLACK);
		list< pair<int,int> > :: iterator it;
		for(it = ((Bezier*)(this))->controlPoints.begin();it != ((Bezier*)(this))->controlPoints.end();it++)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first,(*it).second);
			glEnd();
			tempx = (*it).first;
			tempy = (*it).second;
			(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
			(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
		}
		((Bezier*)(this))->drawCurve(((Bezier*)(this))->controlPoints);		//Redrawing the curve at new translated coordinates
		
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}
	
}


void Object :: scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint)
{
	cout<<"\n\tPivot Points: "<<pivotPoint.first<<" "<<pivotPoint.second;
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	if(this->objectName != "Ellipse" && this->objectName != "Bezier")	//This code is same for lines and Circle
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
		draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
		
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}	
	}
	
	else if(this->objectName == "Ellipse")
	{
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
	else if(this->objectName == "Bezier")
	{
		coordinates.clear();
		glColor3ubv(Color::BLACK);
		list< pair<int,int> > :: iterator it;
		for(it = ((Bezier*)(this))->controlPoints.begin();it != ((Bezier*)(this))->controlPoints.end();it++)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first,(*it).second);
			glEnd();
			(*it).first += (*it).first * scaleValue.first + pivotPoint.first * (1 - scaleValue.first);
			(*it).second += (*it).second * scaleValue.second + pivotPoint.second * (1 - scaleValue.second);
		}
		((Bezier*)(this))->drawCurve(((Bezier*)(this))->controlPoints);		//Redrawing the curve at new translated coordinates
		
		Axis::drawAxis();
		list<Object*>:: iterator i;
		for(i = allObjects.begin(); i!= allObjects.end();i++)
		{
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
		}
	}	
}


void Object :: fillBoundary(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor,int type)
{
	this->fillColor = fillColor;
	filled = true;
	if(type == 1)
		fill4(x,y,fillColor,boundaryColor);
	else if(type == 2)
		fill8(x,y,fillColor,boundaryColor);
	
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

void Object :: fill4(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor)
{	
	GLubyte interiorColor[3];
	glReadPixels(x,height - y,1,1,GL_RGB,GL_UNSIGNED_BYTE,interiorColor);

	if((interiorColor[0] != fillColor[0] || interiorColor[1] != fillColor[1] || interiorColor[2] != fillColor[2]) && 
	(interiorColor[0] != boundaryColor[0] || interiorColor[1] != boundaryColor[1] || interiorColor[2] != boundaryColor[2]))
	{
		glColor3ubv(fillColor);
		glPointSize(Thickness::THICKNESS1);
		glBegin(GL_POINTS);
			glVertex2i(x - width/2,height/2 - y);
			filledCoordinates.push_back(make_pair(x-width/2,height/2 - y));
		glEnd();
		glFlush();
		fill4(x+1,y,fillColor,boundaryColor);
		fill4(x-1,y,fillColor,boundaryColor);
		fill4(x,y+1,fillColor,boundaryColor);
		fill4(x,y-1,fillColor,boundaryColor);
	}
}


void Object :: fill8(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor)
{	
	GLubyte interiorColor[3];
	glReadPixels(x,height - y,1,1,GL_RGB,GL_UNSIGNED_BYTE,interiorColor);

	if((interiorColor[0] != fillColor[0] || interiorColor[1] != fillColor[1] || interiorColor[2] != fillColor[2]) && 
	(interiorColor[0] != boundaryColor[0] || interiorColor[1] != boundaryColor[1] || interiorColor[2] != boundaryColor[2]))
	{
		glColor3ubv(fillColor);
		glPointSize(Thickness::THICKNESS1);
		glBegin(GL_POINTS);
			glVertex2i(x - width/2,height/2 - y);
			filledCoordinates.push_back(make_pair(x-width/2,height/2 - y));
		glEnd();
		glFlush();
		
		fill8(x+1,y+1,fillColor,boundaryColor);
		fill8(x+1,y-1,fillColor,boundaryColor);
		fill8(x-1,y+1,fillColor,boundaryColor);
		fill8(x-1,y-1,fillColor,boundaryColor);
		
		fill8(x+1,y,fillColor,boundaryColor);
		fill8(x-1,y,fillColor,boundaryColor);
		fill8(x,y+1,fillColor,boundaryColor);
		fill8(x,y-1,fillColor,boundaryColor);
	}
}

