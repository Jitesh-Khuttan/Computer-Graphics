#include "simpleDDA.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern int width,height;

SimpleDDA :: SimpleDDA(GLubyte* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Simple Line";
}


void SimpleDDA :: reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected SimpleDDA "<<endl;
	glColor3ubv(colorToDraw);
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


void SimpleDDA::draw(int startX,int endX,int startY,int endY,int width,int height)
{

        double xinc,yinc;
        int maxValue,loop,dx,dy;
		float C_X,C_Y;
		pair<int,int> currentCoordinates;
        //cout<<"\n\tReceived: StartX:"<<startX<<" StartY: "<<startY<<" endX:"<<endX<<" endY:"<<endY;
        startX = startX - width/2;
        startY = height/2 - startY;
        endX = endX - width/2;
        endY = height/2 - endY;
        //cout<<"\n\tConverted: StartX:"<<startX<<" StartY: "<<startY<<" endX:"<<endX<<" endY:"<<endY;
        
        vertices.clear();
        coordinates.clear();
	    vertices.push_back(pair<int,int>(startX,startY));
	    vertices.push_back(pair<int,int>(endX,endY));
        
		dy = (endY - startY);
        dx = (endX - startX);


        maxValue = max(dy,dx);
        if(maxValue < 0)
           maxValue = abs(maxValue);
        xinc = (GLdouble)dx/maxValue;
        yinc = (GLdouble)dy/maxValue;
        //cout<<"\n\txinc: "<<xinc<<" yinc:"<<yinc;
        C_X = startX;
        C_Y = startY;

        //glClear(GL_COLOR_BUFFER_BIT);
        glColor3ubv(color);
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


