#include <game.h>

#include <GLFW/glfw3.h>

Game myGame;

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

void processInput(GLFWwindow *window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TowerDefense", NULL, NULL);
    glfwMakeContextCurrent(window);
    myGame.init();
    while (!glfwWindowShouldClose(window))
    {
        myGame.setTime(glfwGetTime());
        processInput(window);
        myGame.logic();
        myGame.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
}