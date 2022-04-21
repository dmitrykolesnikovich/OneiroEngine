//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "Root.hpp"

namespace oe::Core
{
    class Window
    {
        class WindowData;
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

        static void UpdateSize(int width, int height);

        bool IsMouseButtonInput{true};
        bool IsKeyInput{true};
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
        WindowData mData{};
        GLFWwindow* mWindow{};
    };
}