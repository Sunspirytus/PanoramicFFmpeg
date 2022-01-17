#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "postprocess.h"
#include "texture.h"
#include "video.h"

int main(int argc, const char * argv[])
{
    if(!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "ffmpegdemo", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ViewerShader("demo/viewer.vs", "demo/viewer.fs", nullptr);
    Postprocess PostProcessor;
    Texture Tex;
    Tex.LoadTexture("pic.png");

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 1.f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        
        ViewerShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Tex.GetGlTexture());

        PostProcessor.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    glfwTerminate();
    return 0;
}
