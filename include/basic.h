#pragma once
#include<mesh.h>
#include<algorithm>
#include <glm/glm.hpp>

class Cube : public Mesh
{
public:
	Cube(GLfloat len, glm::vec3 trans=glm::vec3(0,0,0));
private:
	GLfloat length;
};
class Sphere :public Mesh
{
public:
	Sphere(GLfloat r, int a=100, int b=100, glm::vec3 trans = glm::vec3(0, 0, 0));
private:
	GLfloat r;
	int a, b;
};
class Prism :public Mesh
{
public:
	//棱柱，r:半径，h:高度，num:棱数
	Prism(GLfloat r, GLfloat h, GLint num, glm::vec3 trans = glm::vec3(0, 0, 0));
private:
	GLfloat r, h;
	GLint num;
};
class Cylinder :public Prism
{
public:
	Cylinder(GLfloat r, GLfloat h, glm::vec3 trans = glm::vec3(0, 0, 0)) :Prism(r, h, 100,trans) {};
};
class Cone : public Mesh
{
public:
	Cone(GLfloat r, GLfloat h, int num=100, glm::vec3 trans = glm::vec3(0, 0, 0));
private:
	GLfloat r, h;
};
class Frustum :public Mesh
{
public:
	//棱台 rh:上面半径，rl：下面半径，h：高
	Frustum(GLfloat rh, GLfloat rl, GLfloat h, int num, glm::vec3 trans = glm::vec3(0, 0, 0));
private:
	GLfloat rh, rl, h;
	int num;
};