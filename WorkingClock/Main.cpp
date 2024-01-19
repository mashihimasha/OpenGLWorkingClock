#include <glut.h>
#include <cmath>
#include <ctime>
#include <string>
#include <iostream>

float PI = 3.14;

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void drawClockCircle(float radius, int n){
	glBegin(GL_POLYGON);
	for (int i=0; i < n; ++i ) {
		float angle = (2 * PI * i) / n;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
}
void drawCircleMarks(float radius, int n) {
	glBegin(GL_LINES);
	for (int i = 0; i <= n; ++i) {
		float angle = 2 * PI * i / n;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex2f(x, y);
		glVertex2f(0, 0);
	}
	glEnd();
}

void drawCenterDot() {
	glColor3f(1.0, 0.0, 0.0);
	drawClockCircle(0.18, 360);

	glColor3f(1.0, 1.0, 1.0);
	drawClockCircle(0.09, 360);
}

void drawClockHand(float radius,int angle) {
	glBegin(GL_LINES);
	float x = radius * cos(PI/2);
	float y = radius * sin(PI/2);
	glVertex2f(x, y);
	glVertex2f(0, 0);
	glEnd();
}

void drawNumbers(float radius) {
	long double angle = PI/2-PI/6;
	int count = 0;
	int charIndex=0;

	glColor3f(0.0, 0.0, 0.0); // Set the number color to white

	for (int i = 1; i <= 12; i++) {
		std::string number = std::to_string(i);
		double x = radius * cos(angle);
		double y = radius * sin(angle);
		x -= 0.2;
		y -= 0.2;

		if (i < 10) {
			for (char c : number) {

				glRasterPos2f(x, y);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
			}
		}
		else{
			for (int j = 0; j < 1; j++) {
				for (char c : number) {
					if (charIndex == 0) {
						glRasterPos2f(x-0.1, y);
					}
					else{
						glRasterPos2f(x+0.25, y);
					}
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
					charIndex++;
				}
				charIndex = 0;
			}
		}
		angle -= PI / 6;
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//outer black circle
	glColor3f(0.0, 0.0, 0.0);
	drawClockCircle(5.8,64);


	//inner white
	glColor3f(1.0, 1.0, 1.0);
	drawClockCircle(5.3,64);

	//black second marks
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	drawCircleMarks(5.2, 60);

	//minute marks
	glLineWidth(2.5);
	glColor3f(0.0, 0.0, 0.0);
	drawCircleMarks(5.2, 12);

	//inner white
	glColor3f(1.0, 1.0, 1.0);
	drawClockCircle(4.8, 64);

	drawNumbers(4.4);

	time_t currentTime = time(0);
	struct tm now;
	localtime_s(&now, &currentTime);
	int current_seconds = now.tm_sec;
	int current_minutes = now.tm_min;
	int current_hours = now.tm_hour;


	//hour hand
	glLineWidth(4.5);
	glPushMatrix();
	glRotatef(-current_hours*30 - current_minutes*0.1, 0, 0, 1);
	glColor3f(0.0, 0.0, 0.0);
	drawClockHand(4.0,1);
	glPopMatrix();

	//minute hand
	glLineWidth(4.5);
	glPushMatrix();
	glRotatef(-current_minutes*6 - current_seconds*0.1, 0, 0, 1);
	glColor3f(0.0, 0.0, 0.0);
	drawClockHand(4.8, 1);
	glPopMatrix();

	//second hand
	glLineWidth(2.0);
	glPushMatrix();
	glRotatef(-current_seconds * 6, 0, 0, 1);
	glColor3f(1.0, 0.0, 0.0);
	drawClockHand(4.8, 1);
	glPopMatrix();

	drawCenterDot();

	glutSwapBuffers();

}

void reshape(int w, int h) {
	glViewport(0.0, 0.0, w, h);
	if (h == 0) {
		h = 1;
	}
	float aspect_ratio = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w >= h) {
		gluOrtho2D(-10.0 * aspect_ratio, 10.0 * aspect_ratio, -10.0, 10.0); // Define your coordinate system
		//left right top bottom
	}
	else {
		gluOrtho2D(-10.0, 10.0, -10.0 / aspect_ratio, 10.0 / aspect_ratio);
	}
	glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char** argv) {
	glutInit( &argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(720, 480);
	glutCreateWindow("Working Clock");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);

	init();

	glutMainLoop();

	return 0;
}