//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_DLL_EXPORT
#include "Oneiro.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace oe::Core
{
    class OE_API Window
    {
        struct WindowData
        {
            uint16_t width{ 1280 };
            uint16_t height{ 720 };
            const char* title{ "Oneiro Engine" };
        };

        static Window* mInstance;
    public:
        ~Window() { glfwDestroyWindow(mWindow); }

        bool Create()
        {
            mInstance = this;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            mWindow = glfwCreateWindow(mData.width, mData.height, mData.title, nullptr, nullptr);

            glfwMakeContextCurrent(mWindow);
            glfwSwapInterval(1);
            UpdateAR(mData.width, mData.height);

            if (mWindow == nullptr)
                return false;

            return true;
        }

        [[nodiscard]] bool IsClosed() const { return glfwWindowShouldClose(mWindow); }
        void SwapBuffers() { glfwSwapBuffers(mWindow); }
        const WindowData& GetData() { return mData; }

        static GLFWwindow* GetGLFWWindow() { return Get()->mWindow; }
        [[nodiscard]] const WindowData* GetWindowData() const { return &mData; }

        static Window* Get() { return mInstance; }
        static void PollEvents() { glfwPollEvents(); }
        static void WaitEvents() { glfwWaitEvents(); }

        int GetWidth()
        {
            int w{};
            glfwGetWindowSize(mWindow, &w, nullptr);
            return w;
        }

        int GetHeight()
        {
            int h{};
            glfwGetWindowSize(mWindow, nullptr, &h);
            return h;
        }

        static void UpdateSize(int w, int h)
        {
            UpdateAR(w, h);
            Get()->mData.width = w;
            Get()->mData.height = h;
        }

        [[nodiscard]] float GetAR() const { return mAR; }
    private:
        static void UpdateAR(int w, int h)
        {
            Get()->mAR = (float)w / (float)h;
        }
        WindowData mData{};
        GLFWwindow* mWindow{};
        float mAR{};
    };
}