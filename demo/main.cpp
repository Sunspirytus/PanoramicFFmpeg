#include <chrono>
#include <ratio>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/trigonometric.hpp"
#include "shader.h"
#include "postprocess.h"
#include "texture.h"
#include "video.h"
#include "timer.h"
#include "camera.h"

#define WIDTH 800
#define HEIGHT 400

int main(int argc, const char * argv[])
{
    if(!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "ffmpegdemo", nullptr, nullptr);
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

    Timer mTimer;

    Shader ViewerShader("demo/viewer.vs", "demo/viewer.fs", nullptr);
    Postprocess PostProcessor;

    Camera mCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, (float)WIDTH / (float)HEIGHT);

    //Texture Tex;
    //Tex.LoadTexture("pic.png");

    Video mVideo;
    mVideo.Load("time lapse_sunset in amsterdam_4k4k30_3dv.mp4");

    mTimer.SetFpsLimit(mVideo.GetFPS());

    uint32_t width = mVideo.GetWidth();
    uint32_t height = mVideo.GetHeight();
    VideoPixelFormat format = mVideo.GetFormat();

    Texture Y;
    Texture U;
    Texture V;
    if (format == VideoPixelFormat::YUV420P) 
    {
        Y.Create(width, height, TextureFormat::R8, nullptr);
        U.Create(width / 2, height / 2, TextureFormat::R8, nullptr);
        V.Create(width / 2, height / 2, TextureFormat::R8, nullptr);
    }
    else
    {
        std::cout << "not yuv 420p video" << std::endl;
        return -1;
    }

    // render loop
    std::vector<std::vector<uint8_t>> VideoImg;
    std::vector<glm::vec3> ViewportCornerDir(4);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 1.f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (mVideo.AcquireNextFrame(VideoImg))
        {
            Y.UpdateTextureData(VideoImg[0].data());
            U.UpdateTextureData(VideoImg[1].data());
            V.UpdateTextureData(VideoImg[2].data());
        }
        
        mCamera.GetViewportCornerDirection(ViewportCornerDir.data());

        ViewerShader.use();
        ViewerShader.setInt("inTextureY", 0);
        ViewerShader.setInt("inTextureU", 1);
        ViewerShader.setInt("inTextureV", 2);

        ViewerShader.setVec3("CornerDirection0", ViewportCornerDir[0]);
        ViewerShader.setVec3("CornerDirection1", ViewportCornerDir[1]);
        ViewerShader.setVec3("CornerDirection2", ViewportCornerDir[2]);
        ViewerShader.setVec3("CornerDirection3", ViewportCornerDir[3]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Y.GetGlTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, U.GetGlTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, V.GetGlTexture());

        PostProcessor.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        mTimer.SleepForFps();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    glfwTerminate();
    return 0;
}
