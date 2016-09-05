/**
*	CS 334 - Fundamentals of Computer Graphics
*	Initial framework for assignment 1
*/

#include <iostream>
#include "GL/glut.h"

/* Constant values */
const float PI = 3.14159265359;

/* Information of the box world */
int world[8][8] = 
{
	{3, 0, 0, 0, 0, 0, 0, 5},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 3},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 2, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 4}
};

/* Window information */
float windowWidth = 800;
float windowHeight = 600;

/* #### The modelview matrix to load; remember OpenGL is row-major and not column major */
/* You do not necessarily need to use this matrix */
GLfloat myModelMat[4][4] = {
{ 1, 0, 0, 0 },
{ 0, 1, 0, 0 },
{ 0, 0, 1, 0 },
{ -4, -1, -26, 1 }
};


/* #### Define variables for the camera here... */
float xPosition = 0;
float zPosition = 0;
float xRotation = 0;
float yRotation = 0;
float zRotation = 0;

/*Variables for building animation*/
float buildingRotation = 0;
float buildingXLean = 0;
float buildingZLean = 0;

/* Field of view, aspect and near/far planes for the perspective projection */
float fovy = 45.0;
float aspect = windowWidth / windowHeight;
float zNear = 1.0;
float zFar = 100.0;


/* Vertices for a cube */
/* Note: Vertices are with respect to the origin */
float cubeVertices[24] = {
     1.0,  1.0,  1.0,        // Right - Top - Front
     1.0,  1.0,  0.0,        // Right - Top - Back
     0.0,  1.0,  0.0,        // Left - Top - Back
     0.0,  1.0,  1.0,        // Left - Top - Front
     1.0,  0.0,  1.0,        // Right - Bottom - Front
     1.0,  0.0,  0.0,        // Right - Bottom - Back
     0.0,  0.0,  0.0,        // Left - Bottom - Back
     0.0,  0.0,  1.0,        // Left - Bottom - Front
};

/* The indices for drawing the triangles that generate the faces of the cube */
/* 3 points per triangle x 2 triangles per face x 6 faces of the cube */
GLubyte cubeIndices[36] = {
    0, 1, 2,   2, 3, 0,        // Triangles for the top face
    4, 5, 6,   6, 7, 4,        // Triangles for the bottom face
    0, 1, 5,   5, 4, 0,        // Triangles for the Right face
    2, 3, 7,   7, 6, 2,        // Triangles for the left face
    0, 4, 7,   7, 3, 0,        // Triangles for the front face
    1, 2, 6,   6, 5, 1         // Triangles for the back face
};

/* Vertices for the floor plane */
float floorVertices[12] = 
{
	-1.0,  0.0, -1.0,
	-1.0,  0.0,  9.0,
	 9.0,  0.0,  9.0,
	 9.0,  0.0, -1.0
};

void drawAndColorElements() {
	glColor3f(1.0, 0.0, 0.0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices);
	glColor3f(0.0, 0.0, 0.0);
	glDrawElements(GL_LINE_STRIP, 36, GL_UNSIGNED_BYTE, cubeIndices);
}

void animateBuilding(int i, int j, int k, int frameCount) {
	float distance = sqrt(xPosition*xPosition + zPosition*zPosition);
	glTranslatef(i, k, j);

	if (i==0 && j==0) {
		glTranslatef(0, buildingXLean/5, 0);
		drawAndColorElements();
	} else if (i==0 && j==7) {
		glTranslatef(.5, 0, 0.5);
		buildingRotation += .005*distance;
		if (k==0 || k==2 || k==4) {
			glRotatef(buildingRotation,0,1,0);
		} else {
			glRotatef(-buildingRotation,0,1,0);
		}
		glTranslatef(-.5, 0, -0.5);
		drawAndColorElements();
	} else if (i==2 && j==4) {
		glTranslatef(0.5, 0.5, 0.5);
		buildingXLean -= .005*zPosition/10;
		buildingZLean -= 0.05*xPosition/10;
		glRotatef(buildingXLean,1,0,0);
		glRotatef(buildingXLean,0,1,0);
		glRotatef(buildingXLean,0,0,1);
		glTranslatef(-0.5, -0.5, -0.5);
		drawAndColorElements();	
	} else if (i==3 && j==7) {
		glColor3f(1-xPosition/10, 0, zPosition/10);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices);
		glColor3f(xPosition/10, 0, zPosition/10);
		glDrawElements(GL_LINE_STRIP, 36, GL_UNSIGNED_BYTE, cubeIndices);		
	} else if (i==5 && j==2) {
		glTranslatef(0.5, 0.5, 0.5);
		float bounce = 1 + distance*sin(frameCount/200.0)/50;
		float squish = 1 - distance*sin(frameCount/200.0)/50;
		if (k==0) {
			glScalef(bounce, squish, bounce);
		} else {
			glScalef(squish, bounce, squish);
		}
		glTranslatef(-0.5, -0.5, -0.5);
		drawAndColorElements();	
	} else if (i==7 && j==0) {
		glScalef(1+distance/5, 1+distance/5, 1+distance/5);
		drawAndColorElements();	
	} else if (i==7 && j==7) {
		glTranslatef(.5, -k, 0.5);
		buildingXLean -= .005*zPosition/10;
		buildingZLean -= 0.05*xPosition/10;
		glRotatef(buildingXLean,1,0,0);
		glRotatef(buildingZLean,0,0,1);
		glTranslatef(-.5, k, -0.5);
		drawAndColorElements();	
	}
}

/**
*    Function invoked for drawing using OpenGL
*/
void display()
{
    static int frameCount=0;

	/* #### frame count, might come in handy for animations */
	frameCount++;
	
    /* Clear the window */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* Set the perspective projection */

    /* #### Load/set the model view matrix */
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((GLfloat *)myModelMat);
	
	glTranslatef(4, 0, 26);
	glRotatef(xRotation,1,0,0);
	glRotatef(yRotation,0,1,0);
	glRotatef(zRotation,0,0,1);
	glTranslatef(xPosition-4, 0, zPosition-26);

    /* Enable client */
	glEnableClientState(GL_VERTEX_ARRAY);
	
	/* Draw the world */
	glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
	for(int i = 0; i < 8; i += 1) 
	{
		for(int j = 0; j < 8; j += 1) 
		{

			if(world[i][j] > 0) 
			{
				for(int k = 0; k < world[i][j]; k += 1) 
				{
					glPushMatrix();
					animateBuilding(i,j,k, frameCount);
					glPopMatrix();
				}
				
			}
		}
	}

	/* Draw the floor */
	glVertexPointer(3, GL_FLOAT, 0, floorVertices);
	glColor3f(0.0, 1.0, 0.0);
	glDrawArrays(GL_POLYGON, 0, 4);
	
	/* Disable client */
	glDisableClientState(GL_VERTEX_ARRAY);

    /* Force execution of OpenGL commands */
    glFlush();

    /* Swap buffers for animation */
    glutSwapBuffers();
}


/**
*    Function invoked when window system events are not being received
*/
void idle()
{
    /* Redraw the window */
    glutPostRedisplay();
}


/**
*    #### Function invoked when an event on regular keys occur
*/
void keyboard(unsigned char k, int x, int y)
{
    /* Show which key was pressed */
    //std::cout << "Pressed \"" << k << "\" ASCII: " << (int)k << std::endl;

    if(k == 'a') 
	{
		yRotation -= 5;
	}
	else if(k == 'd') 
	{
		yRotation += 5;
	}
	else if(k == 'w') 
	{
		xRotation += 5;
	}
	else if(k == 's') 
	{
		xRotation -= 5;
	}
	else if(k == 'p') {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fovy, aspect, zNear, zFar);
	}
	else if(k == 'o') {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-4, 4, -1, 5, 0, 20);
	}
    else if (k == 27)
    {
        /* Close application if ESC is pressed */
        exit(0);
    }
}


/**
*	#### Function invoked when an event on special keys occur
*/
void special(int key, int x, int y) 
{
	if(key == GLUT_KEY_UP) 
	{
		zPosition += 0.25;
	}
	else if(key == GLUT_KEY_DOWN) 
	{
		zPosition -= 0.25;
	}
	else if(key == GLUT_KEY_RIGHT) 
	{
		xPosition -= 0.25;
	}
	else if(key == GLUT_KEY_LEFT) 
	{
		xPosition += 0.25;
	}
}


/**
*	Set OpenGL initial state
*/
void init()
{
    /* Set clear color */
    glClearColor(1.0, 1.0, 1.0, 0.0);

    /* Enable the depth buffer */
    glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, zNear, zFar);
}


/**
*	Main function
*/
int main(int argc, char **argv)
{
    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("CS 334 - Assignment 1: Cook it!");

    /* Set OpenGL initial state */
    init();

    /* Callback functions */
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

    /* Start the main GLUT loop */
    /* NOTE: No code runs after this */
    glutMainLoop();
}