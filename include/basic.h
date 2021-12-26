#pragma once
#include<mesh.h>
#include <glm/glm.hpp>

class Cube : public Mesh
{
public:
	Cube(GLfloat len, glm::vec3 );
private:
	GLfloat length;
};
class Sphere :public Mesh
{
public:
	Sphere(GLfloat r, int a=100, int b=100);
private:
	GLfloat r;
	int a, b;
};
class Prism :public Mesh
{
public:
	//������r:�뾶��h:�߶ȣ�num:����
	Prism(GLfloat r, GLfloat h, GLint num);
private:
	GLfloat r, h;
	GLint num;
};
class Cylinder :public Prism
{
public:
	Cylinder(GLfloat r, GLfloat h) :Prism(r, h, 100) {};
};
class Cone : public Mesh
{
public:
	Cone(GLfloat r, GLfloat h, int num=100);
private:
	GLfloat r, h;
};
class Frustum :public Mesh
{
public:
	//��̨ rh:����뾶��rl������뾶��h����
	Frustum(GLfloat rh, GLfloat rl, GLfloat h, int num );
private:
	GLfloat rh, rl, h;
	int num;
};