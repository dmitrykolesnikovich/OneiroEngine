//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Engine.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Core/Event.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Scene/SceneManager.hpp"
//#include "Oneiro/Renderer/Gui/GuiLayer.hpp"
#include "HazelAudio/HazelAudio.h"
#include "Oneiro/Core/ResourceManager.hpp"

namespace oe::Runtime
{
    void Engine::Init()
    {
        Core::Init();
        Renderer::PreInit();
        Hazel::Audio::Init();
        mRoot = new Core::Root;
        mWindow = new Core::Window;
    }

    void Engine::Run(const std::shared_ptr<Application>& app)
    {
        using namespace Core;

        if (mRoot == nullptr || mWindow == nullptr)
            return;

        mRoot->SetApplication(app.get());
        mRoot->SetWindow(mWindow);

        Event::Dispatcher::Subscribe<Event::ErrorEvent>([](const Event::Base& e)
                                                        {
                                                        const auto& errorEvent =
                                                                dynamic_cast<const Event::ErrorEvent&>(e);
                                                        log::get("log")->error("GLFW ERROR[" +
                                                                                       std::to_string(
                                                                                               errorEvent
                                                                                                       .Error) +
                                                                                       "]: " +
                                                                                       errorEvent
                                                                                               .Description);
                                                        });

        if (!mWindow->Create())
            throw std::runtime_error("Failed to create window!");

        SetupEvents();

        if (!app->Init())
            throw std::runtime_error("Failed to initialize application!");

        Renderer::Init();

        LoadResources();

        float lastFrame{};
        float last{};
        float currentFrame{};
        while (!mWindow->IsClosed())
        {
            currentFrame = glfwGetTime();
            mDeltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            if (!UpdateGame(app)) break;
        }

        app->Shutdown();
    }

    bool Engine::UpdateGame(const std::shared_ptr<Application>& app)
    {
        if (app->IsStopped())
            return false;

        Core::Window::PollEvents();
        gl::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

#ifdef OE_RENDERER_VULKAN
        Renderer::Vulkan::BeginScene();
        Renderer::GuiLayer::NewFrame();
#endif

        if (!app->Update())
            return false;
#ifdef OE_RENDERER_VULKAN
        Renderer::GuiLayer::Draw();
        Renderer::Vulkan::EndScene();
#else
        mWindow->SwapBuffers();
#endif
        return true;
    }

    void Engine::Shutdown()
    {
        delete mWindow;
        delete mRoot;

        Hazel::Audio::Shutdown();
        Renderer::Shutdown();
        Core::Shutdown();
    }

    void Engine::SetupEvents()
    {
        using namespace Core;
        Event::Dispatcher::Subscribe<Event::FrameBufferSizeEvent>([](const Event::Base& e)
                                                                  {
                                                                  const auto& resizeEvent =
                                                                          dynamic_cast<const Event::FrameBufferSizeEvent&>(e);
                                                                  if (resizeEvent.Width > 0 ||
                                                                          resizeEvent.Height > 0)
                                                                  {
                                                                      Root::GetWindow()->UpdateSize(
                                                                              resizeEvent.Width,
                                                                              resizeEvent.Height);
                                                                      Renderer::GL::Viewport(
                                                                              resizeEvent.Width,
                                                                              resizeEvent.Height);
                                                                  }
                                                                  });

        Event::Dispatcher::Subscribe<Event::KeyInputEvent>([](const Event::Base& e)
                                                           {
                                                           const auto& keyInputEvent =
                                                                   dynamic_cast<const Event::KeyInputEvent&>(e);
                                                           Root::GetApplication()->HandleKey(
                                                                   static_cast<Input::Key>(keyInputEvent
                                                                           .Key),
                                                                   static_cast<Input::Action>(keyInputEvent
                                                                           .Action));
                                                           });

        Event::Dispatcher::Subscribe<Event::MouseButtonEvent>([](const Event::Base& e)
                                                              {
                                                              const auto& mouseButtonEvent =
                                                                      dynamic_cast<const Event::MouseButtonEvent&>(e);
                                                              Root::GetApplication()->HandleButton(
                                                                      static_cast<Input::Button>(mouseButtonEvent
                                                                              .Button),
                                                                      static_cast<Input::Action>(mouseButtonEvent
                                                                              .Action));
                                                              });
    }

    Core::Root* Engine::mRoot{};
    Core::Window* Engine::mWindow{};
}
