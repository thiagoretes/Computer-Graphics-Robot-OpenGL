/*
* GL01Hello.cpp: Test OpenGL C/C++ Setup
*/
#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <cstdio>
#include <cmath>


#define PI 3.14159265
#define TIMER_FRAMES 16 //Isso me dá aproximadamente 60 FPS
#define VELOCIDADE_EIXOS 45 //Os eixos movimentarão 45 graus por segundo

#define ANGULO_GARRA_ABERTA 120
#define ANGULO_GARRA_FECHADA 30

class Point3D
{
private:
	float x;
	float y;
	float z;
public:
	Point3D(float x, float y, float z);
	Point3D();
	void setCoords(float x, float y, float z);
	float getX();
	float getY();
	float getZ();
	Point3D getVector(Point3D a, Point3D b)
	{
		return Point3D(a.x - b.x, a.y - b.y, a.z - b.z);
	};
	void crossProduct(Point3D a, Point3D b)
	{
		x = a.y*b.z - a.z*b.y;
		y = a.z*b.x - a.x*b.z;
		z = a.x*b.y - a.y*b.x;
	}
	
};
void Point3D::setCoords(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
float Point3D::getX()
{
	return x;
}
float Point3D::getY()
{
	return y;
}
float Point3D::getZ()
{
	return z;
}

Point3D::Point3D(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Point3D::Point3D()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

class Retangulo
{
private:
	Point3D vertice[4];
public:
	Retangulo(Point3D vertices[]);
	Retangulo();
	void setVertice(int num, Point3D value);
	Point3D getVertice(int num);
	Retangulo* divideBy(int num)
	{
		if (num < 2) return NULL;
		float baseX = vertice[1].getX() - vertice[0].getX();
		
		float baseY = vertice[1].getY() - vertice[0].getY();
		
		float baseZ = vertice[1].getZ() - vertice[0].getZ();

		float heightX = vertice[0].getX() - vertice[3].getX();

		float heightY = vertice[0].getY() - vertice[3].getY();

		float heightZ = vertice[0].getZ() - vertice[3].getZ();
		
		Retangulo* arr = new Retangulo[num*num]();
		float base[3] = { baseX / num, baseY / num, baseZ / num };
		float height[3] = { heightX / num, heightY / num, heightZ / num };

		for (int i = 0; i < num*num; i++)//muda coluna
		{
			
			
			arr[i].setVertice(0, Point3D(vertice[0].getX()+((i%num)*base[0])- ((i/num)*height[0]), vertice[0].getY()+((i%num)*base[1])- ((i/num)*height[1]), vertice[0].getZ()+((i%num)*base[2])- ((i/num)*height[2])));
			arr[i].setVertice(1, Point3D(vertice[0].getX() + ((1 + i%num)*base[0]) - ((i/num)*height[0]), vertice[0].getY() + ((1 + i%num)*base[1]) - ((i/num)*height[1]), vertice[0].getZ() + ((i%num + 1)*base[2])- ((i/num)*height[2])));
			arr[i].setVertice(2, Point3D(vertice[0].getX() + ((1 + i%num)*base[0]) - ((1+i/num)*height[0]), vertice[0].getY() + ((1 + i%num)*base[1]) - ((1 + i/num)*height[1]), vertice[0].getZ() + ((i%num + 1)*base[2]) - ((1 + i/num)*height[2])));
			arr[i].setVertice(3, Point3D(vertice[0].getX() + ((i%num)*base[0]) - ((1 + i/num)*height[0]), vertice[0].getY() + ((i%num)*base[1]) - ((1 + i/num)*height[1]), vertice[0].getZ() + ((i%num)*base[2]) - ((1 + i/num)*height[2])));
			
		}
		return arr;


		
	}
};

Retangulo::Retangulo(Point3D vertices[])
{
	for (int i = 0; i < 4; i++)
		this->vertice[i] = vertices[i];
}

Retangulo::Retangulo()
{
	for (int i = 0; i < 4; i++) {
		this->vertice[i] = Point3D();
	}
}
void Retangulo::setVertice(int num, Point3D value)
{
	if (num > 3 || num < 0) return;
	this->vertice[num] = value;
}
Point3D Retangulo::getVertice(int num)
{
	if (num > 3 || num < 0) return this->vertice[0];
	return this->vertice[num];
}

class Cuboid
{
private:
	Retangulo face[6];
	
public:
	Point3D pos;
	float mat[16];
	float inverse_mat[16];
	Cuboid()
	{
		
		for (int i = 0; i < 6; i++)
		{
			face[i] = Retangulo();
		}
	}
	Cuboid(Retangulo value[])
	{
		for (int i = 0; i < 6; i++)
		{
			face[i] = value[i];
		}
	}
	Cuboid(float base, float width, float height)
	{
		face[0].setVertice(0, Point3D(-base / 2, height / 2, -width / 2)); //Manter a ordem sentido horario
		face[0].setVertice(1, Point3D(base / 2, height / 2, -width / 2));
		face[0].setVertice(2, Point3D(base / 2, -height / 2, -width / 2));
		face[0].setVertice(3, Point3D(-base / 2, -height / 2, -width / 2));

		
		face[1].setVertice(0, Point3D(base / 2, height / 2, -width / 2));
		face[1].setVertice(1, Point3D(base / 2, height / 2, width / 2)); 
		face[1].setVertice(2, Point3D(base / 2, -height / 2, width / 2));
		face[1].setVertice(3, Point3D(base / 2, -height / 2, -width / 2));

		face[2].setVertice(1, Point3D(-base / 2, height / 2, width / 2)); //Manter a ordem sentido horario
		face[2].setVertice(0, Point3D(base / 2, height / 2, width / 2));
		face[2].setVertice(3, Point3D(base / 2, -height / 2, width / 2));
		face[2].setVertice(2, Point3D(-base / 2, -height / 2, width / 2));

		face[3].setVertice(0, Point3D(-base / 2, height / 2, -width / 2));
		face[3].setVertice(1, Point3D(-base / 2, height / 2, width / 2));
		face[3].setVertice(2, Point3D(-base / 2, -height / 2, width / 2));
		face[3].setVertice(3, Point3D(-base / 2, -height / 2, -width / 2));

		face[4].setVertice(0, Point3D(-base / 2, height / 2, width / 2));
		face[4].setVertice(1, Point3D(base / 2, height / 2, width / 2));
		face[4].setVertice(2, Point3D(base / 2, height / 2, -width / 2));
		face[4].setVertice(3, Point3D(-base / 2, height / 2, -width / 2));

		face[5].setVertice(0, Point3D(-base / 2, -height / 2, -width / 2));
		face[5].setVertice(1, Point3D(base / 2, -height / 2, -width / 2));
		face[5].setVertice(2, Point3D(base / 2, -height / 2, width / 2));
		face[5].setVertice(3, Point3D(-base / 2, -height / 2, width / 2));

	}
	Retangulo getFace(int num)
	{
		//if (num > 5 || num < 0) return face[0];
		return face[num];
	}
	void setFace(int num, Retangulo ret)
	{
		//if (num > 5 || num < 0) return;
		face[num] = ret;
	}
};

class Sphere
{
private:
	float radius;
public:
	Point3D pos;
	float mat[16];
	Sphere()
	{
		radius = 1.0f;
	}
	Sphere(float value)
	{
		if (value < 0) value = -value;
		radius = value;
	}
	float getRadius()
	{
		return radius;
	}
	void setRadius(float value)
	{
		if (value < 0) value = -value;
		radius = value;
	}
};
Sphere bola(0.4f),eixo1(0.5f),eixo2(0.5f),eixo_garra(0.5f), preview_bola(0.4f);




Cuboid baseCube(2.0f,2.0f,2.0f), haste1(0.25f, 0.25f, 3.0f), haste2(3.0f, 0.25f, 0.25f), garra(0.1f, 0.1f, 0.5f);


class CamData
{
public:
	float mat[16];
	float inverse_mat[16];
} camData;
void hRotationEixo1();

/*class Plane
{
public:
	Point3D vector[2];
	Point3D normal;
	Point3D point;
	Plane(Point3D p1, Point3D p2, Point3D p3)
	{
		float mat[9] = { 1,1,1,p1.getX(),p1.getY(),p1.getZ(),p2.getX(),p2.getY(),p2.getZ() };
		vector[0] = p1.getVector(p1, p2);
		vector[1] = p1.getVector(p1, p3);
		point = p1;
		this->normal.setCoords((mat[0] * mat[4] * mat[8]) - (mat[0] * mat[5] * mat[7]), (mat[1] * mat[5] * mat[6]) - (mat[1] * mat[3] * mat[8]), (mat[2] * mat[3] * mat[7]) - (mat[2] * mat[4] * mat[6]));

	}
	Plane()
	{
		vector[0] = Point3D();
		vector[1] = Point3D();
		normal = Point3D();
		point = Point3D();
	}



};
*/
bool gluInvertMatrix(const float m[16], float invOut[16])
{
	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}
void npLocalToWorld(Point3D *world, const float *inverseCam, const float *modelView)
{
	//x = matrix[12] or [3][0]
	world->setCoords(inverseCam[0] * modelView[12] + inverseCam[4] * modelView[13] +
		inverseCam[8] * modelView[14] + inverseCam[12] * modelView[15], inverseCam[1] * modelView[12] + inverseCam[5] * modelView[13] +
		inverseCam[9] * modelView[14] + inverseCam[13] * modelView[15], inverseCam[2] * modelView[12] + inverseCam[6] * modelView[13] +
		inverseCam[10] * modelView[14] + inverseCam[14] * modelView[15]);
	//y = matrix[13] or [3][1]
	
	//z = matrix[14] or [3][2]
	
}

float angulo_garra = 30;
float next_angulo_garra = 30;
float angulo_garra_base = 120;
float next_angulo_garra_base = 120;

float angulo_X = 45;
float angulo_Y = 45;

float angulo_eixo1_vertical = 0;
float angulo_eixo2_vertical = 0;
float next_angulo_eixo1_vertical = 0;
float next_angulo_eixo2_vertical = 0;

float angulo_eixo1_horizontal = 0;
float next_angulo_eixo1_horizontal = 0;
Point3D normal_eixo1 = Point3D();


BOOL animGarra = false;
BOOL grabbingBall = false;
BOOL ballGrabbed = false;
BOOL previewing_bola = false;
BOOL throwingBall = false;

int startTime; //Para gerenciar o tempo da animação
int prevTime; //Tempo em que a ultima animação foi executada



//FIM Classes
void drawCuboid(Cuboid cube)
{
	
	
	glBegin(GL_QUADS);                      // Draw A Quad
	
	glColor3f(0, 0, 1.0f); //Blue
	
	for (int i = 0; i < 6; i++)
	{
		Retangulo face = cube.getFace(i);
		Retangulo* faces = face.divideBy(8);
		for(int k = 0; k < 64; k++)
			for (int j = 0; j < 4; j++) {
				Point3D vertice = faces[k].getVertice(j);
				switch (i)
				{
				case 0:
					glNormal3f(0, 0, -1);
					break;
				case 1:
					glNormal3f(-1, 0, 0);
					break;
				case 2:
					glNormal3f(0, 0, 1);
					break;
				case 3:
					glNormal3f(1, 0, 0);
					break;
				case 4:
					glNormal3f(0, -1, 0);
					break;
				case 5:
					glNormal3f(0, 1, 0);
					break;
				}
				glVertex3f(vertice.getX(), vertice.getY(), vertice.getZ());
				
			}
		delete[] faces;
	}
	glEnd();                            // Done Drawing The Quad
}
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 360; //# of triangles used to draw circle

							 //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glNormal3f(0, 1, 0);
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
		
	}
	glEnd();
}
void drawSingleGarra()
{
	glPushMatrix();
	//angle_garra_base = 120;
	glTranslatef(0.5f, -0.25f, 0);
	glRotatef(angulo_garra_base, 0, 0, 1);
	drawCuboid(garra);

	glTranslatef(0, -0.4f, 0);
	glColor3f(1.0f, 1.0f, 0);
	glutSolidSphere(0.15f, 50, 50);
	//Abrir fechar garra implementar aqui IF

	glRotatef(-angulo_garra_base - angulo_garra, 0, 0, 1);
	glTranslatef(0, -0.4f, 0);
	drawCuboid(garra);

	glPopMatrix();
}
void drawGarras()
{
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		glRotatef(60, 0, 1, 0);
		drawSingleGarra();
	}
	
	
	/*float angle_garra_base = 30;
	glPushMatrix();

	glTranslatef(0.5f, -0.25f, 0);
	glRotatef(angle_garra_base, 0, 0, 1);
	drawCuboid(garra);

	glTranslatef(0, -0.4f, 0);
	glColor3f(1.0f, 1.0f, 0);
	glutSolidSphere(0.15f, 50, 50);
	//Abrir fechar garra implementar aqui IF
	
	glRotatef(-angle_garra_base -angulo_garra, 0, 0, 1);
	glTranslatef(0, -0.4f, 0);
	drawCuboid(garra);

	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -0.25f, 0.5f);
	glRotatef(-30, 1, 0, 0);
	drawCuboid(garra);

	glTranslatef(0, -0.4f, 0);
	glColor3f(1.0f, 1.0f, 0);
	glutSolidSphere(0.15f, 50, 50);
	//Abrir fechar garra implementar aqui IF
	glRotatef(30+angulo_garra, 1, 0, 0);
	glTranslatef(0, -0.4f, 0);
	
	drawCuboid(garra);

	glPopMatrix();
	glPushMatrix();

	glTranslatef(-0.5f, -0.25, 0);
	glRotatef(-30, 0, 0, 1);
	drawCuboid(garra);

	glTranslatef(0, -0.4f, 0);
	glColor3f(1.0f, 1.0f, 0);
	glutSolidSphere(0.15f, 50, 50);
	//Abrir fechar garra implementar aqui IF
	glRotatef(30+angulo_garra, 0, 0, 1);
	glTranslatef(0, -0.4f, 0);
	
	drawCuboid(garra);

	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -0.25, -0.5f);
	glRotatef(30, 1, 0, 0);
	drawCuboid(garra);

	glTranslatef(0, -0.4f, 0);
	glColor3f(1.0f, 1.0f, 0);
	glutSolidSphere(0.15f, 50, 50);
	//Abrir fechar garra implementar aqui IF
	glRotatef(-30 - angulo_garra, 1, 0, 0);
	glTranslatef(0, -0.4f, 0);
	
	drawCuboid(garra);

	glPopMatrix();
	*/
}

class Point2D
{
public:
	float x;
	float y;
};

GLdouble ox = 0.0, oy = 0.0, oz = 0.0;
void mouse(int button, int state, int x, int y) {
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLfloat wx = x, wy, wz;

	
	if (button == GLUT_RIGHT_BUTTON)
		exit(0);
	glGetIntegerv(GL_VIEWPORT, viewport);
	y = viewport[3] - y;
	wy = y;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
	gluUnProject(wx, wy, wz, modelview, projection, viewport, &ox, &oy, &oz);
	ox -= baseCube.pos.getX();
	oy -= baseCube.pos.getY();
	oz -= baseCube.pos.getZ();
	if (button == GLUT_LEFT_BUTTON)
	{
		if (previewing_bola) {
			preview_bola.pos.setCoords(ox, oy, oz);
			previewing_bola = false;
		}
		if (ballGrabbed)
		{
			hRotationEixo1();
			throwingBall = true;
		}
	}
	//glutPostRedisplay();
}

void getMouseRay()
{
	POINT mousePos;
	
	

	GetCursorPos(&mousePos);
	
	ScreenToClient(WindowFromPoint(mousePos), &mousePos);
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLfloat wx = mousePos.x, wy, wz;
	
	//GLdouble nx = 0, ny = 0, nz = 0;


	glGetIntegerv(GL_VIEWPORT, viewport);
	int y = viewport[3] - mousePos.y;
	wy = y;
	int x = wx;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
	gluUnProject(wx, wy, wz, modelview, projection, viewport, &ox, &oy, &oz);
	/*if (ox == nx - baseCube.pos.getX())
	{
		if(oy == ny - baseCube.pos.getY())
			if(oz == nz - baseCube.pos.getZ() + 0.4f)


	}*/
	ox -= baseCube.pos.getX();
	oy -= baseCube.pos.getY();
	oz -= baseCube.pos.getZ();
}




void hRotationEixo1()
{
	Point2D o, a, b, test;
	//Origem será o eixo 1
	o.x = eixo1.pos.getX();
	o.y = eixo1.pos.getZ();
	//Ponto A será o eixo da garra
	a.x = eixo_garra.pos.getX();
	a.y = eixo_garra.pos.getZ();
	//Ponto B será a bola
	b.x = preview_bola.pos.getX() + baseCube.pos.getX();
	b.y = preview_bola.pos.getZ() + baseCube.pos.getZ();
	

	test = b;
	test.x -= a.x;
	test.y -= a.y;

	normal_eixo1.crossProduct(Point3D(a.x, 0, a.y), Point3D(b.x, 0, b.y));
	normal_eixo1.setCoords(0, normal_eixo1.getY()/sqrtf(powf(normal_eixo1.getY(),2)), 0);
	
	

	next_angulo_eixo1_horizontal = acosf(((a.x - o.x)*(b.x - o.x) + (a.y - o.y)*(b.y - o.y)) / (sqrtf(powf(a.x - o.x, 2) + powf(a.y - o.y, 2))*sqrtf(powf(b.x - o.x, 2) + powf(b.y - o.y, 2))));
	
	next_angulo_eixo1_horizontal = ((next_angulo_eixo1_horizontal) * (180 / PI)) * normal_eixo1.getY() + angulo_eixo1_horizontal;
	

}
void vRotationEixo1_2(Point3D ballPos)
{
	//
	//Vertices conhecidos do triangulo = EIXO 1 e BOLA
	Point2D a, b, c, a2, b2, c2, a3, b3, c3, d,e;
	int sinal_eixo1 = (eixo1.pos.getX() > 0) ? 1 : -1;
	int sinal_bola = (ballPos.getX() > 0) ? 1 : -1;
	int sinal_eixo2 = (eixo2.pos.getX() > 0) ? 1 : -1;
	d.x = sqrtf(powf(eixo2.pos.getX(), 2) + powf(eixo2.pos.getZ(), 2)) * sinal_eixo2;
	d.y = eixo2.pos.getY();
	printf("eixo2 Y %f\n", d.y);
	a.x = sqrtf(powf(eixo1.pos.getX(), 2) + powf(eixo1.pos.getZ(), 2)) * sinal_eixo1;
	b.x = sqrtf(powf(ballPos.getX(), 2) + powf(ballPos.getZ(), 2)) * sinal_bola;
	a.y = eixo1.pos.getY();
	b.y = ballPos.getY();

	e.x = sqrtf(powf(eixo_garra.pos.getX(), 2) + powf(eixo_garra.pos.getZ(), 2));
	e.y = eixo_garra.pos.getY() - sqrtf(3)*1.3 / 2;

	//Trazendo A para a origem
	a2.x = 0;
	a2.y = 0;
	b2.x = b.x - a.x;
	b2.y = b.y - a.y;

	//Rotacionando b para que ele esteja no eixo X
	a3 = a2;
	b3.y = 0;
	b3.x = sqrtf(powf(b2.x - a2.x, 2) + powf(b2.y - a2.y, 2));
	//angulo da rotação acima
	float theta = atan2f(b.y - a.y, b.x - a.x);
	float dAB = b3.x;
	float dAC = 4;
	float dBC = 4.0f + (sqrtf(3)*1.3 / 2);
	//float dBC = 4.0f;


	c3.x = (powf(dAB,2)+ powf(dAC,2) - powf(dBC,2)) / (dAB * 2);
	c3.y = sqrtf((dAB + dAC + dBC)*(dAB + dAC - dBC)*(dAB - dAC + dBC)*(-dAB + dAC + dBC)) / (2 * dAB);
	
	printf("a3: x: %f, y: %f\n", a3.x, a3.y);
	printf("b3: x: %f, y: %f\n", b3.x, b3.y);
	printf("c3: x: %f, y: %f\n", c3.x, c3.y);
	float angleA = acosf(((c3.x - a3.x)*(b3.x - a3.x) + (c3.y - a3.y)*(b3.y - a3.y)) / (sqrtf(powf(c3.x - a3.x, 2) + powf(c3.y - a3.y, 2))*sqrtf(powf(b3.x - a3.x, 2) + powf(b3.y - a3.y, 2))));
	float angleB = acosf(((b3.x - c3.x)*(a3.x - c3.x) + (b3.y - c3.y)*(a3.y - c3.y)) / (sqrtf(powf(b3.x - c3.x, 2) + powf(b3.y - c3.y, 2))*sqrtf(powf(a3.x - c3.x, 2) + powf(a3.y - c3.y, 2))));
	float angleSup = acosf(((d.x - a.x)*(b.x - a.x) + (d.y - a.y)*(b.y - a.y)) / (sqrtf(powf(d.x - a.x, 2) + powf(d.y - a.y, 2))*sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2))));
	//float angleSup2 = acosf(((a.x - e.x)*(b.x - e.x) + (a.y - e.y)*(b.y - e.y)) / (sqrtf(powf(a.x - e.x, 2) + powf(a.y - e.y, 2))*sqrtf(powf(b.x - e.x, 2) + powf(b.y - e.y, 2))));
	//float angleSup2 = acosf(((a.x - d.x)*(b.x - d.x) + (a.y - d.y)*(b.y - d.y)) / (sqrtf(powf(a.x - d.x, 2) + powf(a.y - d.y, 2))*sqrtf(powf(b.x - d.x, 2) + powf(b.y - d.y, 2))));
	printf("AngleA: %f\nAngleB: %f\n", angleA, angleB);
	printf("angleSUP: %f\n", angleSup * 180 / PI);
	
	next_angulo_eixo1_vertical = ((angleSup-angleA) * (180 / PI)) + angulo_eixo1_vertical;
	next_angulo_eixo2_vertical =  ((angleB) * (180 / PI))-90;
	//float angleB
	//teste

	//b = bola

	
}

/* Handler for window-repaint event. Call back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();                           // Reset The Projection Matrix
	gluPerspective(120.0f, (GLfloat)1000 / (GLfloat)700, 0.1f, 100.0f);
	glTranslatef(-1.5f, 0.0f, -13.0f);
	glRotatef(angulo_X, 1.0f, 0, 0.0);
	glRotatef(angulo_Y, 0.0f, 1.0f, 0.0);
	glTranslatef(1.5f, 0.0f, 13.0f);

	glGetFloatv(GL_MODELVIEW_MATRIX, camData.mat);
	gluInvertMatrix(camData.mat, camData.inverse_mat);


	glMatrixMode(GL_MODELVIEW); // I like to keep the matrix mode here unless changing the projection
	glLoadIdentity();                           // Reset The Current Modelview Matrix


	glTranslatef(-1.5f, 0.0f, -13.0f);                 // Move Left 1.5 Units And Into The Screen 6.0
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHTING);
	//Teste global position
	glGetFloatv(GL_MODELVIEW_MATRIX, baseCube.mat);
	npLocalToWorld(&baseCube.pos, camData.inverse_mat, baseCube.mat);


	drawCuboid(baseCube);
	

	//Draw MESA ou SUPORTE DO ROBO
	glPushMatrix();
	glTranslatef(0.0f, -6.0f, 0.0f);
	glColor3f(((float)139 / (float)255), (69.0f / 255.0f), 19.0f / 255);
	//glRotatef(90, 1, 0, 0);
	//glutSolidCone(8.5f, 4.0f, 50, 50);
	glutSolidCube(10.0f);
	glTranslatef(0.0f, 5.01f, 0);
	glRotatef(90, 1, 0, 0);
	drawFilledCircle(0, 0, 8.0f);
	//glPopMatrix();


	//glutSolidCube(2.0f);
	//glPushMatrix();
	if(previewing_bola)
	{
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -13.0f);                 // Move Left 1.5 Units And Into The Screen 6.0
	//glTranslatef(ox - baseCube.pos.getX(), oy - baseCube.pos.getY() + 0.4f, oz - baseCube.pos.getZ());
	glTranslatef((GLfloat)(ox), -0.9f, (GLfloat)(oz));
	glColor3f(1.0f, 0.0f, 0);
	glRotatef(90, 1, 0, 0);
	glDisable(GL_LIGHTING);

	drawFilledCircle(0, 0, 0.4f);
	glEnable(GL_LIGHTING);
	//DRAW BOLA
	//glGetFloatv(GL_MODELVIEW_MATRIX, preview_bola.mat);
	//npLocalToWorld(&preview_bola.pos, camData.inverse_mat, bola.mat);
	

	//glutSolidSphere(0.4f, 50, 50);
	
	}
	glPopMatrix();
	if (!ballGrabbed)
	{


		glPushMatrix();
		//glLoadIdentity();
		//glTranslatef(8.5f, -0.6f, 0.0f);
		glTranslatef(preview_bola.pos.getX(), -0.6f, preview_bola.pos.getZ());
		glColor3f(1.0f, 0.0f, 0);
		//DRAW BOLA
		glGetFloatv(GL_MODELVIEW_MATRIX, bola.mat);
		npLocalToWorld(&bola.pos, camData.inverse_mat, bola.mat);

		glutSolidSphere(0.4f, 50, 50);
		glPopMatrix();
	}


	glPushMatrix();
	glTranslatef(0, 1.5f, 0);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, eixo1.mat);
	npLocalToWorld(&eixo1.pos, camData.inverse_mat, eixo1.mat);
	
	glColor3f(1.0f, 1.0f, 0);
	glutSolidSphere(0.5f, 50, 50);
	//Rotação EIXO 1 entra aqui
	glRotatef(angulo_eixo1_horizontal, 0, 1, 0);
	glRotatef(-angulo_eixo1_vertical, 0, 0, 1.0f);//TESTE ROTATION
	glTranslatef(0, 2.0f, 0);

	glGetFloatv(GL_MODELVIEW_MATRIX, haste1.mat);
	npLocalToWorld(&haste1.pos, camData.inverse_mat, haste1.mat);
	drawCuboid(haste1);
	glTranslatef(0, 2.0f, 0);
	glColor3f(1.0f, 1.0f, 0);
	//Rotação EIXO 2
	glRotatef(angulo_eixo2_vertical, 0, 0, 1.00f);//TESTE ROTATION
	glGetFloatv(GL_MODELVIEW_MATRIX, eixo2.mat);
	npLocalToWorld(&eixo2.pos, camData.inverse_mat, eixo2.mat);

	glutSolidSphere(0.5f, 50, 50);
	glTranslatef(2.0f, 0, 0);

	glGetFloatv(GL_MODELVIEW_MATRIX, haste2.mat);
	npLocalToWorld(&haste2.pos, camData.inverse_mat, haste2.mat);

	drawCuboid(haste2);
	glTranslatef(2.0f, 0, 0);
	glColor3f(1.0f, 1.0f, 0);
	//Rotação eixo das garras - corrigir o angulo do eixo 1 + eixo 2

	glGetFloatv(GL_MODELVIEW_MATRIX, eixo_garra.mat);
	npLocalToWorld(&eixo_garra.pos, camData.inverse_mat, eixo_garra.mat);

	glPushMatrix();
	glutSolidSphere(0.5f, 50, 50);
	glRotatef(90, 0, 0, 1);
	drawGarras();
	glPopMatrix();
	if (ballGrabbed)
	{
		glPushMatrix();
		glTranslatef((sqrtf(3)*1.3 / 2), 0, 0);
		glColor3f(1.0f, 0.0f, 0);
		//DRAW BOLA
		glGetFloatv(GL_MODELVIEW_MATRIX, bola.mat);
		npLocalToWorld(&bola.pos, camData.inverse_mat, bola.mat);

		glutSolidSphere(0.4f, 50, 50);
		glPopMatrix();
	}
	glPopMatrix();
	if (previewing_bola) getMouseRay();//Atualizar a posição do mouse pro caso de estar previewing a posição da bola
	//glFlush();  // Render now
	glutSwapBuffers();
	
}

void processSpecialKeys(int key, int xx, int yy) {

	

	switch (key) {
	case GLUT_KEY_LEFT:
		angulo_Y += 15;
		angulo_Y = angulo_Y - ((int)(angulo_Y / 360)*(float)360);
		
		break;
	case GLUT_KEY_RIGHT:
		angulo_Y -= 15;
		angulo_Y = angulo_Y - ((int)(angulo_Y / 360)*(float)360);
		break;
	case GLUT_KEY_UP:
		angulo_X += 15;
		angulo_X = angulo_X - ((int)(angulo_X / 360)*(float)360);
		break;
	case GLUT_KEY_DOWN:
		angulo_X -= 15;
		angulo_X = angulo_X - ((int)(angulo_X / 360)*(float)360);
		break;
	case GLUT_KEY_END:
		printf("Base CUBE: X: %6.2f Y: %6.2f Z: %6.2f\n", baseCube.pos.getX(), baseCube.pos.getY(), baseCube.pos.getZ());
		printf("Bola: X: %6.2f Y: %6.2f Z: %6.2f\n", bola.pos.getX(), bola.pos.getY(), bola.pos.getZ());
		printf("Eixo 1: X: %6.2f Y: %6.2f Z: %6.2f\n", eixo1.pos.getX(), eixo1.pos.getY(), eixo1.pos.getZ());
		printf("Haste1: X: %6.2f Y: %6.2f Z: %6.2f\n", haste1.pos.getX(), haste1.pos.getY(), haste1.pos.getZ());
		printf("Eixo 2: X: %6.2f Y: %6.2f Z: %6.2f\n", eixo2.pos.getX(), eixo2.pos.getY(), eixo2.pos.getZ());
		printf("Haste2: X: %6.2f Y: %6.2f Z: %6.2f\n", haste2.pos.getX(), haste2.pos.getY(), haste2.pos.getZ());
		printf("Eixo Garra: X: %6.2f Y: %6.2f Z: %6.2f\n\n", eixo_garra.pos.getX(), eixo_garra.pos.getY(), eixo_garra.pos.getZ());
		printf("MouseX: %6.2f, MouseY: %6.2f, MouseZ: %6.2f\n", ox, oy, oz);
		
		break;
	case GLUT_KEY_PAGE_DOWN:
		hRotationEixo1();

		//vRotationEixo1_2();
		//grabbingBall = true;
		printf("Rotation: A: %6.2f, B:%6.2f\n", angulo_eixo1_vertical, angulo_eixo2_vertical);
		break;
	}
}



GLvoid ReSizeGLScene(GLsizei width, GLsizei height)             // Resize And Initialize The GL Window
{
	if (height == 0)                              // Prevent A Divide By Zero By
	{
		height = 1;                           // Making Height Equal One
	}

	glViewport(0, 0, width, height);                    // Reset The Current Viewport
	/*glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();                           // Reset The Projection Matrix

												// Calculate The Aspect Ratio Of The Window
	
	//glPushMatrix();
	gluPerspective(120.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	//glPopMatrix();
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glRotatef(45, 1.0f, 0, 0.0);
	glTranslatef(1.5f, 0.0f, 6.0f);*/
	
	
	

	glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
	glLoadIdentity();                           // Reset The Modelview Matrix

}

void idleFunc()
{
	glutPostRedisplay();
}

void animate(int value)
{
	// Set up the next timer tick (do this first)
	glutTimerFunc(TIMER_FRAMES, animate, 0);

	

	// Measure the elapsed time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;
	int elapsedTime = currTime - startTime;

	if (angulo_eixo1_horizontal != next_angulo_eixo1_horizontal)//Animação EIXO1
	{
		if (angulo_eixo1_horizontal < next_angulo_eixo1_horizontal) {

			angulo_eixo1_horizontal += ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_eixo1_horizontal > next_angulo_eixo1_horizontal)
			{
				angulo_eixo1_horizontal = next_angulo_eixo1_horizontal;
				if(ballGrabbed)
					vRotationEixo1_2(Point3D(preview_bola.pos.getX() + baseCube.pos.getX(), preview_bola.pos.getY() + baseCube.pos.getY() +0.4f, preview_bola.pos.getZ() + baseCube.pos.getZ()));
				else
				{
					vRotationEixo1_2(bola.pos);
					grabbingBall = true;
				}
			}
		}
		else
		{
			angulo_eixo1_horizontal -= ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_eixo1_horizontal < next_angulo_eixo1_horizontal)
			{
				angulo_eixo1_horizontal = next_angulo_eixo1_horizontal;
				if (ballGrabbed)
					vRotationEixo1_2(Point3D(preview_bola.pos.getX() + baseCube.pos.getX(), preview_bola.pos.getY() + baseCube.pos.getY() + 0.4f, preview_bola.pos.getZ() + baseCube.pos.getZ()));
				else
				{
					vRotationEixo1_2(bola.pos);
					grabbingBall = true;
				}
			}
		}
	}

	if (angulo_eixo1_vertical != next_angulo_eixo1_vertical)//Animação EIXO1
	{
		if (angulo_eixo1_vertical < next_angulo_eixo1_vertical) {
			
			angulo_eixo1_vertical += ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_eixo1_vertical > next_angulo_eixo1_vertical) angulo_eixo1_vertical = next_angulo_eixo1_vertical;
		}
		else
		{
			angulo_eixo1_vertical -= ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_eixo1_vertical < next_angulo_eixo1_vertical) angulo_eixo1_vertical = next_angulo_eixo1_vertical;
		}
	}
	if ( angulo_eixo2_vertical != next_angulo_eixo2_vertical)//Animação EIXO2
	{
		if (angulo_eixo2_vertical < next_angulo_eixo2_vertical) {
			angulo_eixo2_vertical += ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_eixo2_vertical > next_angulo_eixo2_vertical)
			{
				angulo_eixo2_vertical = next_angulo_eixo2_vertical;
				/*if (ballGrabbed && throwingBall)
				{
					ballGrabbed = false;
					throwingBall = false;
				}*/
			}
		}
		else
		{
			angulo_eixo2_vertical -= ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_eixo2_vertical < next_angulo_eixo2_vertical)
			{
				angulo_eixo2_vertical = next_angulo_eixo2_vertical;
				/*if (ballGrabbed && throwingBall)
				{
					ballGrabbed = false;
					throwingBall = false;
				}*/
			}
		}
	}
	
	//Animação garra
	if (animGarra)
	{
		if (angulo_garra_base > next_angulo_garra_base)
		{
			angulo_garra_base -= ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_garra_base < next_angulo_garra_base) {
				angulo_garra_base = next_angulo_garra_base;
				animGarra = false;
				if (throwingBall)
				{
					throwingBall = false;
					ballGrabbed = false;
					next_angulo_eixo1_vertical = 0;
					next_angulo_eixo2_vertical = 0;
				}
			}
		}
		else if(angulo_garra_base < next_angulo_garra_base)
		{
			angulo_garra_base += ((float)VELOCIDADE_EIXOS / (float)1000)*timeSincePrevFrame;
			if (angulo_garra_base > next_angulo_garra_base) {
				angulo_garra_base = next_angulo_garra_base;
				animGarra = false;
				if (throwingBall)
				{
					throwingBall = false;
					ballGrabbed = false;
					next_angulo_eixo1_vertical = 0;
					next_angulo_eixo2_vertical = 0;
				}
			}
		}
	}

	if (grabbingBall)
	{
		if (angulo_eixo1_vertical == next_angulo_eixo1_vertical && angulo_eixo2_vertical == next_angulo_eixo2_vertical)
		{
			next_angulo_garra = ANGULO_GARRA_FECHADA;
			next_angulo_garra_base = ANGULO_GARRA_FECHADA;
			grabbingBall = false;
			animGarra = true;
			ballGrabbed = true;
		}
	}

	if (throwingBall)
	{
		if (angulo_eixo1_vertical == next_angulo_eixo1_vertical && angulo_eixo2_vertical == next_angulo_eixo2_vertical && angulo_eixo1_horizontal == next_angulo_eixo1_horizontal)
		{
			next_angulo_garra = ANGULO_GARRA_FECHADA;
			next_angulo_garra_base = ANGULO_GARRA_ABERTA;
			
			animGarra = true;
			
		}
	}

	if (ballGrabbed && !animGarra && !throwingBall)
	{
		next_angulo_eixo1_vertical = 0;
		next_angulo_eixo2_vertical = 0;
	}
	
	
	// Rotate the triangle
	//rot = (ROTRATE / 1000) * elapsedTime;

	

	// ##### END OF GAME/APP MAIN CODE #####



	// Force a redisplay to render the new image
	glutPostRedisplay();

	prevTime = currTime;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		if (!grabbingBall && (angulo_eixo1_vertical == next_angulo_eixo1_vertical) && (angulo_eixo2_vertical == next_angulo_eixo2_vertical))
		{

			previewing_bola = !previewing_bola;
			if (previewing_bola == true)
				preview_bola.pos.setCoords(bola.pos.getX() - baseCube.pos.getX(), bola.pos.getY() - baseCube.pos.getY() - 0.6f, bola.pos.getZ() - baseCube.pos.getZ());
			
		}
		break;
	}
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	//Initializing objects
	
	


	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitWindowSize(1000, 700);   // Set the window's initial width & height
	glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
	glutCreateWindow("Robot - CG"); // Create a window with the given title
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Set up a basic display buffer (only single buffered for now)
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	//GLfloat light_position[] = { 0, 5.0, 0.0f, 1.0f };
	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[] = { 1.5f, 0.0f, 6.0f, 1.0f };
	GLfloat spot_cutoff[] = { 180.0f };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	//glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
	glFrontFace(GL_CW);

	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);	//enable automatic normalization


	glClearDepth(1.0);	//specify depth for clear buffer. [0,1]
	glDepthFunc(GL_LEQUAL);	//remove anything equal or farther away
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations

	
	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	glutReshapeFunc(ReSizeGLScene);
	ReSizeGLScene(1000, 700);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	//glutIdleFunc(idleFunc);
	
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(keyboard);
	
	// Start the timer
	glutTimerFunc(TIMER_FRAMES, animate, 0);

	// Initialize the time variables
	startTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = startTime;

	glutMouseFunc(mouse);
	glutMainLoop();           // Enter the infinitely event-processing loop

	return 0;
}