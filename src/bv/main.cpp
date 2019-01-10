#include <iostream>
#include <string>
#include <memory>
#include <bv/BVRenderer.h>
#include <AntTweakBar.h>



void resize(GLFWwindow *window, int width, int height);
void destroy(GLFWwindow *window);
void input(GLFWwindow *window, int key, int scancode, int action, int modifiers);
void errorHandler(int error, const char* description);
void cursorPos(GLFWwindow *window, double x, double y);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scrollInput(GLFWwindow *window, double x, double y);
void charInput(GLFWwindow *window, unsigned int c);

void update();
void render();


std::unique_ptr<cgbv::Renderer> renderer;

int main()
{
    if(!glfwInit())
    {
        std::cout << "GLFW failed!" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(errorHandler);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Bildverarbeitung", nullptr, nullptr);
    if(!window)
    {
        std::cout << "Failed to open window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

    renderer = std::make_unique<cgbv::BVRenderer>(window);
    if(!renderer->setup())
    {
        return -1;
    }


    glfwSetWindowCloseCallback(window, destroy);
    glfwSetKeyCallback(window, input);
    glfwSetFramebufferSizeCallback(window, resize);

	glfwSetCursorPosCallback(window, cursorPos);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetScrollCallback(window, scrollInput);
	glfwSetCharCallback(window, charInput);



    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSwapInterval(0);

    do
    {
        render();
        update();

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(!glfwWindowShouldClose(window));

    glfwDestroyWindow(window);
    window = nullptr;


    TwTerminate();
    glfwTerminate();

    return 0;
}


void resize(GLFWwindow *window, int width, int height)
{
    renderer->resize(width, height);
}


void destroy(GLFWwindow *window)
{
    renderer->destroy();
}


void input(GLFWwindow *window, int key, int scancode, int action, int modifiers)
{
    renderer->input(key, scancode, action, modifiers);
}


void errorHandler(int error, const char* description)
{
    std::string msg(description);
    std::cout << msg << std::endl;
}


void render()
{
    renderer->render();
}


void update()
{
    renderer->update();
}


void cursorPos(GLFWwindow *window, double x, double y)
{
	renderer->cursorPosition(x, y);
}


void mouseButton(GLFWwindow *window, int button, int action, int mods)
{
	renderer->mouseButton(button, action, mods);
}


void scrollInput(GLFWwindow *window, double x, double y)
{
	renderer->scrollInput(x, y);
}


void charInput(GLFWwindow *window, unsigned int c)
{
	renderer->charInput(c);
}