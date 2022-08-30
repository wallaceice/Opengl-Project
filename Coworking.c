#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

#define TEXTURA_DO_PLANO "mesa.rgb"
float initialX = -1.90f;
float initialY = 0.20f;
float initialZ = 2.00f;

float finalX = 2.50f;
float finalZ = -1.00f;

GLuint  textura_plano;
static int rot = 0;

static float lightY = 3;
static int move = 0;
static GLdouble zoom = 0;

//initial position cleaner
float moveX = -1.9f;
float moveY = 0.2f;
float moveZ = 2.0f;
static int direction = 1;

GLsizei widht = 500, height = 500;


GLfloat chao_difusa[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat chao_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat chao_brilho[] = { 50.0 };

GLfloat mat_2_difusa[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat mat_2_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_2_brilho[] = { 50.0 };

GLfloat mat_3_difusa[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat mat_3_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_3_brilho[] = { 50.0 };

GLfloat parede_difusa[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat parede_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat parede_brilho[] = { 50.0 };

GLfloat decoration_difusa[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat decoration_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat decoration_brilho[] = { 50.0 };

GLfloat luminaria_difusa[] = { 1.0,1.0,0.0, 1.0 };
GLfloat luminaria_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat luminaria_brilho[] = { 50.0 };

GLfloat mesa_difusa[] = { 0.0, 0.0,0.0, 1.0 };
GLfloat mesa_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mesa_brilho[] = { 50.0 };

GLfloat quadro_difusa[] = { 0.0, 1.0,1.0, 1.0 };
GLfloat quadro_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat quadro_brilho[] = { 50.0 };

GLfloat posicao_luz0[] = { 0.0, 4.0, 0.0, 1.0 };
GLfloat cor_luz0[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat cor_luz0_amb[] = { 0.3, 0.3, 0.3, 1.0 };

GLfloat posicao_luz[] = { 1.0, 4.0, 0.0, 1.0 };
GLfloat cor_luz[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat cor_luz_amb[] = { 0.3, 0.3, 0.3, 1.0 };

void carregar_texturas(void) {
	IMAGE* img;
	GLenum gluerr;

	/* textura do plano */
	glGenTextures(1, &textura_plano);
	glBindTexture(GL_TEXTURE_2D, textura_plano);

	if (!(img = ImageLoad(TEXTURA_DO_PLANO))) {
		fprintf(stderr, "Error reading a texture.\n");
		exit(-1);
	}

	gluerr = gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		img->sizeX, img->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE,
		(GLvoid*)(img->data));
	if (gluerr) {
		fprintf(stderr, "GLULib%s\n", gluErrorString(gluerr));
		exit(-1);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

void moving() {


	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mesa_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mesa_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mesa_brilho);

	glPushMatrix();
	glTranslatef(moveX, moveY, moveZ); 
	glScalef(0.6, 0.1, 0.6);
	glutSolidCube(1.0);
	glPopMatrix();


	glBegin(GL_LINES);
	glVertex3f(moveX, moveY, moveZ);
	glVertex3f(moveX, moveY*3, moveZ);
	glEnd();


	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, decoration_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, decoration_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, decoration_brilho);

	//lampada
	glPushMatrix();
	glTranslatef(moveX, moveY*3, moveZ);
	glutSolidSphere(0.05, 5,5);
	glPopMatrix();
}

void init(void) {

	carregar_texturas();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
	glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
	glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

}


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, textura_plano);
	glPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (GLfloat)height / (GLfloat)height, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, -3.0, -10.0+zoom);
	glRotatef((GLfloat)rot, 0.0, 1.0, 0.0);




	/* propriedades do chao */
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chao_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chao_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, chao_brilho);

	//lampada
	glPushMatrix();
	glTranslatef(0, 3.1, 0);
	glutSolidSphere(0.3, 10, 10);
	glPopMatrix();

	/* chao*/
	glPushMatrix();
	glScalef(6.0, 0.15, 6.0);
	glutSolidCube(1.0);
	glPopMatrix();

	/* propriedades da parede */

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, parede_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, parede_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, parede_brilho);

	/* parede esquerda */
	glPushMatrix();
	glTranslatef(-2.9, 2.1, 0.0);
	glScalef(0.15, 4.0, 6.0);
	glutSolidCube(1.0);
	glPopMatrix();


	/*  parede fundo */
	glPushMatrix();
	glTranslatef(0.0, 2.1, -2.9);
	glScalef(6.0, 4.0, 0.15);
	glutSolidCube(1.0);
	glPopMatrix();


	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, quadro_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, quadro_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, quadro_brilho);
	/*Quadro parede*/

	glPushMatrix();
	glTranslatef(-2.7, 1.8, 0.9);
	glScalef(0.1, 1.0, 1.5);
	glutSolidCube(1.0);
	glPopMatrix();


	////acentos
	glPushMatrix();
	glTranslatef(-2, 0.4, -2.3);
	glScalef(1, 0.8, 1.2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.4, -2.3);
	glScalef(1, 0.8, 1.2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0.4, -2.3);
	glScalef(1, 0.8, 1.2);
	glutSolidCube(1.0);
	glPopMatrix();
	/////fimm acentos ////

	//glEnable(GL_COLOR_MATERIAL);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luminaria_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, luminaria_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, decoration_brilho);


	glPushMatrix();
	glTranslatef(0, 3, 0);
	glRotatef((270 % 360), 1.0, 0.0, 0.0);
	glutSolidCone(0.6, 1, 10, 10);
	glPopMatrix();
	////cone////////

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, decoration_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, decoration_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, decoration_brilho);



		////Decoration//////
	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
		  // first
	glVertex3f(1.0, 1.6, 0.0f);
	glVertex3f(0.5, 0.9, 0.6);
	glVertex3f(1.5, 0.9, 0.6);

	// middle
	glVertex3f(1.0, 1.6, 0.0f);
	glVertex3f(0.5, 0.9, 0.6);
	glVertex3f(0.5, 0.9, -0.5);
	//middle op
	glVertex3f(1.0, 1.6, 0.0f);
	glVertex3f(1.5, 0.9, 0.6);
	glVertex3f(1.5, 0.9, -0.5);

	// last
	glVertex3f(1.0, 1.6, 0.0f);
	glVertex3f(0.5, 0.9, -0.5);
	glVertex3f(1.5, 0.9, -0.5);

	glEnd();
	/////Decoration_end//////



	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mesa_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mesa_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mesa_brilho);

	/*pé dir frente*/
	glPushMatrix();
	glTranslatef(0.2, 0.1, 0.9);
	glScalef(0.1, 0.6, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();



	/*pé esq frente*/
	glPushMatrix();
	glTranslatef(0.2, 0.1, 1.3);
	glScalef(0.1, 0.6, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	/*pé dir tras*/
	glPushMatrix();
	glTranslatef(-0.2, 0.75, 0.9);
	glScalef(0.1, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	/*pé esq tras*/

	glPushMatrix();
	glTranslatef(-0.2, 0.75, 1.3);
	glScalef(0.1, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	/*acento*/
	glPushMatrix();
	glTranslatef(0.0, 0.4, 1.1);
	glScalef(0.55, 0.1, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();

	/*encosto*/

	glPushMatrix();
	glTranslatef(-0.2, 1.1, 1.1);
	glScalef(0.1, 0.55, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();

	//////Fim cadeira
	/* mesa */
	/*pé da mesa*/

	/*pé da mesa direita frente*/
	glPushMatrix();
	glTranslatef(1.6, 0.3, 2);
	glScalef(0.1, 0.8, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	/*pé da mesa esq frente*/

	glPushMatrix();
	glTranslatef(0.2, 0.3, 2);
	glScalef(0.1, 0.8, 0.1);
	glutSolidCube(1.0);

	glPopMatrix();

	/*pé da mesa direita tras*/

	glPushMatrix();
	glTranslatef(1.5, 0.3, -0.0);
	glScalef(0.1, 0.7, 0.1);
	glutSolidCube(1.0);

	glPopMatrix();

	/*pé da mesa esq tras*/

	
	glPushMatrix();
	glTranslatef(0.2, 0.3, -0.0);
	glScalef(0.1, 0.75, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_3_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_3_especular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_3_brilho);
	/*topo da mesa*/
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0.9, 0.8, 0.9);
	glScalef(2.0, 0.1, 3.0);
	glutSolidCube(1.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	/*Fim mesa*/


		//moving
	moving();

	/*/*/
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h) {

	if (h == 0) h = 1;
  
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0, -2.0, -11.0);

}


void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'r':
		rot = (rot + 5) % 360;
		glutPostRedisplay();
		break;
	case 'e':
		rot = (rot - 5) % 360;
		glutPostRedisplay();
		break;
	case 'z':
		zoom = zoom ++;
		if (zoom > 70)
			zoom = 0;
		glutPostRedisplay();
		break;
	case 'x':
		zoom = zoom --;
		if (zoom < -20)
			zoom = 0;
		glutPostRedisplay();
		break;
	case 27:                                         // tecla Esc (encerra o programa)
		exit(0);
		break;
	}
}


void changemove(int value) {

	if (direction)
	{
		if (moveX <= finalX && moveZ > finalZ)
		{
			moveZ -= 0.01f;
		}else
		if (moveX <= finalX && moveZ <= finalZ)
		{
				moveX += 0.01f;
		}else
		if(moveZ <= initialZ) {
				moveZ += 0.01f;
		}else

		if (moveX >= finalX && moveZ >= initialZ) {
			direction = 0;
		}


	}
	if (!direction)
	{
		if (moveX >= finalX && moveZ >= finalZ)
		{
			moveZ -= 0.01f;
		}
		else
			if (moveX >= initialX && moveZ <= finalZ)
			{
				moveX -= 0.01f;
			}
			else
				if (moveZ <= initialZ) {
					moveZ += 0.01f;
				}
				else

					if (moveX <= initialX && moveZ >= initialZ) {
						direction = 1;
					}
	}

	glutPostRedisplay();
	glutTimerFunc(10, changemove, 0);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, changemove, 0);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

