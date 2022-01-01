#include <game.h>
#include <basic.h>

#include <cmath>
#include <cstring>
#include <random>
#include <algorithm>

using namespace glm;
using namespace std;

void Game::init()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    scrWidth = 1920, scrHeight = 1080;
    currentTime = deltaTime = 0.0f;
    cameraState = 0;
    initLight();
    initShadow();
    initModel();
    initLine();
    initSky();
    MAP_init();
}

void Game::initLine()
{
    lineShader = getShaderProgram("../glsl/line.vs", "../glsl/line.fs");
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
}

void Game::initShadow()
{
    shadowShader = getShaderProgram("../glsl/shadow.vs", "../glsl/shadow.fs");
    shadowWidth = 8192, shadowHeight = 4096;
    glGenFramebuffers(1, &shadowFBO);
    glGenTextures(1, &shadowMap);

    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::initLight()
{
    en_light1 = false;
    light0.direction = vec3(1.0f, -1.0f, 1.0f);
    light0.ambient = vec3(0.15f);
    light0.diffuse = vec3(1.0f);
    light0.specular = vec3(1.0f);
    light1.ambient = vec3(0.0f, 0.0f, 0.2f);
    light1.diffuse = vec3(0.0f, 0.0f, 1.0f);
    light1.specular = vec3(0.0f, 0.0f, 1.0f);
    light1.constant = 1.0f;
    light1.linear = 0.7f;
    light1.quadratic = 1.8f;
}

void Game::upd()
{
    data2draw = _M.upd();
    if (data2draw.END == 1)
    {
        state = 1;
    }
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

void Game::setE(int x, int y, int cd)
{
    _M.Enemy_app[x][y].fl = 1;
    _M.Enemy_app[x][y].cd = _M.Enemy_app[x][y].counter = cd;
}

void Game::delT(int x, int y)
{
    for (int i = 1; i <= _M.cnt_Tower; ++i)
        if (_M._T[i].x == x && _M._T[i].y == y)
        {
            _M._T[i].save = _M._T[i].health = -1;
        }
}

void Game::delE(int x, int y)
{
    _M.Enemy_app[x][y].fl = 0;
}

void rasterize(double x1, double y1, double x2, double y2, int a[][40])
{
    if (x1 > x2)
        swap(x1, x2), swap(y1, y2);
    int l = floor(x1 + eps), r = floor(x2 - eps);
    if (l == r)
    {
        int ly = floor(y1 + eps), ry = floor(y2 - eps);
        if (ly > ry)
            swap(ly, ry);
        for (int i = ly; i <= ry; i++)
            a[l][i] = 1;
        return;
    }
    double k = (y2 - y1) / (x2 - x1), b = y1 - k * x1;
    for (int i = l; i <= r; i++)
    {
        double yl = k * std::max(x1, (double)i) + b, yr = k * std::min(x2, (double)(i + 1)) + b;
        if (yl > yr)
            swap(yl, yr);
        int lll = floor(yl + eps), rr = floor(yr - eps);
        // cout<<yl<<" "<<yr<<endl;
        for (int j = lll; j <= rr; j++)
            a[i][j] = 1;
    }
}

_MAP new_M;
void Game::edit()
{
    // switchState();
    char op;
    std::cout << "please input opeation: wall: w, tower: t, entry: e " << std::endl;
    std::cin >> op;
    if (op == 'w')
    {
        int x, y;
        std::cout << "please input x, y:";
        std::cin >> x >> y;
        setWall(x, y);
    }
    else if (op == 't')
    {
        int x, y;
        std::cout << "please input x, y:";
        std::cin >> x >> y;
        setT(x, y);
    }
    else if (op == 'e')
    {
        int x, y;
        std::cout << "please input x, y:";
        std::cin >> x >> y;
        setE(x, y);
    }
    // switchState();
}
// deltaTime
void Game::logic()
{
    if (deltaTime > 0.05)
        deltaTime = 0.05;

    new_M.cnt_Enemy = new_M.cnt_Tower = 0;

    for (int i = 1; i <= _M.cnt_Enemy; ++i)
        _M._E[i].save -= deltaTime;
    for (int i = 1; i <= _M.cnt_Tower; ++i)
        _M._T[i].save -= deltaTime;

    // home
    new_M.Home_x = _M.Home_x;
    new_M.Home_y = _M.Home_y;

    // type
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            new_M.ty[i][j] = _M.ty[i][j];
        }

    // Enemy_init
    // for(int i = 1; i <= _M.cnt_Enemy; ++i){
    //     ++new_M.cnt_Enemy;
    //     new_M._E[new_M.cnt_Enemy] = _M._E[i];
    // }

    // Enemy_app
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            new_M.Enemy_app[i][j] = _M.Enemy_app[i][j];
        }
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            if (new_M.Enemy_app[i][j].fl)
            {
                if (new_M.Enemy_app[i][j].counter > deltaTime)
                {
                    new_M.Enemy_app[i][j].counter -= deltaTime;
                    continue;
                }
                new_M.Enemy_app[i][j].counter = new_M.Enemy_app[i][j].cd;
                new_M.new_Enemy(i + 0.5, j + 0.5);
            }

    // Tower
    for (int i = 1; i <= _M.cnt_Tower; ++i)
    {
        ++new_M.cnt_Tower;
        new_M._T[new_M.cnt_Tower] = _M._T[i];
        new_M._T[new_M.cnt_Tower].fl = 0;
    }

    // Enemy
    _M.find_init();
    for (int i = 1; i <= _M.cnt_Enemy; ++i)
        if (_M._E[i].health > eps)
        {
            _Enemy sw = _M._E[i];
            pair<double, double> res = _M.find(i, deltaTime);

            int _FL = 0;
            for (int j = 1; j <= _M.cnt_Enemy; ++j)
                if (i != j)
                    if (sqrt((res.first - _M._E[j].x) * (res.first - _M._E[j].x) + (res.second - _M._E[j].y) * (res.second - _M._E[j].y)) < 2 * ER)
                        _FL = 1;

            if (res.first < -0.5)
            {
                new_M.new_Enemy(_M._E[i].x, _M._E[i].y, _M._E[i].health);
                continue;
            }

            if (_FL && PZ_Enemy)
                new_M.new_Enemy(sw.x, sw.y, sw.health, sw.flx, sw.fly), res.first = sw.x, res.second = sw.y;
            else
                new_M.new_Enemy(res.first, res.second, _M._E[i].health, _M._E[i].flx, _M._E[i].fly);
            if (sqrt((res.first - _M._E[i].x) * (res.first - _M._E[i].x) + (res.second - _M._E[i].y) * (res.second - _M._E[i].y)) >= deltaTime - eps)
            {
            }
            else
            {
                int I = 0, ss = 1e7;
                for (int i = 1; i <= _M.cnt_Tower; ++i)
                    if (_M._T[i].health > eps)
                    {
                        if (sqrt((_M._T[i].x + 0.5 - res.first) * (_M._T[i].x + 0.5 - res.first) + (_M._T[i].y + 0.5 - res.second) * (_M._T[i].y + 0.5 - res.second)) < ss)
                        {
                            I = i;
                            ss = sqrt((_M._T[i].x + 0.5 - res.first) * (_M._T[i].x + 0.5 - res.first) + (_M._T[i].y + 0.5 - res.second) * (_M._T[i].y + 0.5 - res.second));
                        }
                    }
                new_M._T[I].health -= std::max((double)0, (double)deltaTime - ss); //吃塔
                if (new_M._T[I].health < eps)
                { //吃席
                    new_M._T[I].save = SAVE_TIME;
                }
            }
        }
        else if (_M._E[i].save > eps)
        {
            ++new_M.cnt_Enemy;
            new_M._E[new_M.cnt_Enemy] = _M._E[i];
        }

    //炮击
    int e[40][40];

    new_M.Vector.clear();

    for (int i = 1; i <= new_M.cnt_Tower; ++i)
        if (new_M._T[i].health > eps)
        {

            double ss = 1000;
            double Dis = 1000;
            int _j = 0;
            for (int j = 1; j <= new_M.cnt_Enemy; ++j)
                if (new_M._E[j].health > eps)
                {
                    memset(e, 0, sizeof e);
                    rasterize(new_M._T[i].x + 0.5, new_M._T[i].y + 0.5, new_M._E[j].x, new_M._E[j].y, e);
                    int fl = 0;
                    for (int i = 0; i < MAP_SIZE; ++i)
                        for (int j = 0; j < MAP_SIZE; ++j)
                            if (new_M.ty[i][j] && e[i][j])
                            {
                                fl = 1;
                            }
                    if (!fl)
                    {
                        double dis = sqrt((new_M.Home_x + 0.5 - new_M._E[j].x) * (new_M.Home_x + 0.5 - new_M._E[j].x) + (new_M.Home_y + 0.5 - new_M._E[j].y) * (new_M.Home_y + 0.5 - new_M._E[j].y));

                        double J = atan2(
                            new_M._E[j].y - (new_M._T[i].y + 0.5),
                            new_M._E[j].x - (new_M._T[i].x + 0.5));
                        J *= 180 / acos(-1);
                        if (J < 0)
                        {
                            J += 360;
                        }
                        double t = abs(J - new_M._T[i].J);
                        t = std::min(t, 360 - t);

                        if (Dis < _WARNING)
                        {
                            if (dis < Dis)
                            {
                                Dis = dis;
                                _j = j;
                                ss = t;
                            }
                        }
                        else
                        {
                            if (dis < _WARNING)
                            {
                                Dis = dis;
                                _j = j;
                                ss = t;
                            }
                            else if (t < ss)
                            {
                                Dis = dis;
                                ss = t;
                                _j = j;
                            }
                        }
                    }
                }

            if (!_j)
                continue;
            if (ss <= deltaTime * VJ)
            {
                double J = atan2(new_M._E[_j].y - (new_M._T[i].y + 0.5), new_M._E[_j].x - (new_M._T[i].x + 0.5));
                J *= 180 / acos(-1);
                if (J < 0)
                {
                    J += 360;
                }
                new_M._T[i].J = J;

                P u;
                u.dst = _j;
                u.src = i;
                u.del = deltaTime - ss / VJ;
                new_M.Vector.push_back(u);
                // new_M.Vector.push_back((P) ( _j, i, (double)deltaTime - ss / VJ ));

                // new_M.Hit(_j, i, deltaTime - ss/VJ);
            }
            else
            {
                double J = atan2(new_M._E[_j].y - (new_M._T[i].y + 0.5), new_M._E[_j].x - (new_M._T[i].x + 0.5));
                J *= 180 / acos(-1);
                if (J < 0)
                {
                    J += 360;
                }
                ss = deltaTime * VJ;
                if (J > new_M._T[i].J)
                {
                    if (J - new_M._T[i].J < 180)
                    {
                        new_M._T[i].J += ss;
                        if (new_M._T[i].J > 360)
                            new_M._T[i].J -= 360;
                    }
                    else
                    {
                        new_M._T[i].J -= ss;
                        if (new_M._T[i].J < 0)
                            new_M._T[i].J += 360;
                    }
                }
                else
                {
                    if (new_M._T[i].J - J < 180)
                    {
                        new_M._T[i].J -= ss;
                        if (new_M._T[i].J < 0)
                            new_M._T[i].J += 360;
                    }
                    else
                    {
                        new_M._T[i].J += ss;
                        if (new_M._T[i].J > 360)
                            new_M._T[i].J -= 360;
                    }
                }
            }
        }

    for (auto u : new_M.Vector)
    {
        new_M.Hit(u);
    }

    // update
    _M = new_M;
    upd();
    //  cerr << "START" << endl;
    //  for (int i = 1; i <= _M.cnt_Enemy;++i)cerr << _M._E[i].x << ' ' << _M._E[i].y << ' '<< _M._E[i].health<<endl;
}
// float baseAngle = 0.0f;
void Game::MAP_init()
{
    setHome(0, 10);
    for (int i = 1; i < 7; ++i)
        for (int j = 0; j < 20; ++j)
            if (((i + j) & 1) == 0)
                setT(i, j);
    for (int i = 11; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            if ((j)&1)
                setE(i, j);
    setWall(11, 9);
    setWall(11, 11);
    setWall(10, 9);
    setWall(10, 11);
    setWall(9, 9);
    setWall(9, 11);
    setWall(9, 8);
    setWall(9, 12);

    setWall(9, 6);
    setWall(9, 14);
    setWall(9, 5);
    setWall(9, 15);
    setWall(9, 4);
    setWall(9, 16);
    // setWall(0, 1);
    /* setHome(10, 10);
     setWall(11, 5);
     setWall(11, 6);
     setWall(11, 7);
     setWall(11, 8);
     setWall(11, 9);
     setWall(11, 10);
     setWall(11, 11);
     setWall(10, 11);
     setWall(9, 11);
     setWall(9, 10);
     setWall(9, 9);
     setWall(9, 8);
     setWall(9, 7);
     setWall(9, 6);
     setE(18, 18);
     setE(1, 18);
     setE(18, 1);
     // setE(10, 10);
     setT(6, 6);
     // setT(5, 7);
     setT(4, 8);
     // setT(7, 5);
     setT(18, 16);
     setT(10, 9);
     */
}

void Game::drawScene(const glm::mat4 &projection, const glm::mat4 &view, ShaderProgram &prgm)
{
    // baseAngle += deltaTime * 5;
    // modelStack.rotate(baseAngle, vec3(0.0f, 1.0f, 0.0f));
    // modelStack.push();
    // for (int i = 0; i < 10; i += 2)
    // {
    //     for (int j = 0; j < 20; j += 2)
    //     {
    //         modelStack.push();
    //         modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
    //         drawModel(projection, view, turret, prgm);
    //         modelStack.pop();
    //     }
    // }
    modelStack.push();
    modelStack.translate(vec3(-0.3f, -0.32f, 0.0f));
    for (int i = 0; i < data2draw.T.size(); ++i)
    {
        if (data2draw.T[i].health > 0)
        {
            modelStack.push();
            modelStack.rotate(-data2draw.T[i].J + 180.0f, vec3(0.0f, 1.0f, 0.0f));
            modelStack.translate(vec3((float)(data2draw.T[i].x) * 4, 0.0f, (float)(data2draw.T[i].y) * 4));
            drawModel(projection, view, turret, prgm);
            modelStack.pop();
        }
    }
    // modelStack.pop();
    modelStack.pop();
    // for (int i = 10; i < 20; i += 2)
    // {
    //     for (int j = 0; j < 20; j += 2)
    //     {
    //         modelStack.push();
    //         modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
    //         drawModel(projection, view, virus, prgm);
    //         modelStack.pop();
    //     }
    // }

    modelStack.push();
    // modelStack.scale(vec3(0.8f));
    for (int i = 0; i < data2draw.E.size(); ++i)
    {
        if (data2draw.E[i].health > 0)
        {
            modelStack.push();
            modelStack.scale(vec3(std::max((float)(data2draw.E[i].health / MAX_EnemyHealth), 0.4f)));
            modelStack.translate(vec3((float)(data2draw.E[i].x - 0.5) * 4, 0.25f, (float)(data2draw.E[i].y - 0.5) * 4));
            drawModel(projection, view, virus, prgm);
            modelStack.pop();
        }
    }
    modelStack.pop();

    // modelStack.push();
    // modelStack.translate(vec3(12.0f, 4.0f, 12.0f));
    // drawModel(projection, view, box, prgm);
    // modelStack.pop();

    modelStack.push();
    modelStack.scale(vec3(0.5f));
    // modelStack.mul(rotateMat);
    en_light1 = true;
    light1.position = vec3(data2draw.Home_x * 4.0f, 0.0f, data2draw.Home_y * 4.0f);
    modelStack.translate(light1.position);
    drawModel(projection, view, sphere, prgm);
    modelStack.pop();

    modelStack.push();
    modelStack.scale(vec3(2.0f));
    modelStack.translate(vec3(0.0f, 1.0f, 0.0f));
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            if (data2draw.ty[i][j] == 1)
            {
                modelStack.push();
                modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
                drawModel(projection, view, box, prgm);
                modelStack.pop();
            }
        }
    }
    modelStack.pop();

    modelStack.push();
    modelStack.scale(vec3(2.0f));
    modelStack.translate(vec3(0.0f, -3.0f, 0.0f));
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            modelStack.push();
            modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
            if (((i + j) & 1) == 0)
                drawModel(projection, view, box, prgm);
            else
                drawModel(projection, view, box2, prgm);
            modelStack.pop();
        }
    }
    modelStack.pop();
}
// projection & view is not in use here
void Game::shadowGen(const glm::mat4 &projection, const glm::mat4 &view)
{
    Camera &c = cam[cameraState];
    vec3 viewCenter = vec3(40.0f, 0.0f, 40.0f);
    float orthoHeight = normalize(light0.direction).y < -0.7 ? 60.0f : 35.0f;
    float orthoWidth = 60.0f;
    if (c.front.y < -0.7 && c.pos.y < 15)
    {
        viewCenter = vec3(c.pos.x, 0.0f, c.pos.z);
        if (c.front.y < -0.8 && c.pos.y < 10)
        {
            orthoHeight = 10.0f;
            orthoWidth = 20.0f;
        }
        else
        {
            orthoWidth = glm::clamp(20.0f + 400.0f * (c.front.y + 0.8f) + 8.0f * (c.pos.y - 10.0f), 20.0f, 60.0f);
            orthoHeight = glm::clamp(10.0f + std::max((orthoHeight - 10.0f) * 10.0f * (c.front.y + 0.8f), 0.0f) + std::max((orthoHeight - 10.0f) / 5.0f * (c.pos.y - 10.0f), 0.0f), 10.0f, orthoHeight);
        }
    }

    mat4 lightProjection = ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, 0.1f, 128.0f);
    vec3 lightPos = viewCenter - 50.0f * normalize(light0.direction);
    mat4 lightView = lookAt(lightPos, viewCenter, vec3(0.0f, 1.0f, 0.0f));
    mat4 lightSpaceMatrix = lightProjection * lightView;
    shadowShader.use();
    shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    drawScene(projection, view, shadowShader);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, scrWidth, scrHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    normalShader.use();
    normalShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    normalShader.setInt("shadowMap", 0);
}

float rand_f() { return (((float)rand() / (float)RAND_MAX) - 0.5); }

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 view = cam[cameraState].getViewMatrix();
    mat4 projection = perspective(radians(cam[cameraState].zoom), scrWidth / scrHeight, 0.1f, 200.0f);

    // rotateMat = rotate(rotateMat, radians(deltaTime * 10), vec3(rand_f(), 0.0f, rand_f()));

    shadowGen(projection, view);

    drawScene(projection, view, normalShader);

    drawLine(projection, view, lineShader);

    updateSky(projection, view);
    drawSky();
}

void Game::drawLine(const glm::mat4 &projection, const glm::mat4 &view, ShaderProgram &prgm)
{
    lines.clear();
    for (int i = 0; i < data2draw.T.size(); ++i)
    {
        if (data2draw.T[i].fl)
        {
            lines.push_back(vec3((float)data2draw.T[i].x * 4, 0.25f, (float)data2draw.T[i].y * 4));
            lines.push_back(vec3((float)(data2draw.T[i].dstx - 0.5) * 4, 0.25f, (float)(data2draw.T[i].dsty - 0.5) * 4));
        }
    }
    if (lines.size() == 0)
        return;
    prgm.use();
    prgm.setMat4("projection", projection);
    prgm.setMat4("view", view);
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(vec3), &lines[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
    glDrawArrays(GL_LINES, 0, lines.size());
}

void Game::processKeyMove(bool w, bool a, bool s, bool d)
{
    if (w && !s)
        cam[cameraState].move(cameraState == 0 ? FORWARD : LEVEL_FORWARD, deltaTime);
    else if (!w && s)
        cam[cameraState].move(cameraState == 0 ? BACKWARD : LEVEL_BACKWARD, deltaTime);

    if (a && !d)
        cam[cameraState].move(cameraState == 0 ? LEFT : LEVEL_LEFT, deltaTime);
    else if (!a && d)
        cam[cameraState].move(cameraState == 0 ? RIGHT : LEVEL_RIGHT, deltaTime);
}
void Game::processRotate(bool q, bool e)
{
    if (cameraState == 0)
        return;
    if (q && !e)
    {
        cam[cameraState].rotate(-20, 0);
    }
    if (e && !q)
    {
        cam[cameraState].rotate(20, 0);
    }
}
void Game::processLight(bool i, bool j, bool k, bool l)
{
    if (i && !k)
    {
        light0.direction.x += 0.1;
    }
    else if (!i && k)
    {
        light0.direction.x -= 0.1;
    }
    if (j && !l)
    {
        light0.direction.z += 0.1;
    }
    else if (!j && l)
    {
        light0.direction.z -= 0.1;
    }
}
void Game::processMouseMove(double xoffset, double yoffset)
{
    cam[cameraState].rotate(xoffset, yoffset);
}

void Game::initModel()
{
    normalShader = getShaderProgram("../glsl/normal.vs", "../glsl/normal.fs");
    turret = new Model("../resources/models/turret1/turret.obj");
    box = new Model("../resources/models/woodbox/Wooden Crate.obj");
    virus = new Model("../resources/models/virusLow/virus.obj");
    box2 = new Model("../resources/models/woodbox2/Cube_obj.obj");
    sphere = new Model("../resources/models/home/HumanPlanet.obj");
    // vector<Mesh *> t;
    // t.push_back(new Sphere(1.0f));
    // sphere = new Model(t);
    // rotateMat = mat4(1.0f);
    // turret = new Model("../resources/models/test/nanosuit.obj");
}

void Game::drawModel(const glm::mat4 &projection, const glm::mat4 &view, Model *m, ShaderProgram &prgm)
{
    prgm.use();
    prgm.setMat4("projection", projection);
    prgm.setMat4("view", view);
    prgm.setMat4("model", modelStack.getModelMatrix());
    prgm.setMat3("normalMatrix", modelStack.getNormalMatrix());
    useLight(prgm, light0);
    if (en_light1 == false)
        prgm.setBool("en_light1", false);
    else
        useLight(prgm, light1);
    prgm.setVec3("viewPos", cam[cameraState].pos);
    m->draw(prgm);
}

void Game::initSky()
{
    skyShader = getShaderProgram("../glsl/skybox.vs", "../glsl/skybox.fs");
    sky.init();
}

void Game::updateSky(const glm::mat4 &projection, const glm::mat4 &view)
{
    mat4 view3 = mat4(mat3(view));
    skyShader.use();
    skyShader.setMat4("projection", projection);
    skyShader.setMat4("view", view3);
    glUseProgram(0);
}

void Game::drawSky()
{
    sky.draw(skyShader);
}

void Game::setWall(int x, int y)
{
    _M.ty[x][y] = 1;
}
