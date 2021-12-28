#include "Glogic.h"
#include <cstring>
#include <cmath>
using namespace std;
int X[MAP_SIZE * 4 * SPLIT * MAP_SIZE * 4 * SPLIT + 10], Y[MAP_SIZE * 4 * SPLIT * MAP_SIZE * 4 * SPLIT + 10];

int _MAP::new_Enemy(double x, double y, double counter)
{
    ++cnt_Enemy;
    _E[cnt_Enemy].x = x;
    _E[cnt_Enemy].y = y;
    _E[cnt_Enemy].health = counter;

    return cnt_Enemy;
}

void _MAP::find_init()
{
    for (int i = 0;i < MAP_SIZE * 4 * SPLIT + 10;i++)
        for (int j = 0;j < MAP_SIZE * 4 * SPLIT + 10;j++)a[i][j] = 0;

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (ty[i][j] == QIANG) {
                for (int k = i * 4 * SPLIT - PZ; k <= (i * 4 + 4) * SPLIT - 1 + PZ; ++k)
                    for (int l = j * 4 * SPLIT - PZ; l <= (j * 4 + 4) * SPLIT - 1 + PZ; ++l) if (k >= 0 && l >= 0)a[k][l] = 1;
            }
        }

    for (int i = 1; i <= cnt_Tower; ++i) if (_T[i].health >= eps) {
        for (int k = (_T[i].x * 4 + 1) * SPLIT - PZ; k <= (_T[i].x * 4 + 3) * SPLIT - 1 + PZ; ++k)
            for (int l = (_T[i].y * 4 + 1) * SPLIT - PZ; l <= (_T[i].y * 4 + 3) * SPLIT - 1 + PZ; ++l) if (k >= 0 && l >= 0)a[k][l] = 1;
    }

    //// When a[Home] is 1
    for (int i = 0;i < MAP_SIZE * 4 * SPLIT;++i)
        for (int j = 0;j < MAP_SIZE * 4 * SPLIT;++j) di[i][j] = 9999;

    int qn;
    X[qn = 1] = (Home_x * 4 + 1.5) * SPLIT;
    Y[1] = (Home_y * 4 + 1.5) * SPLIT;
    di[X[1]][Y[1]] = 0;

    for (int q1 = 1; q1 <= qn; ++q1) {
        int x = X[q1], y = Y[q1];
        if (x > 0 && !a[x - 1][y]) {
            if (di[x - 1][y] > di[x][y] + 1) {
                di[x - 1][y] = di[x][y] + 1;
                X[++qn] = x - 1;
                Y[qn] = y;
            }
        }
        if (x < MAP_SIZE - 1 && !a[x + 1][y]) {
            if (di[x + 1][y] > di[x][y] + 1) {
                di[x + 1][y] = di[x][y] + 1;
                X[++qn] = x + 1;
                Y[qn] = y;
            }
        }
        if (y > 0 && !a[x][y - 1]) {
            if (di[x][y - 1] > di[x][y] + 1) {
                di[x][y - 1] = di[x][y] + 1;
                X[++qn] = x;
                Y[qn] = y - 1;
            }
        }
        if (y < MAP_SIZE - 1 && !a[x][y + 1]) {
            if (di[x][y + 1] > di[x][y] + 1) {
                di[x][y + 1] = di[x][y] + 1;
                X[++qn] = x;
                Y[qn] = y + 1;
            }
        }
    }


    for (int i = 0;i < MAP_SIZE * 4 * SPLIT;++i)
        for (int j = 0;j < MAP_SIZE * 4 * SPLIT;++j) di2[i][j] = 9999;
    qn = 0;
    for (int i = 1; i <= cnt_Tower; ++i) if (_T[i].health >= eps) {
        X[++qn] = (_T[i].x * 4 + 1.5) * SPLIT;
        Y[qn] = (_T[i].y * 4 + 1.5) * SPLIT;
        di2[X[qn]][Y[qn]] = 0;
    }

    for (int q1 = 1; q1 <= qn; ++q1) {
        int x = X[q1], y = Y[q1];
        if (x > 0 && !a[x - 1][y]) {
            if (di2[x - 1][y] > di2[x][y] + 1) {
                di2[x - 1][y] = di2[x][y] + 1;
                X[++qn] = x - 1;
                Y[qn] = y;
            }
        }
        if (x < MAP_SIZE - 1 && !a[x + 1][y]) {
            if (di2[x + 1][y] > di2[x][y] + 1) {
                di2[x + 1][y] = di2[x][y] + 1;
                X[++qn] = x + 1;
                Y[qn] = y;
            }
        }
        if (y > 0 && !a[x][y - 1]) {
            if (di2[x][y - 1] > di2[x][y] + 1) {
                di2[x][y - 1] = di2[x][y] + 1;
                X[++qn] = x;
                Y[qn] = y - 1;
            }
        }
        if (y < MAP_SIZE - 1 && !a[x][y + 1]) {
            if (di2[x][y + 1] > di2[x][y] + 1) {
                di2[x][y + 1] = di2[x][y] + 1;
                X[++qn] = x;
                Y[qn] = y + 1;
            }
        }
    }
}

pair<double, double> _MAP::find(double x, double y, double deltaTime)
{
    int X = x * 4 * SPLIT + eps;
    int Y = y * 4 * SPLIT + eps;

    if (sqrt((x - (Home_x + 0.5)) * (x - (Home_x + 0.5)) + (y - (Home_y + 0.5)) * (y - (Home_y + 0.5))) <= sqrt(2)) return make_pair(Home_x, Home_y);
    if (a[X][Y])return make_pair(x, y);

    if (di[X][Y] < 999) {
        int t = rand() & 1;
        double _x = x, _y = y;
        if (fabs(x * 4 * SPLIT - X) < eps && fabs(y * 4 * SPLIT - Y) < eps) {
            if (di[X][Y + 1] < di[X][Y] && di[X + 1][Y] < di[X][Y]) {
                if (t) _x += deltaTime; else _y += deltaTime;
            }
            else
                if (di[X][Y + 1] < di[X][Y] && X>0 && di[X - 1][Y] < di[X][Y]) {
                    if (t) _x -= deltaTime; else _y += deltaTime;
                }
                else
                    if (Y > 0 && di[X][Y - 1] < di[X][Y] && di[X + 1][Y] < di[X][Y]) {
                        if (t) _x += deltaTime; else _y -= deltaTime;
                    }
                    else
                        if (Y > 0 && di[X][Y - 1] < di[X][Y] && X>0 && di[X - 1][Y] < di[X][Y]) {
                            if (t) _x -= deltaTime; else _y -= deltaTime;
                        }
                        else {
                            if (di[X + 1][Y] < di[X][Y]) {
                                _x += deltaTime;
                            }
                            else
                                if (X > 0 && di[X - 1][Y] < di[X][Y]) {
                                    _x -= deltaTime;
                                }
                                else
                                    if (di[X][Y + 1] < di[X][Y]) {
                                        _y += deltaTime;
                                    }
                                    else
                                        if (Y > 0 && di[X][Y - 1] < di[X][Y]) {
                                            _y -= deltaTime;
                                        }
                        }
            return make_pair(_x, _y);
        }
        if (fabs(x * 4 * SPLIT - X) < eps) {

            if (di[X][Y + 1] < di[X][Y]) {
                _y += deltaTime;
                _y = min(_y, (Y + 1) / 4.0 / SPLIT);
            }
            else
                if (Y > 0 && di[X][Y - 1] < di[X][Y]) {
                    _y -= deltaTime;
                    _y = max(_y, (Y - 1) / 4.0 / SPLIT);
                }
            return make_pair(_x, _y);
        }
        if (di[X + 1][Y] < di[X][Y]) {
            _x += deltaTime;
            _x = min(_x, (X + 1) / 4.0 / SPLIT);
        }
        else
            if (X > 0 && di[X - 1][Y] < di[X][Y]) {
                _x -= deltaTime;
                _x = max(_x, (X - 1) / 4.0 / SPLIT);
            }
        return make_pair(_x, _y);
    }
    if (di2[X][Y] > 999) {
        return make_pair(-1, -1);
    }

    int t = rand() & 1;
    double _x = x, _y = y;
    if (fabs(x * 4 * SPLIT - X) < eps && fabs(y * 4 * SPLIT - Y) < eps) {
        if (di2[X][Y + 1] < di2[X][Y] && di2[X + 1][Y] < di2[X][Y]) {
            if (t) _x += deltaTime; else _y += deltaTime;
        }
        else
            if (di2[X][Y + 1] < di2[X][Y] && X>0 && di2[X - 1][Y] < di2[X][Y]) {
                if (t) _x -= deltaTime; else _y += deltaTime;
            }
            else
                if (Y > 0 && di2[X][Y - 1] < di2[X][Y] && di2[X + 1][Y] < di2[X][Y]) {
                    if (t) _x += deltaTime; else _y -= deltaTime;
                }
                else
                    if (Y > 0 && di2[X][Y - 1] < di2[X][Y] && X>0 && di2[X - 1][Y] < di2[X][Y]) {
                        if (t) _x -= deltaTime; else _y -= deltaTime;
                    }
                    else {
                        if (di2[X + 1][Y] < di2[X][Y]) {
                            _x += deltaTime;
                        }
                        else
                            if (X > 0 && di2[X - 1][Y] < di2[X][Y]) {
                                _x -= deltaTime;
                            }
                            else
                                if (di2[X][Y + 1] < di2[X][Y]) {
                                    _y += deltaTime;
                                }
                                else
                                    if (Y > 0 && di2[X][Y - 1] < di2[X][Y]) {
                                        _y -= deltaTime;
                                    }
                    }
        return make_pair(_x, _y);
    }
    if (fabs(x * 4 * SPLIT - X) < eps) {

        if (di2[X][Y + 1] < di2[X][Y]) {
            _y += deltaTime;
            _y = min(_y, (Y + 1) / 4.0 / SPLIT);
        }
        else
            if (Y > 0 && di2[X][Y - 1] < di2[X][Y]) {
                _y -= deltaTime;
                _y = max(_y, (Y - 1) / 4.0 / SPLIT);
            }
        return make_pair(_x, _y);
    }
    if (di2[X + 1][Y] < di2[X][Y]) {
        _x += deltaTime;
        _x = min(_x, (X + 1) / 4.0 / SPLIT);
    }
    else
        if (X > 0 && di2[X - 1][Y] < di2[X][Y]) {
            _x -= deltaTime;
            _x = max(_x, (X - 1) / 4.0 / SPLIT);
        }
    return make_pair(_x, _y);
}

void _MAP::Hit(P x)
{
    // 从 x.src 打 x.dst 
    _T[x.src].fl = 1;
    _T[x.src].dstx = _E[x.dst].x;
    _T[x.src].dsty = _E[x.dst].y;
    if (_E[x.dst].health > eps && _E[x.dst].health < x.del) {
        //传出 x.dst die  //可能不需要
        _E[x.dst].save = SAVE_TIME;
    }
    _E[x.dst].health -= x.del;
}

UPD _MAP::upd()
{
    UPD ret;
    for (int i = 0;i < MAP_SIZE;++i)
        for (int j = 0;j < MAP_SIZE;++j) ret.ty[i][j] = ty[i][j];
    ret.Home_x = Home_x;
    ret.Home_y = Home_y;
    ret.END = 0;
    for (int i = 1;i <= cnt_Enemy;++i)if (_E[i].health > eps || _E[i].save > eps)ret.E.push_back(_E[i]);
    for (int i = 1;i <= cnt_Tower;++i)if (_T[i].health > eps || _T[i].save > eps)ret.T.push_back(_T[i]);

    for (int i = 0;i < MAP_SIZE;++i)
        for (int j = 0;j < MAP_SIZE;++j) if (Enemy_app[i][j].fl) ret.app.push_back(make_pair(i, j));

    for (int i = 1;i <= cnt_Enemy;++i)if (_E[i].health > eps) {
        double _x = abs(Home_x + 0.5 - _E[i].x);
        double _y = abs(Home_y + 0.5 - _E[i].y);
        if (_x * _x + _y * _y <= 2 + eps) {
            ret.END = 1;
        }
    }

    return ret;
}
