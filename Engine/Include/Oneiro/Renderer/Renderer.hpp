//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once
#include <vulkan/vulkan_core.h>

#include "Oneiro/Core/Root.hpp"
#include "Vulkan/CommandBuffer.hpp"
#include "Vulkan/LogicalDevice.hpp"
#include "Vulkan/SwapChain.hpp"

//#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer
{
    /*enum DrawMode
    {
        TRIANGLES = gl::TRIANGLES
    };*/

    void PreInit();
    void Init();
    void Shutdown();
    /*void Viewport(GLsizei width, GLsizei height);
    void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void DrawArrays(DrawMode mode, GLint count);
    void DrawArraysInstanced(DrawMode mode, GLsizei arraysCount, GLsizei instanceCount);*/
    namespace Vulkan
    {
        void BeginScene();

        void EndScene();
    }
}