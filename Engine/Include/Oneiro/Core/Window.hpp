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
#include "Root.hpp"

#include <iostream>

namespace oe::Core
{
    class OE_API Window
    {
    public:
        struct Callbacks
        {
            typedef void (*frameBufferSizeCallback)(int w, int h);
            typedef void (*mouseButtonCallback)(Input::Button button, Input::Action action);
            typedef void (*keyCallback)(Input::Key key, Input::Action action);
            typedef void (*errorCallback)(int code, const char* description);
            typedef void (*focusCallback)(bool isFocused);
        };
    private:
        class WindowData;
        class WindowCallbacks;
    public:
        ~Window() { glfwDestroyWindow(mWindow); }

        bool Create();

        void SwapBuffers() { glfwSwapBuffers(mWindow); }
        static void PollEvents() { glfwPollEvents(); }
        static void WaitEvents() { glfwWaitEvents(); }

        [[nodiscard]] bool IsClosed() const { return glfwWindowShouldClose(mWindow); }
        const WindowData& GetData() { return mData; }
        inline GLFWwindow* GetGLFW() { return mWindow; }

        void SetAR(float ar) { mData.ar = ar; }
        void SetWidth(int w) { mData.width = w; }
        void SetHeight(int h) { mData.height = h; }
        void SetSize(int w, int h) { mData.width = w; mData.height = h; }

        static void SetFramerate(int fps) { glfwSwapInterval(fps); }
        static void SetKeyCallback(Callbacks::keyCallback kCallback);
        static void SetMouseButtonCallback(Callbacks::mouseButtonCallback mbCallback);
        static void SetFrameBufferSizeCallback(Callbacks::frameBufferSizeCallback fbsCallback);
        static void SetFocusCallback(Callbacks::focusCallback focusCallback);

        static void SetErrorCallback(Callbacks::errorCallback errorCallback);

        static void UpdateSize(int w, int h)
        {
            UpdateAR(w, h);
            Root::GetWindow()->SetSize(w, h);
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
            Callbacks::frameBufferSizeCallback frameBufferSize{};
            Callbacks::mouseButtonCallback mouseButton{};
            Callbacks::keyCallback key{};
            Callbacks::errorCallback error{};
            Callbacks::focusCallback focus{};
        };

        static void UpdateAR(int w, int h) { Root::GetWindow()->SetAR((float)w / (float)h); }
    private:
        static WindowCallbacks mCallbacks;
        WindowData mData{};
        GLFWwindow* mWindow{};
    };
}