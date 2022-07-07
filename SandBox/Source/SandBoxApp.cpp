//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "SandBoxApp.hpp"

namespace SandBox
{
    bool Application::OnInit()
    {
        using namespace oe;

        SetMode(Input::CURSOR, Input::CURSOR_DISABLED);

        gl::CullFace(gl::BACK);
        gl::FrontFace(gl::CCW);

        if (World::World::IsExists("main"))
        {
            mWorld = World::World::Load("main");
            return mWorld.get();
        }

        mWorld = std::make_shared<World::World>("Main", "main");
        auto player = mWorld->CreateEntity("Player");
        player.AddComponent<MainCameraComponent>();

        mWorld->CreateEntity("Backpack").AddComponent<ModelComponent>().Model->Load("Assets/Models/backpack/backpack.obj");
        mWorld->CreateEntity("Cube").AddComponent<ModelComponent>().Model->Load("Assets/Models/cube.fbx");

        mWorld->CreateEntity("Plane").AddComponent<ModelComponent>().Model->Load({
            {{1.0f, 1.0f, 1.0f, 1.0f}, { 10.0f, -3.0f,  10.0f}, {0.0f, 1.0f, 0.0f}, {10.0f,  0.0f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {-10.0f, -3.0f,  10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f,  0.0f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {-10.0f, -3.0f, -10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 10.0f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, { 10.0f, -3.0f,  10.0f}, {0.0f, 1.0f, 0.0f}, {10.0f,  0.0f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, {-10.0f, -3.0f, -10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 10.0f}},
            {{1.0f, 1.0f, 1.0f, 1.0f}, { 10.0f, -3.0f, -10.0f}, {0.0f, 1.0f, 0.0f}, {10.0f, 10.0f}}
        });

        return true;
    }

    bool Application::OnUpdate(float deltaTime)
    {
        using namespace oe;

        const auto playerEntity = mWorld->GetEntity("Player");
        const auto backpackEntity = mWorld->GetEntity("Backpack");
        const auto cubeEntity = mWorld->GetEntity("Cube");

        auto& mainCamera = playerEntity.GetComponent<MainCameraComponent>();

        auto& backpackTransform = backpackEntity.GetComponent<TransformComponent>();
        auto& playerTransform = playerEntity.GetComponent<TransformComponent>();
        auto& cubeTransform = cubeEntity.GetComponent<TransformComponent>();

        if (IsKey(Input::Action::PRESS, Input::Key::W))
            mainCamera.UpdateForward(deltaTime);
        if (IsKey(Input::Action::PRESS, Input::Key::S))
            mainCamera.UpdateBackward(deltaTime);
        if (IsKey(Input::Action::PRESS, Input::Key::D))
            mainCamera.UpdateRight(deltaTime);
        if (IsKey(Input::Action::PRESS, Input::Key::A))
            mainCamera.UpdateLeft(deltaTime);

        backpackTransform.Rotation += glm::vec3(deltaTime);
        playerTransform.Translation = mainCamera.Position;
        cubeTransform.Translation = glm::vec3(5.5f, 1.0f, 0.5f);
        cubeTransform.Rotation.x = 90.0f;

        mWorld->UpdateEntities();

        return true;
    }

    void Application::OnEvent(const oe::Core::Event::Base& e)
    {
        if (typeid(e) == typeid(oe::Core::Event::CursorPosEvent))
        {
            const auto& cursorEvent = dynamic_cast<const oe::Core::Event::CursorPosEvent&>(e);
            auto& mainCamera = mWorld->GetEntity("Player").GetComponent<oe::MainCameraComponent>();
            mainCamera.UpdateMouse(static_cast<float>(cursorEvent.XPos), static_cast<float>(cursorEvent.YPos));
        }
    }

    void Application::OnShutdown()
    {
        mWorld->Save(true);
    }
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char*[])
    {
        return std::make_shared<SandBox::Application>("SandBox", 1280, 720);
    }
}