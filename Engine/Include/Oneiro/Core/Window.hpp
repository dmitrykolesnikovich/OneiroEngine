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

namespace oe::Core
{
    class OE_API Window
    {
    public:
        struct Callbacks
        {
            typedef void (*frameBufferSizeCallback)(int width, int height);
            typedef void (*mouseButtonCallback)(Input::Button button, Input::Action action);
            typedef void (*keyCallback)(Input::Key key, Input::Action action);
            typedef void (*errorCallback)(int code, const char* description);
            typedef void (*focusCallback)(bool isFocused);
        };
    private:
        class WindowData;
        class WindowCallbacks;
    public:
        ~Window();

        bool Create();

        void SwapBuffers() const;
        static void PollEvents();
        static void WaitEvents();

        [[nodiscard]] bool IsClosed() const;
        [[nodiscard]] const WindowData& GetData() const;
        [[nodiscard]] GLFWwindow* GetGLFW() const;

        void SetWidth(int width) const;
        void SetHeight(int height) const;
        void SetSize(int width, int height);

        static void SetFramerate(int fps);
        static void SetKeyCallback(Callbacks::keyCallback kCallback);
        static void SetMouseButtonCallback(Callbacks::mouseButtonCallback mbCallback);
        static void SetFrameBufferSizeCallback(Callbacks::frameBufferSizeCallback fbsCallback);
        static void SetFocusCallback(Callbacks::focusCallback focusCallback);

        static void SetErrorCallback(Callbacks::errorCallback errorCallback);

        static void UpdateSize(int width, int height);
    private:
        void SetAR(float aspectRatio);
        static void UpdateAR(int width, int height);

        struct WindowData
        {
            const char* title{"Oneiro Engine"};
            float ar{};
            int width{1280};
            int height{720};
        };

        struct WindowCallbacks
        {
            Callbacks::frameBufferSizeCallback frameBufferSize{};
            Callbacks::mouseButtonCallback mouseButton{};
            Callbacks::keyCallback key{};
            Callbacks::errorCallback error{};
            Callbacks::focusCallback focus{};
        };

        static WindowCallbacks mCallbacks;
        WindowData mData{};
        GLFWwindow* mWindow{};
    };
}