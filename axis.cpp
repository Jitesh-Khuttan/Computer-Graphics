#include "axis.h"
#include "color.h"
#include "thickness.h"
extern GLsizei width;
extern GLsizei height;

void Axis :: drawAxis()
{
	glColor3ubv(Color::GREEN);
	glLineWidth(Thickness::THICKNESS2);
	glBegin(GL_LINES);
		glVertex2i(0,height/2);
		glVertex2i(0,-height/2);
		glVertex2i(width/2,0);
		glVertex2i(-width/2,0);
	glEnd();
}
