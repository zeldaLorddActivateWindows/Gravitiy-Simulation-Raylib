#pragma once  
#include <string>  

class Window
{
public:
    struct InitResources
    {
        const char* title_;
        int height_;
        int width_;
        int fps_;
        float g_;
    };

    Window(const char* title, int height, int width, int fps, float g) : resources{ title, height, width, fps, g } {}

    int GetHeight() const { return resources.height_; }
    int GetWidth() const { return resources.width_; }
    const char* GetTitle() const { return resources.title_; }
    int GetFps() const { return resources.fps_; }
    float GetG() const { return resources.g_; }

    void SetHeight(int height) { resources.height_ = height; }
    void SetWidth(int width) { resources.width_ = width; }
    void SetTitle(const char* title) { resources.title_ = title; }
    void SetFps(int fps) { resources.fps_ = fps; }
    void SetG(float g) { resources.g_ = g; }

private:
    InitResources resources;
};
