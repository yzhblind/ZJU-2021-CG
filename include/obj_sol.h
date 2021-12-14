#ifndef __Obj_Sol__

#define __Obj_Sol__

#define  Group_NAME0 "____DEFAULT_CYBBB______"
#include<cstdio>
#include<vector>
#include<string>
#include<iostream>



class Point {
public:
double x, y, z, w;
Point(double _x=0, double _y=0, double _z=0, double _w=1.0):x(_x),y(_y),z(_z),w(_w)
{
}

};

class Normal {
public:
double x, y, z;
Normal(double _x=0, double _y=0, double _z=0):x(_x),y(_y),z(_z)
{
}

};

class Int_3 {
public:
	int x, y, z;
	Int_3(int _x=0, int _y=0, int _z=0):x(_x),y(_y),z(_z) {}
};

class Face {
public:
	std::vector<Int_3>Points;
	Face() {}
};

std::vector<double> get_number(std::string s);

//g     分组：  g  name  
//v     顶点坐标 第四个参数可选, 默认1.0
//vt    纹理坐标 二个参数
//vn    顶点法向量 三个参数
/* f    面 
	使用顶点、纹理和法线索引列表定义的，
	格式为 vertex_index/texture_index/normal_index，
	其中每个索引从 1 开始，并根据定义引用元素的顺序增加。
	可以使用三个以上的索引来定义四边形等多边形。
*/
class Group {
public:
	std::string name;
	std::vector<Point>v;
	std::vector<std::pair<double,double>>vt;
	std::vector<Normal>vn;
	std::vector<Face>f;

Group() {
	v.push_back(Point());
	vt.push_back(std::make_pair(0,0));
	vn.push_back(Normal());
}

void set_name(std::string s);
void set(std::vector<std::string>G);

};

class OBJ {
	
public:

std::vector <Group> All_Group;

OBJ(){
	All_Group.resize(1);
	All_Group[0].set_name(Group_NAME0);
}
void Get_from_File(std::string s);
void Out_to_File(std::string s);
	
	
};



#endif
