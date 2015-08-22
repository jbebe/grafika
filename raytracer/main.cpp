#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <GL/freeglut_std.h>
#include <string.h>
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

char dbg_str[256];

const int window_width = 600;
const int window_height = 600;

#include <memory>
#include "misc/array.hpp"
#include "graphics/general/misc.hpp"
#include "graphics/general/vector.hpp"
#include "graphics/general/color.hpp"
#include "graphics/general/rtti_image.hpp"
#include "graphics/opengl/debug.hpp"
#include "graphics/raytracer/misc.hpp"
#include "graphics/raytracer/ray.hpp"
#include "graphics/raytracer/object.hpp"
#include "graphics/raytracer/intersection.hpp"
#include "graphics/raytracer/scene.hpp"
#include "graphics/raytracer/shapes/sphere.hpp"
#include "graphics/raytracer/shapes/plane.hpp"
#include "graphics/raytracer/engine.hpp"

rtti_image image(window_width, window_height);
std::auto_ptr<rt::Engine> gbl_engine;
std::auto_ptr<rt::Scene> gbl_scene;

void onInitialization(){
	glViewport(0, 0, window_width, window_height);
	
	gbl_scene = std::auto_ptr<rt::Scene>(new rt::Scene(Color(0, 0, 0)));
	rt::Scene &scene = *gbl_scene;

	Vec3d light1_pos(200, 100, 0);
	Vec3d light2_pos(-400, -50, 210);
	
	scene.addLight(new rt::Light3d(Color(255, 0, 0), light1_pos));
	//scene.addLight(new rt::Light3d(Color(255, 255, 255), light2_pos));
	
	scene.setBackground(Color(0, 0, 0));

	const int off_y = -150;
	const int off_z = 300;
	const int off_x = 0;

	/*light1_pos.y += 20;
	light1_pos.z += 10;
	scene.addObject(new rt::Sphere(light1_pos, 10));
	scene.objects.back()->color = Color::fromByteRGB(255, 255, 255);*/
	
	/*light2_pos.y += 20;
	light2_pos.z += 10;
	scene.addObject(new rt::Sphere(light2_pos, 10));
	scene.objects.back()->color = Color::fromByteRGB(255, 255, 255);*/
	
	scene.addObject(new rt::Sphere(Vec3d(-150+off_x, 50+off_y, 500+off_z), 150));
	scene.objects.back()->color = Color::fromByteRGB(255, 255, 255);
	scene.objects.back()->refl = true;
	
	scene.addObject(new rt::Sphere(Vec3d(150+off_x, -50+off_y, 400+off_z), 100));
	scene.objects.back()->color = Color::fromByteRGB(255, 255, 0);
	scene.objects.back()->refl = true;
	
	scene.addObject(new rt::Sphere(Vec3d(-100+off_x, -100+off_y, 100+off_z), 50));
	scene.objects.back()->color = Color::fromByteRGB(255, 0, 0);
	scene.objects.back()->refl = true;
	
	scene.addObject(new rt::Sphere(Vec3d(off_x, -50+off_y, 150+off_z), 50));
	scene.objects.back()->color = Color::fromByteRGB(255, 0, 255);
	scene.objects.back()->refl = true;
	
	// alul
	scene.addObject(new rt::Plane(Vec3d(0, -150+off_y, 0), Vec3d(0, 1, 0)));
	scene.objects.back()->color = Color::fromByteRGB(50, 50, 50);
	scene.objects.back()->refl = true;
	
	// szemben
	scene.addObject(new rt::Plane(Vec3d(0, 0, 700+off_z), Vec3d(0, 0, -1)));
	scene.objects.back()->color = Color::fromByteRGB(50, 50, 50);
	scene.objects.back()->refl = true;

	// bal
	scene.addObject(new rt::Plane(Vec3d(-400, 0, 0), Vec3d(1, 0, 0)));
	scene.objects.back()->color = Color::fromByteRGB(50, 50, 50);
	scene.objects.back()->refl = true;
	
	// jobb
	scene.addObject(new rt::Plane(Vec3d(400, 0, 0), Vec3d(-1, 0, 0)));
	scene.objects.back()->color = Color::fromByteRGB(50, 50, 50);
	scene.objects.back()->refl = true;
	
	// hátul
	scene.addObject(new rt::Plane(Vec3d(0, 0, -100), Vec3d(0, 0, 1)));
	scene.objects.back()->color = Color::fromByteRGB(50, 50, 50);
	scene.objects.back()->refl = true;
	/*
	scene.addObject(new rt::Sphere(Vec3d(50+off_x, 0+off_y, 0+off_z), 100));
	scene.objects.back()->color = Color::fromByteRGB(255, 255, 255);
	scene.objects.back()->frac = Color(1.1, 1.1, 1.1);
	scene.objects.back()->fresnel = Color(1.0, 1.0, 1.0);
	scene.objects.back()->refl = true;
	scene.objects.back()->refr = true;
	*/
	
	gbl_engine = std::auto_ptr<rt::Engine>(new rt::Engine(
		&scene, 
		window_width, window_height, 
		rt::Cam3d(Vec3d(0, 0, -(window_width/2.0)/tan(rt::FOV_R)), Vec3d(0, 0, 1)), 
		2, // samples (1 - 1x, 2 - 4x, ...)
		0  // max reflections
	));
	//engine->render(&image);
}

void onDisplay(){
    /*
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*/
    glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, image.data);
    glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y){
    if (key == 'd')
		glutPostRedisplay();
}

void onKeyboardUp(unsigned char key, int x, int y){}

void onMouse(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		glutPostRedisplay();
}

void onMouseMotion(int x, int y){}

void onIdle(){
    //long time = glutGet(GLUT_ELAPSED_TIME);
	static bool finished = false;
	if (!finished){
		double percent = gbl_engine->render_slice(&image);
		if (percent == 1.0){
			finished = true;
		}
		snprintf(dbg_str, 255, "Remaining: %f%%", (1.0-percent));
		glutPostRedisplay();
	}
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();					// Esemenykezelo hurok

    return 0;
}

