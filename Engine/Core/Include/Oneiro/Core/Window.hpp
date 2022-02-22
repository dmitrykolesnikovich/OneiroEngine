//
// Created by Dezlow on 20.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//

#pragma once

#ifndef ONEIRO_CORE_WINDOW_HPP
#define ONEIRO_CORE_WINDOW_HPP

#define OE_DLL_EXPORT
#include "Oneiro.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace oe
{
    class OE_API Window
    {
        struct WindowData
        {
            uint16_t width{1280};
            uint16_t height{720};
            const char* title{"Oneiro Engine"};
        };

        static Window* mInstance;
    public:
        ~Window() { glfwDestroyWindow(mWindow); }

        bool Create()
        {
            mInstance = this;

            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            mWindow = glfwCreateWindow(mData.width, mData.height, mData.title, nullptr, nullptr);

            glfwMakeContextCurrent(mWindow);

            if (mWindow == nullptr)
                return false;

            return true;
        }

        bool isClosed() const { return glfwWindowShouldClose(mWindow); }
        void SwapBuffers() { glfwSwapBuffers(mWindow); }

        GLFWwindow* GetGLFWWindow() { return mWindow; }
        const WindowData* GetWindowData() const { return &mData; }

        static Window* Get() { return mInstance; }
        static void PollEvents() { glfwPollEvents(); }
        static void WaitEvents() { glfwWaitEvents(); }
    private:
        WindowData mData;
        GLFWwindow* mWindow;
    };
};

OE_API oe::Window* oe::Window::mInstance{};

#endif //ONEIRO_CORE_WINDOW_HPP
