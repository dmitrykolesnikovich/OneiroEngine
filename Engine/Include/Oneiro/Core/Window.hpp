//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"

namespace oe::Core
{
    class Window
    {
        struct WindowData;
    public:
        bool Create();
        ~Window();

        static void PollEvents();
        static void WaitEvents();

        void SwapBuffers();

        [[nodiscard]] bool IsClosed() const;

        void SetSize(int width, int height);

        [[nodiscard]] GLFWwindow* GetGLFW() const;

        [[nodiscard]] const char* GetTitle() const;
        [[nodiscard]] float GetAr() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        void UpdateSize(int width, int height);

        bool IsMouseButtonInput{true};
        bool IsKeyInput{true};
    private:
        void SetAR(float aspectRatio);
        void UpdateAR(int width, int height);

        struct WindowData
        {
            const char* Title{"Oneiro Engine"};
            float AR{};
            int Width{1280};
            int Height{720};
        };

        WindowData mData{};
        GLFWwindow* mWindow{};
    };
}
