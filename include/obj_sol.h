#ifndef __Obj_Sol__

#define __Obj_Sol__


#define  NAME0 "____DEFAULT_CYBBB______"

#include<bits/stdc++.h>
using namespace std;




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
	vector<Int_3>Points;
	Face() {}
};

vector<double> get_number(string s);

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
	string name;
	vector<Point>v;
	vector<pair<double,double>>vt;
	vector<Normal>vn;
	vector<Face>f;

Group() {
	v.push_back(Point());
	vt.push_back(make_pair(0,0));
	vn.push_back(Normal());
}

void set_name(string s);
void set(vector<string>G);

};

class OBJ {
	
public:

vector <Group> All_Group;

OBJ(){
	All_Group.resize(1);
	All_Group[0].set_name(NAME0);
}
void Get_from_File(string s);
void Out_to_File(string s);
	
	
};



#endif
