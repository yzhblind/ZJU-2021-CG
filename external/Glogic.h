#include <utility>
#include <set>
#include <unordered_set>
#include <vector>
#define QIANG 1
#define TA 2
#define MAX_TowerHealth 5
#define MAX_EnemyHealth 5
#define MAP_SIZE 30
#define eps 1e-7
#define SPLIT 5
#define PZ (int)(0.76*SPLIT) //碰撞箱
#define SAVE_TIME 0.5 //死亡后粒子持续
#define VJ 90  //塔转速
#define CD_default 5  //出怪

class _Tower;
class _Enemy;
class _MAP;

class _Tower {
public:
    int x, y;
    double J;  //0  +x
    double health;
    int fl; //是否发射
    double save;
    
    _Tower (int _x = 0, int _y = 0, double _h = MAX_TowerHealth,double _J=0,int _fl =0)
        :x(_x), y(_y), health(_h),J(_J),fl(_fl){}
};

class _Enemy {
public:
    double x, y, health;
    double save;
    _Enemy (int _x = 0, int _y = 0, double _h = MAX_EnemyHealth) :x(_x), y(_y), health(_h){}
};

class _Enemy_app {
public:
    int fl;
    double counter, cd;
    _Enemy_app() {fl = 0;}
    void operator = (_Enemy_app a) {
        fl = a.fl;
        counter = a.counter;
        cd = a.cd;
    }
};

class P {
public:
    int dst, src;
    double del;
};

class UPD {

/*
ty = 1  是墙
T 塔   x,y 位置   J 角度   fl 是否在发射  health生命  save死亡后还能存活多久
E 病毒 x,y 位置   health生命  save死亡后还能存活多久
Home 家
app 出怪点
*/

public:
    int ty[MAP_SIZE + 5][MAP_SIZE + 5];  
    vector<_Tower>T;
    vector<_Enemy>E;
    vector<pair<int,int>> app;
    int Home_x,Home_y; 
};

class _MAP {  
public:
    int ty[MAP_SIZE + 5][MAP_SIZE + 5]; // 1 qiang
    //int point[MAP_SIZE + 5][MAP_SIZE + 5]; 
    _Tower _T[MAP_SIZE * MAP_SIZE + 10]; 
    _Enemy _E[4096]; 
    _Enemy_app Enemy_app[MAP_SIZE + 5][MAP_SIZE + 5];  
    int cnt_Tower, cnt_Enemy;
    int Home_x,Home_y; 

    int di[MAP_SIZE*4*SPLIT+10][MAP_SIZE*4*SPLIT+10],di2[MAP_SIZE*4*SPLIT+10][MAP_SIZE*4*SPLIT+10];
    int a[MAP_SIZE*4*SPLIT+10][MAP_SIZE*4*SPLIT+10];
    vector<P>Vector;

    _MAP () {
        for(int i = 0; i < MAP_SIZE; ++i)
        for(int j = 0; j < MAP_SIZE; ++j) ty[i][j] = 0;
        cnt_Tower = cnt_Enemy = 0;
        Home_x = Home_y = MAP_SIZE - 1;
    }
    int new_Enemy(double x, double y, double counter = MAX_EnemyHealth);
    void find_init();
    pair<double,double> find(double x,double y,double deltaTime);
    void Hit(P x);
    UPD upd();
};
