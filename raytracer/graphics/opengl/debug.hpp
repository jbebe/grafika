#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>
#include <GL/freeglut_std.h>

#pragma once

namespace gl {

void drawText(const char* str){
	static double counter = 0;
	glColor3f(1.f, 1.f, 1.f); 
	glRasterPos2f(-0.99, 0.97-counter);
	for (int i = 0, len = strlen(str); i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
	}
	counter += 0.04;
}

} // namespace gl