/*
 *  Coordinates
 *
 *  Display 2, 3 and 4 dimensional coordinates in 3D.
 *
 *  Key bindings:
 *  1      2D coordinates
 *  2      2D coordinates with fixed Z value
 *  3      3D coordinates
 *  4      4D coordinates
 *  +/-    Increase/decrease z or w
 *  arrows Change view angle
 *  0      Reset view angle
 *  ESC    Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static GLfloat view_rotx = 25.0, view_roty = 50.0, view_rotz = 35.0;
static GLfloat view_posz = 60.0;

 /*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int dim = 2;
int th = 0;
int ph = 0;

int num = 50000;
float lorenzArray[50000][3];


// int percent = 0;
int iter = 0;
int iterInc = 10;
/*
 *  Display the scene
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // background color
   glClearColor(0.1, 0.0, 0.0, 0.1);

	//  Reset previous transforms   
   glPushMatrix();

   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   
   glBegin(GL_LINE_STRIP);
   
   int i = 0;
   
   while (i < iter && i < 50000) {
      if (i < 5000) {
      	glColor3f(1,1,0);
      }
      else if(5000 <i && i < 10000) {
      	glColor3f(1, 0, 0);
      }
      else if(10000 <i && i < 15000) {
      	glColor3f(0, 1, 0);
      }
      else if(15000 <i && i < 20000) {
      	glColor3f(1, 1, 1);
      }
      else if(20000 <i && i < 25000) {
      	glColor3f(.5, .5, .5);
      }
      else if(25000 <i && i < 30000) {
      	glColor3f(1, 1, .5);
      }
      else if(30000 <i && i < 35000) {
      	glColor3f(.5, 0, 0);
      }
      else if(40000 <i && i < 45000) {
      	glColor3f(0, 1, .5);
      }
      else {
      	glColor3f(1, 1, 1);
      }
      glVertex3fv(lorenzArray[i++]); 
   }

   glEnd();
   
   if (iter < 50000) {
      if (iter + iterInc > 50000) {
      	iter = 50000;
      }
      else iter += iterInc;
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d ",th,ph);
   Print("s = %f", s);
   Print(" Number of Points Drawn: %d/%d", i, 50000);
   

   glFlush();
   glutSwapBuffers();

   glPopMatrix();
}


/*
 * Simple program to demonstrate generating coordinates
 * using the Lorenz Attractor
*/
void lorenzGen() {
   int i;
   /*  Coordinates  */
   float x = lorenzArray[0][0] = 1;
   float y = lorenzArray[0][1] = 1;
   float z = lorenzArray[0][2] = 1;
   /*  Time step  */
   float dt = 0.001;

   /*
   *  Integrate 50,000 steps (50 time units with dt = 0.001)
   *  Explicit Euler integration
   */
   
   for (i=0;i<50000 -1;i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;

      // populate the array
      lorenzArray[i+1][0] = x;
      lorenzArray[i+1][1] = y;
      lorenzArray[i+1][2] = z;
   }
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;

  if (ch == 'y') {
  	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	view_posz -= 1;
	gluLookAt(0,0,view_posz,0.0,0.0,0.0,0.0,1.0,0.0);
  }

  if (ch == 'h') {
  	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	view_posz += 1;
	gluLookAt(0,0,view_posz,0.0,0.0,0.0,0.0,1.0,0.0);
  }

   //  Increase w by 1
   if (ch == '+') {
      s += 1;
   }
   //  Decrease w by 1
   else if (ch == '-') {
   	s -= 1;
   }

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


// found glFrustum from https://github.com/mikepacs/lorenz/blob/master/lorenz.c
// glFrustum multplies the current matrix by a perspective matrix
static void reshape(int width, int height) {
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.0, 5.0, -h*2, h*2, 1.0, 300.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -60.0);
}


// incerements iter so the display redraws itself periodically
static void idle(void) {
	iter += iterInc;
	glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{

   // populates the lorenzArray with correct values
   lorenzGen();
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

   //  Request 500 x 500 pixel window
   glutInitWindowSize(700,700);
   //  Create the window
   glutCreateWindow("Vahid Mazdeh");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   
   glutIdleFunc(idle);

   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}