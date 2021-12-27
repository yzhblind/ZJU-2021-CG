#pragma once

#include <glad/glad.h>

#include "Glogic.h"  ////

class Game
{

///////////CYBBB_BEGIN
//deltaTime
public:
    
    _MAP _M;
    void MAP_init();
    UPD upd(); //传出当前场景
    void setT(int x, int y); //放塔
    void setE(int x, int y, int cd=CD_default); //放出怪点
    void delT(int x, int y); //撤塔
    void delE(int x, int y); //撤出怪点
    void setHome(int x, int y); //设置家

///////////CYBBB_END

public:
    void init();
    void logic();
    void render();
    void setTime(float t)
    {
        deltaTime = t - currentTime;
        currentTime = t;
    }

private:
    // 判定自由视角与塔防视角切换
    float scrWidth, scrHeight;
    int cameraState;
    Camera cam[2];
    float currentTime;
    float deltaTime;

    Skybox sky;
    ShaderProgram skyShader;

    void initSky();
    void updateSky(const glm::mat4 &projection, const glm::mat4 &view);
    void drawSky();
};
