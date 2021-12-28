#include <game.h>

#include <GLFW/glfw3.h>

#include <iostream>

Game myGame;

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

void processInput(GLFWwindow *window);
void frame_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_enter_callback(GLFWwindow *window, int entered);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 2);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TowerDefense", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_size_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    myGame.init();
    while (!glfwWindowShouldClose(window))
    {
        myGame.setTime(glfwGetTime());
        processInput(window);
        if(!myGame.getState())
        myGame.logic();
        myGame.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

bool cursorEnabled = true;
bool mouseInWindow = false;
double lastX, lastY;

inline bool keyPressed(GLFWwindow *window, int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    myGame.processKeyMove(
        keyPressed(window, GLFW_KEY_W),
        keyPressed(window, GLFW_KEY_A),
        keyPressed(window, GLFW_KEY_S),
        keyPressed(window, GLFW_KEY_D));
    myGame.processLight(
        keyPressed(window,GLFW_KEY_I),
        keyPressed(window, GLFW_KEY_J),
        keyPressed(window, GLFW_KEY_K),
        keyPressed(window, GLFW_KEY_L));
    myGame.processRotate(
        keyPressed(window, GLFW_KEY_Q),
        keyPressed(window, GLFW_KEY_E));
}
void frame_size_callback(GLFWwindow *window, int width, int height)
{
    myGame.setScrSize(width, height);
}
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (mouseInWindow && !myGame.getCameraState())
    {
        myGame.processMouseMove(xpos - lastX, lastY - ypos);
        lastX = xpos, lastY = ypos;
    }
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    myGame.zoomInOutCamera(yoffset);
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
}
void cursor_enter_callback(GLFWwindow *window, int entered)
{
    if (entered)
    {
        mouseInWindow = true;
        glfwGetCursorPos(window, &lastX, &lastY);
    }
    else
    {
        mouseInWindow = false;
    }
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        if (cursorEnabled)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwGetCursorPos(window, &lastX, &lastY);
        cursorEnabled = !cursorEnabled;
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        glm::vec3 cameraPos=myGame.getCameraPos(), cameraFront=myGame.getCameraFront();
        printf("Camera at (%.3lf,%.3lf,%.3lf)\n front (%.3lf, %.3lf,%.3lf)", cameraPos.x, cameraPos.y, cameraPos.z, cameraFront.x, cameraFront.y, cameraFront.z);
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        myGame.switchCamera();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        myGame.switchState();
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        myGame.edit();
    }
}