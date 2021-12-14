#include "obj_sol.h"

//Debug
//int FL;


using namespace std;

vector<double> get_number(string s)
{
	while(s.size() && s[0]==' ')s=s.substr(1,s.size()-1);
	while(s.size() && s[s.size()-1]==' ')s=s.substr(0,s.size()-1);
	//a a a a a//
	vector<double>Ans;

	//cerr<<s.size()<<"size"<<endl;

	if(!s.size()) return Ans;
	s = s + " ";
	//a a a a a //
	while(s.size()){
		int u = s.find(' ',0);
		string t = s.substr(0,u);

		if(!t.size()) break;
		//cerr<<t<<"()"<<endl;
		int _fl = 1;
		if(t[0]=='-'){
			_fl=-1;
			t=t.substr(1,t.size()-1);
		}

		s = s.substr(u+1,s.size()-u-1);

		int fl = 0;
		double ss = 0, op = 1;
		for(auto i = 0; i<t.size(); ++i){
			if(t[i]=='.'){
				op /= 10;
				fl = 1;
				continue;
			}
			if(!fl){
				ss = ss * 10 + t[i] - 48;
				continue;
			}
			ss += op*(t[i]-48);
			op /= 10;
		}
		ss *= _fl;
		//cerr<<"ss"<<ss<<endl;
		Ans.push_back(ss);
	}

	return Ans;
}

void Group:: set_name(string s)
{
	name = s;
}




void Group:: set(vector<string>G)
{
	//cerr<<G.size()<<endl;


	for(auto u: G){

		//cerr<<u<<endl;

		string s = u;
		if(s.size() >= 2 && s[0]=='v' && s[1]==' '){

			//cerr<<"POINT"<<s.substr(2,s.size()-2)<<endl;

			vector<double> ss = get_number(s.substr(2,s.size()-2));
			if(ss.size() < 3) {
				cerr << "Point format error !" << endl;
				continue;
			}

			if(ss.size() < 4) ss.push_back(1.0);

		//	cerr<< ss[0] <<"***"<<endl;

			v.push_back(Point(ss[0],ss[1],ss[2],ss[3]));
			continue;
		}

		if(s.size() >= 2 && s[0]=='v' && s[1]=='t'){
			vector<double> ss = get_number(s.substr(2,s.size()-2));
			if(ss.size() < 2) {
				cerr << "Texture format error !" << endl;
				continue;
			}
			vt.push_back(make_pair(ss[0],ss[1]));
			continue;
		}

		if(s.size() >= 2 && s[0]=='v' && s[1]=='n'){
			vector<double> ss = get_number(s.substr(2,s.size()-2));
			if(ss.size() < 3) {
				cerr << "Normal format error !" << endl;
				continue;
			}
			vn.push_back(Normal(ss[0],ss[1],ss[2]));
			continue;
		}

		if(s.size() >= 2 && s[0]=='f' && s[1]==' '){
			string t = s.substr(2,s.size()-2);
			t += ' ';
			
		//	if(!FL){cerr<<t<<endl;}
			

			

			Face _f;
			while(t.size()){
				vector<double> H1,H2,H3;
				
				int pos = t.find('/',0);
				if(pos < 0) {
					//cerr<<"Face format error !" <<endl;
					break;
				}

				

				H1 = get_number(t.substr(0,pos));


			//	if(!FL){cerr<<t.substr(0,pos)<<'*'<<H1[0]<<endl;}


				if(H1.empty()) H1.push_back(0);
				t = t.substr(pos+1,t.size()-pos-1);
				
				
				pos = t.find('/',0);
				
				
				
				if(pos < 0) {
					cerr<<"Face format error !" <<endl;
					goto Face_break;
				}
				H2 = get_number(t.substr(0,pos)); 
				//if(!FL){cerr<<t.substr(0,pos)<<'*'<<H2[0]<<endl;}
				if(H2.empty()) H2.push_back(0);
				t = t.substr(pos+1,t.size()-pos-1);

				//cerr<<"H3"<<t<<endl;

				pos = t.find(' ',0);

				//cerr<<t.substr(0,pos)<<' '<<pos<<endl;
				
				if(pos < 0) {
					cerr<<"Face format error !" <<endl;
					goto Face_break;
				}
				H3 = get_number(t.substr(0,pos)); 
				if(H3.empty()) H3.push_back(0);
				t = t.substr(pos+1,t.size()-pos-1);
			//	cerr<<"***"<<H3[0]<<endl;
				_f.Points.push_back(Int_3(floor(H1[0]+0.1),floor(H2[0]+0.1),floor(H3[0]+0.1)));
			}	

			//FL=1;

			f.push_back(_f);
			Face_break: continue;
		}
		
	}
}

void OBJ:: Get_from_File(string s)
{
	FILE *fp = fopen(s.c_str(), "r");
	
	char ss[1050];
	int p = 0;
	vector<string> All;
	while(fgets(ss,1023,fp) != NULL) {
		  string _s = ss;
		  if(_s[_s.size()-1] == '\n') _s=_s.substr(0,_s.size()-1);
		  All.push_back(_s);
	}

	vector<string>G;
	for(auto str : All){
		if(!str.size())continue;
		if(str.size() >= 2 && str[0] =='g' && str[1]==' '){
			All_Group[p].set(G); G.clear();
			++p;
			All_Group.push_back(Group());
			All_Group[p].set_name(str.substr(2,str.size()-2));
			continue;
		}
		G.push_back(str);
	}
	All_Group[p].set(G);

	//cerr<<All_Group[0].v[1].x<<endl;
	//G.clear();
	fclose(fp); 
}

void OBJ:: Out_to_File(string s)
{
	FILE *fp = fopen(s.c_str(), "w");
	
	for(auto G : All_Group){
		if(G.name!=Group_NAME0){
			fprintf(fp,"g %s\n",(G.name).c_str());
		}
		for(int u = 1; u < G.v.size(); ++u){
			fprintf(fp,"v %.6lf %.6lf %.6lf\n",G.v[u].x,G.v[u].y,G.v[u].z,G.v[u].w);
		}
		for(int u = 1; u < G.vt.size(); ++u){
			fprintf(fp,"vt %.6lf %.6lf\n",G.vt[u].first, G.vt[u].second);
		}
		for(int u = 1; u < G.vn.size(); ++u){
			fprintf(fp,"vn %.6lf %.6lf %.6lf\n",G.vn[u].x,G.vn[u].y,G.vn[u].z);
		}
		for(auto u : G.f){
			fprintf(fp,"f ");
			for(auto v: u.Points){
				if(v.x)fprintf(fp,"%d",v.x);
				fprintf(fp,"/");
				if(v.y)fprintf(fp,"%d",v.y);
				fprintf(fp,"/");
				if(v.z)fprintf(fp,"%d",v.z);
				fprintf(fp," ");
			}
			fprintf(fp,"\n");
		}
	}

	fclose(fp); 
}

/*
//test
int main()
{
    cerr<<"Testing !"<<endl;
    OBJ a;
    a.Get_from_File("Turret_Top_OBJ.obj");
	a.Out_to_File("2.obj");
//    cout<<a.All_Group[0].v[1].z<<endl;
}
*/
