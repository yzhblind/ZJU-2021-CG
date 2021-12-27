#include  "game.h"
#include <cmath>
#include <cstring>
using namespace std;

void Game::init()
{
    glEnable(GL_DEPTH_TEST);
    scrWidth = 1920, scrHeight = 1080;
    currentTime = deltaTime = 0.0f;
    cameraState = 0;
    initSky();

    MAP_init();  // must add
}

void Game::MAP_init()
{
    // need reset Enemy_app   Home_x,Home_y
}

UPD Game::upd()
{
    return _M.upd();
}

void Game::setHome(int x, int y)
{
    _M.Home_x = x;
    _M.Home_y = y;
}

void Game::setT(int x, int y)
{
    ++_M.cnt_Tower;
    _M._T[_M.cnt_Tower].x = x;
    _M._T[_M.cnt_Tower].y = y;
    _M._T[_M.cnt_Tower].health = MAX_TowerHealth;
}

void Game::setE(int x, int y, int cd = CD_default)
{
    _M.Enemy_app[x][y].fl = 1;
    _M.Enemy_app[x][y].cd = _M.Enemy_app[x][y].counter = cd;
}

void Game::delT(int x, int y)
{
    for(int i=1;i<=_M.cnt_Tower;++i)if(_M._T[i].x==x&&_M._T[i].y==y){
        _M._T[i].save = _M._T[i].health = -1;
    }
}

void Game::delE(int x, int y)
{
    _M.Enemy_app[x][y].fl = 0;
}

void rasterize(double x1,double y1,double x2,double y2,int a[][40])
{
	if(x1>x2) swap(x1,x2),swap(y1,y2);
	int l=floor(x1+eps),r=floor(x2-eps);
	if(l==r)
	{
		int ly=floor(y1+eps),ry=floor(y2-eps);
		if(ly>ry) swap(ly,ry);
		for(int i=ly;i<=ry;i++) a[l][i]=1;
		return;
	}
	double k=(y2-y1)/(x2-x1),b=y1-k*x1;
	for(int i=l;i<=r;i++)
	{
		double yl=k*max(x1,(double)i)+b,yr=k*min(x2,(double)(i+1))+b;
		if(yl>yr) swap(yl,yr);
		int lll=floor(yl+eps),rr=floor(yr-eps);
		//cout<<yl<<" "<<yr<<endl;
		for(int j=lll;j<=rr;j++) a[i][j]=1;
	}
}


//deltaTime
void Game::logic()
{
    _MAP new_M; 

    for(int i=1;i<=_M.cnt_Enemy;++i)_M._E[i].save -= deltaTime;
    for(int i=1;i<=_M.cnt_Tower;++i)_M._T[i].save -= deltaTime;

    //home
    new_M.Home_x = _M.Home_x;
    new_M.Home_y = _M.Home_y;

    //type
    for(int i = 0; i < MAP_SIZE; ++i)
    for(int j = 0; j < MAP_SIZE; ++j) {
        new_M.ty[i][j] = _M.ty[i][j];
    }
    
    //Enemy_init
    //for(int i = 1; i <= _M.cnt_Enemy; ++i){
    //    ++new_M.cnt_Enemy;
    //    new_M._E[new_M.cnt_Enemy] = _M._E[i];
    //}

    //Enemy_app
    for(int i = 0; i < MAP_SIZE; ++i)
    for(int j = 0; j < MAP_SIZE; ++j) {
        new_M.Enemy_app[i][j] = _M.Enemy_app[i][j];
    }
    for(int i = 0; i < MAP_SIZE; ++i)
    for(int j = 0; j < MAP_SIZE; ++j) if(new_M.Enemy_app[i][j].fl){
        if(new_M.Enemy_app[i][j].counter > deltaTime) {
            new_M.Enemy_app[i][j].counter -= deltaTime;
            continue;
        }
        new_M.Enemy_app[i][j].counter = new_M.Enemy_app[i][j].cd;
        new_M.new_Enemy(i,j);
    }

    //Tower
    for(int i = 1; i <= _M.cnt_Tower; ++i){
        ++new_M.cnt_Tower;
        new_M._T[new_M.cnt_Tower] = _M._T[i];
        new_M._T[new_M.cnt_Tower].fl = 0;
    }


    //Enemy
    _M.find_init();
    for(int i=1;i<=_M.cnt_Enemy; ++i)if(_M._E[i].health > eps){
        pair<double,double> res = _M.find(_M._E[i].x,_M._E[i].y,deltaTime);

        if(res.first < -0.5) {
            new_M.new_Enemy(_M._E[i].x,_M._E[i].y,_M._E[i].health);
            continue;
        }

        new_M.new_Enemy(res.first,res.second,_M._E[i].health);
        if(sqrt((res.first-_M._E[i].x)*(res.first-_M._E[i].x)
            +(res.second-_M._E[i].y)*(res.second-_M._E[i].y))>= deltaTime -eps) {
            
        }else {
            int I = 0, ss = 1e7;
            for(int i = 1; i <= _M.cnt_Tower; ++i)if(_M._T[i].health>eps){
                if(sqrt((_M._T[i].x +0.5 -res.first) * (_M._T[i].x +0.5 -res.first) 
                    + (_M._T[i].y +0.5 -res.second) * (_M._T[i].y +0.5 -res.second)) < ss){
                    I = i;
                    ss = sqrt((_M._T[i].x +0.5 -res.first) * (_M._T[i].x +0.5 -res.first) 
                    + (_M._T[i].y +0.5 -res.second) * (_M._T[i].y +0.5 -res.second));
                }
            }
            new_M._T[I].health -= max((double)0,(double)deltaTime - ss); //吃塔
            if(new_M._T[I].health < eps){  //吃席
                new_M._T[I].save = SAVE_TIME;
            }
        }
    } else if(_M._E[i].save > eps){
        ++new_M.cnt_Enemy;
        new_M._E[new_M.cnt_Enemy] = _M._E[i];
    }

    //炮击
    int e[40][40];
    
    new_M.Vector.clear();

    for(int i = 1; i<=new_M.cnt_Tower; ++i)if(new_M._T[i].health > eps){

        int ss = 1000, _j = 0;
        for(int j=1;j<=new_M.cnt_Enemy; ++j)if(new_M._E[j].health > eps){
            memset(e,0,sizeof e);
            rasterize(new_M._T[i].x+0.5,new_M._T[i].y+0.5,new_M._E[j].x, new_M._E[j].y,e);
            int fl = 0;
            for(int i=0; i<MAP_SIZE; ++i)
            for(int j=0; j<MAP_SIZE; ++j)if(new_M.ty[i][j] && e[i][j]){
                fl = 1;
            }
            if(!fl) {
                double J = atan2(
                    new_M._E[j].y-(new_M._T[i].y +0.5 ),
                    new_M._E[j].x-(new_M._T[i].x +0.5 ));
                J *= 180 / acos(-1);
                if(J < 0) {
                    J += 360;
                }

                double t = abs(J-new_M._T[i].J);
                t = min(t,180-t);
                if(t < ss){
                    ss = t; 
                    _j = j;
                }
            }
        }

        if(!_j) continue;
        if(ss <= deltaTime*VJ){
            double J = atan2(new_M._E[_j].y-(new_M._T[i].y +0.5 ),new_M._E[_j].x-(new_M._T[i].x +0.5 ));
            J *= 180 / acos(-1);
            new_M._T[i].J = J;
            new_M.Vector.push_back((P){_j, i, deltaTime - ss/VJ});
            //new_M.Hit(_j, i, deltaTime - ss/VJ);
        } else{
            double J = atan2(new_M._E[_j].y-(new_M._T[i].y +0.5 ),new_M._E[_j].x-(new_M._T[i].x +0.5 ));
            J *= 180 / acos(-1);
            ss -= deltaTime*VJ;
            if(J > new_M._T[i].J){
                if(J-new_M._T[i].J < 180){
                    new_M._T[i].J += ss;
                    if(new_M._T[i].J > 360) new_M._T[i].J-=360;
                } else {
                    new_M._T[i].J -= ss;
                    if(new_M._T[i].J < 0) new_M._T[i].J+=360;
                }
            } else {
                if(new_M._T[i].J-J < 180){
                    new_M._T[i].J -= ss;
                    if(new_M._T[i].J < 0) new_M._T[i].J+=360;
                } else {
                    new_M._T[i].J += ss;
                    if(new_M._T[i].J > 360) new_M._T[i].J-=360;
                }
            }
        }
    }

    for(auto u:new_M.Vector){
        new_M.Hit(u);
    }
    
   
    //update
    _M = new_M;
}
