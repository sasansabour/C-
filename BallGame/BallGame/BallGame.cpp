
#include "stdafx.h"
#include <stdlib.h>
#include <glut.h>
#include <math.h>

//#include <process.h>

int red,blue,green;
int speed = 2;
int k=0;
int d=0;
int numdeletedb=0,jayze[3];
int b=0,x1l=320,x2l=420;

class GLintPoint
{
public:
	GLint x,y;
};
static int num = 0;
static GLintPoint Brick[3][40];
int sh =20;
float w1=0,z1=-1;
float w=-3,z=-1;


class Point2
{
public:
Point2() {x = y = 0.0f;} // constructor1
Point2(float xx, float yy) {x = xx; y = yy;} // constructor2
void set(float xx, float yy) {x = xx; y = yy;}
float getX() {return x;}
float getY() {return y;}
void draw(void)
{ 
	glBegin(GL_POINTS); // draw this point
    glVertex2f((GLfloat)x, (GLfloat)y);
    glEnd();
}
private:
float x, y;
};

class IntRect
{
public:
IntRect() {l = 0; r = 100; b = 0; t = 100;}// constructors
IntRect(int left, int right, int bottom, int top)
{l = left; r = right; b = bottom; t = top;}
void set(int left, int right, int bottom, int top)
{l = left; r = right; b = bottom; t = top;}
void draw(void){glRecti(l,b,r,t);} // draw this rectangle using OpenGL
private:
int l, r, b, t;
};

class RealRect
{
	public:
RealRect() {l = 0; r = 100; b = 0; t = 100;}// constructors
RealRect(float left, float right, float bottom, float top)
{l = left; r = right; b = bottom; t = top;}
void set(float left, float right, float bottom, float top)
{l = left; r = right; b = bottom; t = top;}
void draw(void){glRectf(l,b,r,t);} // draw this rectangle using OpenGL
private:
float l, r, b, t;

};


class Canvas {
public:
Canvas(int width, int height, char* windowTitle); // constructor
void setWindow(float l, float r, float b, float t);
void setViewport(int l, int r, int b, int t){glViewport(l, b,r-l,t-b);}
IntRect getViewport(void); // divulge the viewport data
RealRect getWindow(void); // divulge the window data
float getWindowAspectRatio(void);
void clearScreen(){glClear(GL_COLOR_BUFFER_BIT);}
void setBackgroundColor(float r, float g, float b){glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0,720,0,650);
	glClearColor(1,1,1,0);
	}
void setColor(float r, float g, float b){glColor3f(r,g,b);}
void lineTo(float x, float y);
void lineTo(Point2 p);
void lineRel(float dx, float dy);
void moveTo(float x, float y);
void moveTo(Point2 p);
void moveRel(float dx, float dy);
void draw(float rad);
void turnTo(float angle) {CD = angle;}
void turn(float angle){CD += angle;}
void forward(float dist, int isVisible){
	const float RadPerDeg = 0.017453393; //radians per degree
	cx = cx + dist * cos(RadPerDeg*CD);
 cy = cy + dist * sin(RadPerDeg*CD);
//p.set(x,y);
if(isVisible)
lineTo(cx, cy);
else
moveTo(cx, cy);
}

//others later
float cx;
float cy;
private:
Point2 CP; // current position in the world
IntRect viewport; // the current window
RealRect window; // the current viewport
float CD;
//others later
};

Canvas cvs(720, 640, "Ball");

Canvas:: Canvas(int width, int height, char* windowTitle)
{
char* argv[1]; // dummy argument list for glutInit()
char dummyString[8];
argv[0] = dummyString; // hook up the pointer
int argc = 1; // to satisfy glutInit()
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(width, height);
glutInitWindowPosition(20, 20);
glutCreateWindow(windowTitle); // open the screen window
setWindow(0, (float)width, 0, (float)height); //default world window
setViewport(0, width, 0, height); // default viewport
CP.set(0.0f, 0.0f); // initialize the CP to (0, 0)
}
void Canvas:: moveTo(float x, float y)
{
CP.set(x, y);
}
//<<<<<<<<<<<<<<<<<<<<<<< lineTo >>>>>>>>>>>>>>>>>>>>>>>
void Canvas:: lineTo(float x, float y)
{
glBegin(GL_LINES);
glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
glVertex2f((GLfloat)x, (GLfloat)y); // draw the line
glEnd();
CP.set(x, y); // update the CP
glFlush();
}
//<<<<<<<<<<<<<<<<<<<<<<<< set Window >>>>>>>>>>>>>>>>>>>>
void Canvas:: setWindow(float l, float r, float b, float t)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t);
window.set(l, r, b, t);
}

void Canvas :: moveRel(float dx, float dy)
{
	CP.set(CP.getX() + dx, CP.getY() + dy);
}
void Canvas :: lineRel(float dx, float dy)
{
	float x = CP.getX() + dx, y = CP.getY() + dy;
lineTo(x, y);
CP.set(x, y);
}

void arrow(float f, float h, float t, float w)
{ // assumes global Canvas object: cvs
cvs.lineRel(-w - t / 2, -f); // down the left side
cvs.lineRel(w, 0);
cvs.lineRel(0, -h);
cvs.lineRel(t, 0); // across
cvs.lineRel(0, h); // back up
cvs.lineRel(w, 0);
cvs.lineRel(-w - t / 2, f);
}
int L=1;
void ngon(int n, float cx, float cy, float radius, float rotAngle)
{ // assumes global Canvas object, cvs
if(n < 3) return; // bad number of sides
double angle = rotAngle * 3.14159265 / 180; // initial angle
double angleInc = 2 * 3.14159265 /n; //angle increment
cvs. moveTo(radius + cx, cy);
for(int k = 0; k < n; k++) // repeat n times
{
angle += angleInc;
cvs.lineTo(radius * cos(angle) + cx, radius * sin(angle) + cy);
}
}

Point2 p(9,0);
void drawCircle(Point2 center, float radius)
{
const int numVerts = 40; // use larger for a better circle
ngon(numVerts, center.getX(), center.getY(), radius,0);
}
void Canvas::draw(float rad)
{
	ngon(40,cx,cy,rad,0);
}
//////////////////////////////////////////////////////////////////
void Write(int x, int y, char *string, bool little = false){
	glRasterPos2i(x, y); 
	if(little)
		while(*string) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*string++);
	else
		for(int i=0; string[i]; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
	glFlush();
}

//////////////////////////////////////////
void drawBrick()
{
	for(int i=0;i<40;i++)
	{
		if(i%2==0)
			cvs.setColor(.5,.8,.2);
		if(Brick[2][i].x==1 && Brick[2][i].y==1)	
			glRecti(Brick[0][i].x,Brick[0][i].y,Brick[1][i].x,Brick[1][i].y);
		cvs.setColor(0,0,0);
	}
}
////////////////////////////////////////////
void line()
{
	if(x1l<=0)
	{
		x1l=0;x2l=100;
		glBegin(GL_LINES);
		glVertex2i(x1l,10);
		glVertex2i(x2l,10);;
		glEnd();
		glFlush();
	}
	else if(x2l>=720)
	{
		x2l=720;x1l=620;
		glBegin(GL_LINES);
		glVertex2i(x1l,10);
		glVertex2i(x2l,10);;
		glEnd();
		glFlush();
	}
	else
	{
		glBegin(GL_LINES);
		glVertex2i(x1l,10);
		glVertex2i(x2l,10);;
		glEnd();
		glFlush();
	}
}
//////////////////////////////////////////////////////////////
int RangedRand( int range_min, int range_max)
{
   return (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
}
///////////////////////////////////////////////////////////
static Point2 noghtej;

void circle(float &x,float &y,Point2 &s)
{
	float nx,ny,rx,ry;
	int t=-1,l=0;
	s.set(s.getX()+x,s.getY()+y);
	float right=s.getX()+sh,left=s.getX()-sh,top=s.getY()+sh,bot=s.getY()-sh;
	if(top>=640||bot<=0||right>=720||left<=0)
	{
		if(left<=0&&bot<=0)
		{
			rx=-1*x;
			ry=-1*y;
			l=1;
		}
		else if(left<=0&&top>=640)
		{
			rx=-1*x;
			ry=-1*y;
			l=1;
		}
		else if(right>=720&&top>=640)
		{
			rx=-1*x;
			ry=-1*y;
			l=1;
		}
		else if(right>=720&&bot<=0)
		{
			rx=-1*x;
			ry=-1*y;
			l=1;
		}
		else if(top>=640)
		{
			nx=0;
			ny=-1;
			t=1;
		}
		else if(right>=720)
		{
			nx=-1;
			ny=0;
			t=1;
		}
		else if(bot<=-50)
		{
			exit(0);
		}
		else if(left<=0)
		{
			nx=1;
			ny=0;
			t=1;
		}
	}
	else if(bot<=10 && s.getX()>x1l && s.getX()<x2l)
	{
		nx=0;
		ny=1;
		t=1;
	}
	else if((noghtej.getY()-15)<=10 && noghtej.getX()>x1l && noghtej.getX()<x2l)
	{
		x2l+=50;
		x1l-=50;
	}
	else
	{
		for(int i=0;i<40;i++)
		{
			if(Brick[2][i].x==1 && Brick[2][i].y==1)
			{
				if(top>=Brick[1][i].y && s.getX()>=Brick[0][i].x && s.getX()<=Brick[1][i].x)
				{
					nx=0;
					ny=-1;
					t=1;
					w1=0,z1=-1;
					Brick[2][i].x=0;
					Brick[2][i].y=0;
					if(jayze[0]==i || jayze[1]==i || jayze[2]==i)
					{
						noghtej.set(Brick[0][i].x+((Brick[1][i].x-Brick[0][i].x)/2),Brick[1][i].y);
						b=1;
						numdeletedb++;
						if(abs(w)<50 && numdeletedb==1)
						{
							z*=1.5;	
							w*=1.5;
						}
					}
					break;
				}
			}
		}
	}
	if(t==1)
	{
		rx=x-2*((x*nx)+(y*ny))*(nx);
		ry=y-2*((x*nx)+(y*ny))*(ny);
		l=1;
		t=0;
	}
	if(l==1)
	{
	int red,green,blue;
	{
		red=green=blue=0;
	}
	cvs.setColor(red,green,blue);
	x=rx;
	y=ry;

	}
	l=0;
	t=0;
	if(b==1)
	{
		noghtej.set(noghtej.getX()+w1,noghtej.getY()+z1);
		z1-=.2;
		drawCircle(noghtej,sh-5);
	}
	
	cvs.clearScreen();
	drawBrick();
	line();
	drawCircle(s,sh);
}
static Point2 noghte(RangedRand(200,500),RangedRand(200,500));

void myKeyboard(unsigned char theKey,int x,int y)
{
	switch(theKey)
	{
	case 's':if(abs(w)>2){w/=2;  z/=2;}	break;
	case 'w':if(abs(w)<50){z*=2;	w*=2;}  break;
	case 'd':{x1l+=40;x2l+=40;circle(w,z,noghte);break;}
	case 'a':{x1l-=40;x2l-=40;circle(w,z,noghte);break;}
			
	}
}
void motion(int)
{
	glutKeyboardFunc(myKeyboard);
	circle(w,z,noghte);
	for(long i=0;i<3000000;i++)
		long pu=i*i;
	glutTimerFunc(1,motion,1);
}
void newgame()
{
	cvs.clearScreen();
	if(d==0)
	{
		w*=speed;
		z*=speed;
		d=1;
	}
	glutTimerFunc(1,motion,1);
	cvs.setColor(0,0,0);
}

void mymouse(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		if(x<=440 && x>=250 && (640-y)<=500 && (640-y)>=440)
		{
			newgame();
		}
		else if(x<=450 && x>=250 && (640-y)<=280 && (640-y)>=220)
		{
			exit(0);
		}
	}
}

void drawmenu()
{

	cvs.clearScreen();
	cvs.setColor(0,0,.2);
	glRecti(250,500,450,440);
	cvs.setColor(0,0,.2);
	glRecti(250,280,450,220);
	cvs.setColor(0,0,0);
	Write(300,550,"Ball Game");
	cvs.setColor(1,1,1);
	Write(300,465,"New Game");
	Write(325,245,"Exit");
}
void display(void)
{
	cvs.setWindow(0, 720, 0, 640);
	cvs.setViewport(0, 720, 0, 640);
	glutMouseFunc(mymouse);
	drawmenu();
}
///////////////////////////////////////////////
void Init(){
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,640,0,720);
	glClearColor(1,1,1,1);	
	glLineWidth(2);
}
///////////////////////////////////////////////////////
int _tmain(int argc, char** argv)
{
int x=0,y=640,numb=0;
jayze[0]=RangedRand(0,39);
jayze[1]=RangedRand(0,39);
jayze[2]=RangedRand(0,39);
for(int i=0;i<5;i++)
{
	x+=20;
	for(int j=0;j<8;j++)
	{
		Brick[0][numb].x=x;
		Brick[0][numb].y=y-10;
		Brick[1][numb].x=x+80;
		Brick[1][numb].y=y-40;
		Brick[2][numb].x=1;
		Brick[2][numb].y=1;
		x+=85;
		numb++;
	}
	x=0;
	y-=50;
}

cvs.setBackgroundColor(1.0, 1.0, 1.0); // background is white
cvs.setColor(1.0, 1.0, 1.0); // set drawing color
Init();

glutMouseFunc(mymouse);
glutKeyboardFunc(myKeyboard);
glutDisplayFunc(display);
glutMainLoop();
return 0;
	
}

