#include "symmetricDDA.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern int width,height;

pair<int,int> currentCoordinates;


SymmetricDDA::SymmetricDDA(GLubyte* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Symmetric Line";
}

void SymmetricDDA :: reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected SymmetricDDA "<<endl;
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
       	coordinates.clear();
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
        glColor3ubv(color);
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



