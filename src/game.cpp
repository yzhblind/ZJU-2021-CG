#include "game.h"

void Game::init()
{
    glEnable(GL_DEPTH_TEST);
    currentTime = deltaTime = 0.0f;
}

void Game::logic()
{

}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}