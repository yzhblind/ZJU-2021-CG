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
#define PZ (int)(0.76*SPLIT)

#define VJ 90

class _Tower;
class _Enemy;
class _MAP;

class _Tower {
public:
    int x, y;
    double J;  //0  +x
    double health;
    _Tower (int _x = 0, int _y = 0, double _h = MAX_TowerHealth,double _J=0) :x(_x), y(_y), health(_h),J(_J){}
};

class _Enemy {
public:
    double x, y, health;
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
};