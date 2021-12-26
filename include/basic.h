#pragma once
#include<mesh.h>
#include <glm/glm.hpp>
class Cube : public Mesh
{
public:
	Cube(GLfloat len) {
		Vertex p[24];
		p[0].pos = glm::vec3(0, 0, 0);p[0].normal = glm::vec3(-1, 0, 0);
		p[1].pos = glm::vec3(0, 0, len);p[1].normal = glm::vec3(-1, 0, 0);
		p[2].pos = glm::vec3(0, len, len);p[2].normal = glm::vec3(-1, 0, 0);
		p[3].pos = glm::vec3(0, len, 0);p[3].normal = glm::vec3(-1, 0, 0);
		p[4].pos = glm::vec3(len, 0, 0);p[4].normal = glm::vec3(1, 0, 0);
		p[5].pos = glm::vec3(len, 0, len);p[5].normal = glm::vec3(1, 0, 0);
		p[6].pos = glm::vec3(len, len, len);p[6].normal = glm::vec3(1, 0, 0);
		p[7].pos = glm::vec3(len, len, 0);p[7].normal = glm::vec3(1, 0, 0);

		p[8].pos = glm::vec3(0, 0, 0);p[8].normal = glm::vec3(0, -1, 0);
		p[9].pos = glm::vec3(len, 0, 0);p[9].normal = glm::vec3(0, -1, 0);
		p[10].pos = glm::vec3(len, 0, len);p[10].normal = glm::vec3(0, -1, 0);
		p[11].pos = glm::vec3(0, 0, len);p[11].normal = glm::vec3(0, -1, 0);
		p[12].pos = glm::vec3(0, len, 0);p[12].normal = glm::vec3(0, 1, 0);
		p[13].pos = glm::vec3(len, len, 0);p[13].normal = glm::vec3(0, 1, 0);
		p[14].pos = glm::vec3(len, len, len);p[14].normal = glm::vec3(0, 1, 0);
		p[15].pos = glm::vec3(0, len, len);p[15].normal = glm::vec3(0, 1, 0);

		p[16].pos = glm::vec3(0, 0, 0);p[16].normal = glm::vec3(0, 0, -1);
		p[17].pos = glm::vec3(0, len, 0);p[17].normal = glm::vec3(0, 0, -1);
		p[18].pos = glm::vec3(len, len, 0);p[18].normal = glm::vec3(0, 0, -1);
		p[19].pos = glm::vec3(len, 0, 0);p[19].normal = glm::vec3(0, 0, -1);
		p[20].pos = glm::vec3(0, 0, len);p[20].normal = glm::vec3(0, 0, 1);
		p[21].pos = glm::vec3(0, len, len);p[21].normal = glm::vec3(0, 0, 1);
		p[22].pos = glm::vec3(len, len, len);p[22].normal = glm::vec3(0, 0, 1);
		p[23].pos = glm::vec3(len, 0, len);p[23].normal = glm::vec3(0, 0, 1);
		for (int i = 0;i < 24;i++) push_back(p[i]);
		unsigned int a[] = { 0,1,2,0,2,3,7,6,5,7,5,4,8,9,10,8,10,11,15,14,13,15,13,12,16,17,18,16,18,19,23,22,21,23,21,20 };
		for (int i = 0;i < 36;i++) push_back(a[i]);

	}
private:
	GLfloat length;
};