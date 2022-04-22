//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Event.hpp"
#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Runtime/Engine.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Core/Window.hpp"
//#include "Oneiro/Renderer/Gui/GuiLayer.hpp"
#include <memory>
#include <stdexcept>
#include <string>

namespace oe::Runtime
{
    void Engine::Init()
    {
        Core::Init();
        mRoot = new Core::Root;
        mWindow = new Core::Window;
        /*std::string glVersion = Core::Root::GetConfig("renderer")->GetValue("GL_VERSION");
        if (glVersion == "None")
        {
            glVersion = "3.3";
            Core::Root::GetConfig("renderer")->WriteData("GL_VERSION", glVersion);
        }
        mRoot->SetGLVersion(glVersion);
        mRoot->SetGLSLVersion(std::to_string(std::atoi(&glVersion[0])) +
							  std::to_string(std::atoi(&glVersion[2])) + "0");*/
    }

    void Engine::Run(const std::shared_ptr<Application>& app)
    {
        using namespace Core;
        mRoot->SetApplication(app.get());
        mRoot->SetWindow(mWindow);

        Event::Dispatcher::Subscribe<Event::ErrorEvent>([](const Event::Base& e)
			{
                const auto& errorEvent = dynamic_cast<const Event::ErrorEvent&>(e);
                log::get("log")->error("GLFW ERROR[" + std::to_string(errorEvent.Error) + "]: " +
												   errorEvent.Description);
            });
        if (!mWindow->Create())
            throw std::runtime_error("Failed to create window!");

        Window::SetFramerate(1);
        SetupEvents();

        Renderer::PreInit();

        if (!app->Init())
            throw std::runtime_error("Failed to initialize application!");

        Renderer::Init();

        //mRoot->LoadTexturesAsync();

    	while (!mWindow->IsClosed())
        {
            if (app->IsStopped())
                break;

            Window::PollEvents();

            //Renderer::GuiLayer::NewFrame();

            if (!app->Update())
                break;

            //Renderer::GuiLayer::Draw();

            mWindow->SwapBuffers();
        }

        app->Shutdown();
    }

    void Engine::Shutdown()
    {
        delete mWindow;
        delete mRoot;

        Renderer::Shutdown();
        Core::Shutdown();
    }

    void Engine::SetupEvents()
    {
        using namespace oe::Core;
        Event::Dispatcher::Subscribe<Event::FrameBufferSizeEvent>([](const Event::Base& e)
            {
                const auto& resizeEvent = dynamic_cast<const Event::FrameBufferSizeEvent&>(e);
                //Renderer::Viewport(resizeEvent.Width, resizeEvent.Height);
            });

        Event::Dispatcher::Subscribe<Event::KeyInputEvent>([](const Event::Base& e)
            {
                const auto& keyInputEvent = dynamic_cast<const Event::KeyInputEvent&>(e);
                Root::GetApplication()->HandleKey(static_cast<Input::Key>(keyInputEvent.Key),
                    static_cast<Input::Action>(keyInputEvent.Action));
            });

        Event::Dispatcher::Subscribe<Event::MouseButtonEvent>([](const Event::Base& e)
            {
                const auto& mouseButtonEvent = dynamic_cast<const Event::MouseButtonEvent&>(e);
                Root::GetApplication()->HandleButton(static_cast<Input::Button>(mouseButtonEvent.Button),
                    static_cast<Input::Action>(mouseButtonEvent.Action));
            });
        
        Event::Dispatcher::Subscribe<Event::FocusEvent>([](const Event::Base& e)
            {
                const auto& focusEvent = dynamic_cast<const Event::FocusEvent&>(e);
                if (focusEvent.IsFocused)
                    Window::SetFramerate(1); // 60 fps
                else
                    Window::SetFramerate(-3); // 15 fps
            });
    }

    Core::Root* Engine::mRoot{};
    Core::Window* Engine::mWindow{};
}
