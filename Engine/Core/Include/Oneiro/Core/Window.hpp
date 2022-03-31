//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_DLL_EXPORT
#include "Oneiro.hpp"
#include "Input.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace oe::Core
{
    class OE_API Window
    {
    public:
        typedef void (*frameBufferSizeCallback)(int w, int h);
        typedef void (*mouseButtonCallback)(Input::Button button, Input::Action action);
        typedef void (*keyCallback)(Input::Key key, Input::Action action);
    private:
        class WindowData;
        class WindowCallbacks;
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

            if (mWindow == nullptr)
                return false;

            glfwMakeContextCurrent(mWindow);
            glfwSwapInterval(1);

//            glfwSetFramebufferSizeCallback(mWindow, Window::FrameBufferSizeCallback);
//            glfwSetKeyCallback(mWindow, Window::KeyCallback);
//            glfwSetMouseButtonCallback(mWindow, Window::MouseButtonCallback);

            UpdateAR(mData.width, mData.height);

            return true;
        }

        void SwapBuffers() { glfwSwapBuffers(mWindow); }
        static void PollEvents() { glfwPollEvents(); }
        static void WaitEvents() { glfwWaitEvents(); }

        [[nodiscard]] bool IsClosed() const { return glfwWindowShouldClose(mWindow); }
        const WindowData& GetData() { return mData; }
        static inline GLFWwindow* GetGLFW() { return Get()->mWindow; }
        static inline Window* Get() { return mInstance; }

        void SetAR(float ar) { mData.ar = ar; }
        void SetWidth(int w) { mData.width = w; }
        void SetHeight(int h) { mData.height = h; }
        void SetSize(int w, int h) { mData.width = w; mData.height = h; }

        static void SetKeyCallback(keyCallback kCallback)
        {
            mCallbacks.key = kCallback;
            glfwSetKeyCallback(GetGLFW(), [](GLFWwindow*, int key, int action, int, int)
            {
                Window::mCallbacks.key(static_cast<Input::Key>(key), static_cast<Input::Action>(action));
            });
        }

        static void SetMouseButtonCallback(mouseButtonCallback mbCallback)
        {
            mCallbacks.mouseButton = mbCallback;
            glfwSetMouseButtonCallback(GetGLFW(), [](GLFWwindow*, int button, int action, int)
            {
                Window::mCallbacks.mouseButton(static_cast<Input::Button>(button), static_cast<Input::Action>(action));
            });
        }

        static void SetFrameBufferSizeCallback(frameBufferSizeCallback fbsCallback)
        {
            mCallbacks.frameBufferSize = fbsCallback;
            glfwSetFramebufferSizeCallback(GetGLFW(),[](GLFWwindow*, int w, int h)
            {
                Window::mCallbacks.frameBufferSize(w, h);
                Window::UpdateSize(w, h);
            });
        }

        static void UpdateSize(int w, int h)
        {
            UpdateAR(w, h);
            Get()->SetSize(w, h);
        }
    private:
        struct WindowData
        {
            const char* title{ "OpenGL Window" };
            float ar{};
            uint16_t width{ 1280 };
            uint16_t height{ 720 };
        };
        struct WindowCallbacks
        {
            frameBufferSizeCallback frameBufferSize{};
            mouseButtonCallback mouseButton{};
            keyCallback key{};
        };

        static void UpdateAR(int w, int h) { Get()->SetAR((float)w / (float)h); }

    private:
        static Window* mInstance;
        static WindowCallbacks mCallbacks;
        WindowData mData{};
        GLFWwindow* mWindow{};
    };
}