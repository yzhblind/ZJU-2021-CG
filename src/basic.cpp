#include<basic.h>
Cube::Cube(GLfloat len) {
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
	length = len;
	init();
}
Sphere::Sphere(GLfloat r, int a, int b)
{
	const double pi = acos(-1.0);
	this->r = r; this->a = a;this->b = b;
	int point_count = 0;
	for (int i = 0;i < a;i++)
	{
		for (int j = 0;j < b;j++)
		{
			Vertex p1, p2, p3, p4;
			double alpha = pi / a * i, theta = 2.0 * pi / b * j;
			p1.pos = glm::vec3(r * cos(alpha) * cos(theta), r * cos(alpha) * sin(theta), r * sin(alpha));
			alpha = pi / a * i, theta = 2.0 * pi / b * (j + 1);
			p2.pos = glm::vec3(r * cos(alpha) * cos(theta), r * cos(alpha) * sin(theta), r * sin(alpha));
			alpha = pi / a * (i + 1), theta = 2.0 * pi / b * (j + 1);
			p3.pos = glm::vec3(r * cos(alpha) * cos(theta), r * cos(alpha) * sin(theta), r * sin(alpha));
			alpha = pi / a * (i + 1), theta = 2.0 * pi / b * j;
			p4.pos = glm::vec3(r * cos(alpha) * cos(theta), r * cos(alpha) * sin(theta), r * sin(alpha));
			glm::vec3 normal = (p3.pos - p2.pos) * (p1.pos - p2.pos);
			p1.normal = p2.normal = p3.normal = p4.normal = normal;
			push_back(p1);push_back(p2);push_back(p3);push_back(p4);
			unsigned int a[] = { 0,1,2,0,2,3 };
			for (int l = 0;l < 6;l++)
				push_back(a[l] + point_count);
			point_count += 4;
		}
	}
	init();
}
Prism::Prism(GLfloat r, GLfloat h, GLint num)
{
	const double pi = acos(-1.0);
	std::vector <Vertex> v;
	std::vector <unsigned int> tmp_index;
	for (int i = num - 1;i >= 0;i--)
	{
		Vertex p;
		p.normal = glm::vec3(0, 0, -1);
		p.pos = glm::vec3(r * cos(2.0 * pi / num * i), r * sin(2.0 * pi / num * i), 0);
		v.push_back(p);
	}
	for (int i = 1;i <= num - 2;i++)
	{
		tmp_index.push_back(0);
		tmp_index.push_back(i);
		tmp_index.push_back(i + 1);
	}
	for (int i = 0;i < num;i++)
	{
		Vertex p;
		p.normal = glm::vec3(0, 0, 1);
		p.pos = glm::vec3(r * cos(2.0 * pi / num * i), r * sin(2.0 * pi / num * i), h);
		v.push_back(p);
	}
	for (int i = 1;i <= num - 2;i++)
	{
		tmp_index.push_back(num);
		tmp_index.push_back(i + num);
		tmp_index.push_back(i + num + 1);
	}
	for (int i = 0;i < num;i++)
	{
		Vertex p1, p2, p3, p4;
		p1.pos = glm::vec3(r * cos(2.0 * pi / num * i), r * sin(2.0 * pi / num * i), 0);
		p2.pos = glm::vec3(r * cos(2.0 * pi / num * (i + 1)), r * sin(2.0 * pi / num * (i + 1)), 0);
		p3.pos = glm::vec3(r * cos(2.0 * pi / num * (i + 1)), r * sin(2.0 * pi / num * (i + 1)), h);
		p4.pos = glm::vec3(r * cos(2.0 * pi / num * i), r * sin(2.0 * pi / num * i), h);
		glm::vec3 normal = (p3.pos - p2.pos) * (p1.pos - p2.pos);
		p1.normal = p2.normal = p3.normal = p4.normal = normal;
		int offset = v.size();
		v.push_back(p1);v.push_back(p2);v.push_back(p3);v.push_back(p4);
		unsigned int a[] = { 0,1,2,0,2,3 };
		for (int i = 0;i < 6;i++)
			tmp_index.push_back(a[i] + offset);
	}
	int n = v.size();
	for (int i = 0;i < n;i++) push_back(v[i]);
	n = tmp_index.size();
	for (int i = 0;i < n;i++) push_back(tmp_index[i]);
	this->r = r;this->h = h;
	this->num = num;
	init();

}
Cone::Cone(GLfloat r, GLfloat h, int num)
{
	this->r = r;this->h = h;
	const double pi = acos(-1.0);
	std::vector <Vertex> v;
	std::vector <unsigned int> tmp_index;
	for (int i = num - 1;i >= 0;i--)
	{
		Vertex p;
		p.normal = glm::vec3(0, 0, -1);
		p.pos = glm::vec3(r * cos(2.0 * pi / num * i), r * sin(2.0 * pi / num * i), 0);
		v.push_back(p);
	}
	for (int i = 1;i <= num - 2;i++)
	{
		tmp_index.push_back(0);
		tmp_index.push_back(i);
		tmp_index.push_back(i + 1);
	}
	for (int i = 0;i < num;i++)
	{
		Vertex p1, p2, p3;
		p1.pos = glm::vec3(r * cos(2.0 * pi / num * i), r * sin(2.0 * pi / num * i), 0);
		p2.pos = glm::vec3(r * cos(2.0 * pi / num * (i + 1)), r * sin(2.0 * pi / num * (i + 1)), 0);
		p3.pos = glm::vec3(0, 0, h);
		int offset = v.size();
		glm::vec3 normal = (p1.pos - p2.pos) * (p3.pos - p2.pos);
		p1.normal = p2.normal = p3.normal = normal;
		v.push_back(p1);v.push_back(p2);v.push_back(p3);
		tmp_index.push_back(offset);
		tmp_index.push_back(offset + 1);
		tmp_index.push_back(offset + 2);
	}
	int n = v.size();
	for (int i = 0;i < n;i++) push_back(v[i]);
	n = tmp_index.size();
	for (int i = 0;i < n;i++) push_back(tmp_index[i]);
	init();
}
Frustum::Frustum(GLfloat rh, GLfloat rl, GLfloat h, int num)
{
	const double pi = acos(-1.0);
	std::vector <Vertex> v;
	std::vector <unsigned int> tmp_index;
	for (int i = num - 1;i >= 0;i--)
	{
		Vertex p;
		p.normal = glm::vec3(0, 0, -1);
		p.pos = glm::vec3(rl * cos(2.0 * pi / num * i), rl * sin(2.0 * pi / num * i), 0);
		v.push_back(p);
	}
	for (int i = 1;i <= num - 2;i++)
	{
		tmp_index.push_back(0);
		tmp_index.push_back(i);
		tmp_index.push_back(i + 1);
	}
	for (int i = 0;i < num;i++)
	{
		Vertex p;
		p.normal = glm::vec3(0, 0, 1);
		p.pos = glm::vec3(rh * cos(2.0 * pi / num * i), rh * sin(2.0 * pi / num * i), h);
		v.push_back(p);
	}
	for (int i = 1;i <= num - 2;i++)
	{
		tmp_index.push_back(num);
		tmp_index.push_back(i + num);
		tmp_index.push_back(i + num + 1);
	}

	for (int i = 0;i < num;i++)
	{
		Vertex p1, p2, p3, p4;
		p1.pos = glm::vec3(rl * cos(2.0 * pi / num * i), rl * sin(2.0 * pi / num * i), 0);
		p2.pos = glm::vec3(rl * cos(2.0 * pi / num * (i + 1)), rl * sin(2.0 * pi / num * (i + 1)), 0);
		p3.pos = glm::vec3(rh * cos(2.0 * pi / num * (i + 1)), rh * sin(2.0 * pi / num * (i + 1)), h);
		p4.pos = glm::vec3(rh * cos(2.0 * pi / num * i), rh * sin(2.0 * pi / num * i), h);
		glm::vec3 normal = (p3.pos - p2.pos) * (p1.pos - p2.pos);
		p1.normal = p2.normal = p3.normal = p4.normal = normal;
		int offset = v.size();
		v.push_back(p1);v.push_back(p2);v.push_back(p3);v.push_back(p4);
		unsigned int a[] = { 0,1,2,0,2,3 };
		for (int i = 0;i < 6;i++)
			tmp_index.push_back(a[i] + offset);
	}
	int n = v.size();
	for (int i = 0;i < n;i++) push_back(v[i]);
	n = tmp_index.size();
	for (int i = 0;i < n;i++) push_back(tmp_index[i]);
	this->rh = rh;this->rl = rl; this->h = h;
	this->num = num;
	init();
}